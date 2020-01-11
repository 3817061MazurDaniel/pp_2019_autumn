// Copyright 2019 Zinkov Artem
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <utility>
#include "../../../modules/task_3/zinkov_radix_sort_merge_batcher/radix_sort_merge_batcher.h"

std::vector<int> getRandomVector(int length) {
  if (length < 1)
    throw "WRONG_LEN";

  std::vector<int> vec(length);
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));

  for (auto& val : vec) {
    val = gen() % 100000;
  }

  return vec;
}

std::vector<int> merge_batcher(std::vector<int> global_vec, int size_vec) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int delta = size_vec / size;
  const int residue = size_vec % size;
  std::vector<int> local_vec;
  if (size_vec < size || size == 1) {
    if (rank == 0 )
      global_vec = radix_sort(global_vec);
    return global_vec;
  }

  if (rank == 0) {
    local_vec.reserve(size_vec);
    local_vec.resize(delta + residue);
  } else {
    local_vec.resize(delta);
  }

  int* sendcounts = new int[size];
  int* displs = new int[size];

  for (int i = 0; i < size; i++) {
    displs[i] = 0;
    if (i == 0) {
      sendcounts[i] = delta + residue;
    } else {
      sendcounts[i] = delta;
    }
    if (i > 0) {
      displs[i] = displs[i - 1] + sendcounts[i - 1];
    }
  }

  MPI_Scatterv(global_vec.data(), sendcounts, displs, MPI_INT,
    &local_vec.front(), sendcounts[rank], MPI_INT, 0, MPI_COMM_WORLD);

  int num_merge = 1;
  while (pow(2, num_merge) < size)
    num_merge++;

  local_vec = radix_sort(local_vec);
  local_vec = shuffle(local_vec);

  int merged_proc = 2, displs_proc = 1, length_send, length_recv;
  std::vector<int> res, even, odd;
  MPI_Status status;

  for (int i = 0; i < num_merge; i++) {
    if (rank % merged_proc == 0 && rank + displs_proc < size) {
      length_send = local_vec.size() / 2;

      MPI_Sendrecv(&length_send, 1, MPI_INT, rank + displs_proc, 0,
        &length_recv, 1, MPI_INT, rank + displs_proc, 0, MPI_COMM_WORLD, &status);
      res.resize(length_recv / 2 + length_recv % 2);
      MPI_Sendrecv(&local_vec[local_vec.size() / 2 + local_vec.size() % 2], length_send,
        MPI_INT, rank + displs_proc, 0, &res.front(), length_recv / 2 + length_recv % 2,
        MPI_INT, rank + displs_proc, 0, MPI_COMM_WORLD, &status);

      even = merge_even(local_vec, res);

      odd.resize(length_recv / 2 + local_vec.size() / 2);
      MPI_Recv(&odd.front(), length_recv / 2 + local_vec.size() / 2, MPI_INT,
        rank + displs_proc, 0, MPI_COMM_WORLD, &status);

      local_vec.resize(even.size() + odd.size());
      std::copy(even.begin(), even.end(), local_vec.begin());
      std::copy(odd.begin(), odd.end(), local_vec.begin() + even.size());

      if (i + 1 != num_merge)
        local_vec = transpos(local_vec, even.size(), odd.size());
      else
        local_vec = merge(local_vec, even.size(), odd.size());
    }
    if (rank - displs_proc >= 0 && (rank - displs_proc) % merged_proc == 0) {
      length_send = local_vec.size();
      MPI_Sendrecv(&length_send, 1, MPI_INT, rank - displs_proc, 0, &length_recv, 1,
        MPI_INT, rank - displs_proc, 0, MPI_COMM_WORLD, &status);
      res.resize(length_recv);
      MPI_Sendrecv(local_vec.data(), length_send / 2 + length_send % 2, MPI_INT,
        rank - displs_proc, 0, &res.front(), length_recv, MPI_INT,
        rank - displs_proc, 0, MPI_COMM_WORLD, &status);
      odd = merge_odd(local_vec, res);

      MPI_Send(odd.data(), odd.size(), MPI_INT, rank - displs_proc, 0, MPI_COMM_WORLD);
    }
    merged_proc *= 2;
    displs_proc *= 2;
  }
  return local_vec;
}

std::vector<int> shuffle(std::vector<int> vec) {
  std::vector<int> tmp(vec.size());

  for (size_t i = 0; i < vec.size() / 2 + vec.size() % 2; i++) {
    tmp[i] = vec[2 * i];
  }
  for (size_t i = 1; i < vec.size(); i += 2) {
    tmp[vec.size() / 2 + vec.size() % 2 + i / 2] = vec[i];
  }
  for (size_t i = 0; i < vec.size(); i++) {
    vec[i] = tmp[i];
  }
  return vec;
}

std::vector<int> merge_even(const std::vector<int>& vec1, const std::vector<int>& vec2) {
  std::vector<int> res(vec1.size() / 2 + vec1.size() % 2 + vec2.size());
  size_t j = 0, k = 0;
  int l = 0;

  while (j < (vec1.size() / 2 + vec1.size() % 2) && k < (vec2.size())) {
    if (vec1[j] < vec2[k])
      res[l++] = vec1[j++];
    else
      res[l++] = vec2[k++];
  }

  while (j < vec1.size() / 2 + vec1.size() % 2)
    res[l++] = vec1[j++];
  while (k < vec2.size())
    res[l++] = vec2[k++];
  return res;
}

std::vector<int> merge_odd(const std::vector<int>& vec1, const std::vector<int>& vec2) {
  std::vector<int> res(vec1.size() / 2 + vec2.size());
  size_t j = vec1.size() / 2 + vec1.size() % 2, k = 0;
  int l = 0;

  while (j < vec1.size() && k < vec2.size()) {
    if (vec1[j] < vec2[k])
      res[l++] = vec1[j++];
    else
      res[l++] = vec2[k++];
  }

  while (j < vec1.size())
    res[l++] = vec1[j++];
  while (k < vec2.size())
    res[l++] = vec2[k++];
  return res;
}


std::vector<int> transpos(std::vector<int> vec, int even_size, int odd_size) {
  if (even_size - odd_size == 2) {
    std::vector<int> res(vec.size());
    int j = 0, k = 0, l = 0;

    while (j < even_size && k < odd_size) {
      res[l++] = vec[j++];
      res[l++] = vec[even_size + k];
      k++;
    }

    while (j < even_size)
      res[l++] = vec[j++];

    for (size_t i = 1; i < res.size() - 1; i += 2) {
      if (res[i] > res[i + 1])
        std::swap(res[i], res[i + 1]);
    }

    res = shuffle(res);
    return res;

  } else {
    for (int i = 0; i < even_size - 1; i++)
      if (vec[1 + i] < vec[even_size + i])
        std::swap(vec[1 + i], vec[even_size + i]);
    return vec;
  }
}

std::vector<int>  merge(std::vector<int> vec, int even_size, int odd_size) {
  std::vector<int> res(vec.size());
  int j = 0, k = 0, l = 0;

  while (j < even_size && k < odd_size) {
    res[l++] = vec[j++];
    res[l++] = vec[even_size + k];
    k++;
  }

  while (j < even_size)
    res[l++] = vec[j++];

  for (size_t i = 1; i < res.size() - 1; i += 2) {
    if (res[i] > res[i + 1])
      std::swap(res[i], res[i + 1]);
  }

  return res;
}

std::vector<int> radix_sort(std::vector<int> vec) {
  int max = 0, digit_pos = 1;
  std::vector<int> res(vec.size());
  max = *std::max_element(vec.begin(), vec.end());

  while (max / digit_pos > 0) {
    int digit_count[10] = { 0 };
    for (size_t i = 0; i < vec.size(); i++)
      digit_count[vec[i] / digit_pos % 10]++;

    for (int i = 1; i < 10; i++)
      digit_count[i] += digit_count[i - 1];

    for (int i = vec.size() - 1; i >= 0; i--)
      res[--digit_count[vec[i] / digit_pos % 10]] = vec[i];

    for (size_t i = 0; i < vec.size(); i++)
      vec[i] = res[i];

    digit_pos *= 10;
  }
  return vec;
}

// Copyright 2019 Karin Timofey
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <utility>
#include "../../../modules/task_2/karin_bubble_sort/bubble_sort.h"

std::vector<int> GetRandVec(int size) {
  if (size <= 0)
    throw - 1;
  std::vector<int> vec(size);
  std::mt19937 el;
  el.seed(static_cast<unsigned int>(time(0)));
  for (int i = 0; i < size; i++) {
    vec[i] = el() % 1000;
  }
  return vec;
}

std::vector<int> BubbleSort(const std::vector<int>& vect, int length) {
  std::vector<int> res(vect);
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < length - i - 1; j++) {
      if (res[j] > res[j + 1])
        std::swap(res[j], res[j + 1]);
    }
  }
  return res;
}

std::vector<int> GetMinVec(const std::vector<int>& local_vec, int local_size,
                            const std::vector<int>& neig_vec, int neig_size) {
  std::vector<int> res(local_vec);
  int it1 = 0;
  int it2 = 0;
  for (int i = 0; i < local_size; i++) {
    if ((local_vec[it1] > neig_vec[it2]) && (it2 < neig_size)) {
      res[i] = neig_vec[it2];
      it2++;
    } else {
      res[i] = local_vec[it1];
      it1++;
    }
  }
  return res;
}

std::vector<int> GetMaxVec(const std::vector<int>& local_vec, int local_size,
                            const std::vector<int>& neig_vec, int neig_size) {
  std::vector<int> res(local_vec);
  int it1 = local_size - 1;
  int it2 = neig_size - 1;
  for (int i = local_size - 1; i >= 0; i--) {
    if ((local_vec[it1] < neig_vec[it2]) && (it2 >=0)) {
      res[i] = neig_vec[it2];
      it2--;
    } else {
      res[i] = local_vec[it1];
      it1--;
    }
  }
  return res;
}

std::vector<int> ParBubbleSort(const std::vector<int>& vect) {
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int length;
  int local_size, neig_size, neig_rank;

  if (rank == 0) {
    length = vect.size();
    for (int i = 1; i < size; i++)
      MPI_Send(&length, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
  } else {
    MPI_Status st1;
    MPI_Recv(&length, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &st1);
  }

  int num = length / size;
  int ost = length % size;

  if (rank == 0) {
    local_size = num+ost;
    neig_size = num;
  } else if (rank == 1) {
    local_size = num;
    neig_size = num+ost;
  } else {
    local_size = neig_size = num;
  }
  std::vector<int> local_vec(local_size);
  std::vector<int> neig_vec(neig_size);
  std::vector<int> res(local_size);

  if (rank == 0) {
    for (int i = 1; i < size; i++)
      MPI_Send(&vect[0]+ost+i*num, num, MPI_INT, i, 2, MPI_COMM_WORLD);
    for (int i = 0; i < local_size; i++)
      local_vec[i] = vect[i];
  } else {
    MPI_Status st;
    MPI_Recv(&local_vec[0], num, MPI_INT, 0, 2, MPI_COMM_WORLD, &st);
  }

  local_vec = BubbleSort(local_vec, local_size);

  for (int i = 0; i < size+ost; i++) {
    if (i%2 == 0) {  // even iter
      if (rank % 2 == 0) {  // even proc
        neig_rank = rank + 1;
        if (neig_rank >= size)
          continue;
        MPI_Status st5;
        MPI_Send(&local_vec[0], local_size, MPI_INT, neig_rank, 4, MPI_COMM_WORLD);
        MPI_Recv(&neig_vec[0], neig_size, MPI_INT, neig_rank, 5, MPI_COMM_WORLD, &st5);
        local_vec = GetMinVec(local_vec, local_size, neig_vec, neig_size);
      } else {  // odd proc
        neig_rank = rank - 1;
        if (neig_rank < 0)
          continue;
        if (neig_rank == 0)
          neig_size = num + ost;
        MPI_Status st4;
        MPI_Recv(&neig_vec[0], neig_size, MPI_INT, neig_rank, 4, MPI_COMM_WORLD, &st4);
        MPI_Send(&local_vec[0], local_size, MPI_INT, neig_rank, 5, MPI_COMM_WORLD);
        local_vec = GetMaxVec(local_vec, local_size, neig_vec, neig_size);
      }
    } else {  // odd iter
      if (rank % 2 == 0) {  // even proc
        neig_rank = rank - 1;
        if (neig_rank < 0)
          continue;
        MPI_Status st7;
        MPI_Send(&local_vec[0], local_size, MPI_INT, neig_rank, 7, MPI_COMM_WORLD);
        MPI_Recv(&neig_vec[0], neig_size, MPI_INT, neig_rank, 6, MPI_COMM_WORLD, &st7);
        local_vec = GetMaxVec(local_vec, local_size, neig_vec, neig_size);
      } else {  // odd proc
        neig_rank = rank + 1;
        if (neig_rank >= size)
          continue;
        if (rank == 1)
          neig_size = num;
        MPI_Status st6;
        MPI_Recv(&neig_vec[0], neig_size, MPI_INT, neig_rank, 7, MPI_COMM_WORLD, &st6);
        MPI_Send(&local_vec[0], local_size, MPI_INT, neig_rank, 6, MPI_COMM_WORLD);
        local_vec = GetMinVec(local_vec, local_size, neig_vec, neig_size);
      }
    }
  }

  if (rank == 0) {
    res = local_vec;
    for (int i = 1; i < size; i++) {
      MPI_Status status;
      MPI_Recv(&neig_vec[0], num, MPI_INT, i, 8, MPI_COMM_WORLD, &status);
      res.insert(res.end(), neig_vec.begin(), neig_vec.end());
    }
  } else {
    MPI_Send(&local_vec[0], num, MPI_INT, 0, 8, MPI_COMM_WORLD);
  }
  return res;
}

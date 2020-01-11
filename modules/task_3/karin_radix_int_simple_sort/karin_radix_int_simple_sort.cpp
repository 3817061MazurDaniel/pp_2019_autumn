// Copyright 2019 Karin Timofey
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_3/karin_radix_int_simple_sort/karin_radix_int_simple_sort.h"

std::vector<int> GetRandVec(int size) {
  if (size <= 0)
    throw - 1;
  std::vector<int> vec(size);
  std::mt19937 el;
  el.seed(static_cast<unsigned int>(time(0)));
  for (int i = 0; i < size; i++) {
    vec[i] = el() % 1000000000;
  }
  return vec;
}

std::vector<int> Radix_sort_iter(const std::vector<int>& source, int n, int iter) {
  int* count = new int[256];
  std::vector<int> res(source.size());
  unsigned char* br = (unsigned char*)source.data() + iter;
  for (int i = 0; i < 256; i++)
    count[i] = 0;

  unsigned char tmp;
  for (int i = 0; i < n; i++) {
    tmp = br[i*4];
    count[tmp]++;
  }

  int sum = 0;
  if (iter == 3) {
    for (int i = 128; i < 256; i++) {
      sum += count[i];
      count[i] = sum - count[i];
    }

    for (int i = 0; i < 128; i++) {
      sum += count[i];
      count[i] = sum - count[i];
    }
  } else {
    for (int i = 0; i < 256; i++) {
      sum+=count[i];
      count[i] = sum - count[i];
    }
  }

  for (int i = 0; i < n; i++) {
    res[count[*br]] = source[i];
    count[*br]++;
    br += 4;
  }
  delete[] count;
  return res;
}

std::vector<int> Radix_sort(const std::vector<int>& vec) {
  int length = vec.size();
  std::vector<int> res(vec);

  for (int i = 0; i < 4; i++)
    res = Radix_sort_iter(res, length, i);

  return res;
}

std::vector<int> Merge_sort(const std::vector<int>& loc_vec, int loc_size,
                              const std::vector<int>& neig_vec, int neig_size) {
  std::vector<int> res(loc_size + neig_size);
  int l = 0, n = 0, r = 0;
  while (l < loc_size && n < neig_size) {
    if (loc_vec[l] < neig_vec[n]) {
      res[r] = loc_vec[l];
      r++;
      l++;
    } else {
      res[r] = neig_vec[n];
      n++;
      r++;
    }
  }
  while (l < loc_size) {
    res[r] = loc_vec[l];
    r++;
    l++;
  }
  while (n < neig_size) {
    res[r] = neig_vec[n];
    r++;
    n++;
  }
  return res;
}

int pow2(int st) {
  int res = 1;
  for (int i = 0; i < st; i++)
    res *= 2;
  return res;
}

std::vector<int> Par_Radix_sort(const std::vector<int>& source) {
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int length, ost;
  if (rank == 0) {
    length = source.size()/size;
    ost = source.size() % size;
  }
  MPI_Bcast(&length, 1, MPI_INT, 0, MPI_COMM_WORLD);

  std::vector<int> local_vec(length);
  MPI_Scatter(&source[0], length, MPI_INT, &local_vec[0], length, MPI_INT, 0, MPI_COMM_WORLD);
  if (rank == 0 && ost != 0) {
      local_vec.insert(local_vec.end(), source.end()-ost, source.end());
  }

  local_vec = Radix_sort(local_vec);

  int i = size;
  int iter = 1;
  int iter_length = length;
  int sosed;
  int loc_size = length;
  if (rank == 0)
    loc_size += ost;
  while (i > 1) {
    if (i % 2 == 1) {
      if (rank == 0) {
        MPI_Status status1;
        std::vector<int>neig_vec(iter_length);
        sosed = pow2(iter - 1) * (i - 1);
        MPI_Recv(&neig_vec[0], iter_length, MPI_INT, sosed, 1, MPI_COMM_WORLD, &status1);
        local_vec = Merge_sort(local_vec, loc_size, neig_vec, iter_length);
        loc_size += iter_length;
      }
      if (rank == pow2(iter - 1) * (i - 1)) {
        MPI_Send(&local_vec[0], iter_length, MPI_INT, 0, 1, MPI_COMM_WORLD);
        return local_vec;
      }
    }

    if (rank % pow2(iter) == 0) {
      sosed = rank + pow2(iter-1);
      std::vector<int> neig_vec(iter_length);
      MPI_Status status3;
      MPI_Recv(&neig_vec[0], iter_length, MPI_INT, sosed, 3, MPI_COMM_WORLD, &status3);
      local_vec = Merge_sort(local_vec, loc_size, neig_vec, iter_length);
      loc_size += iter_length;
    }
    if (rank % pow2(iter) != 0) {
      sosed = rank - pow2(iter-1);
      MPI_Send(&local_vec[0], iter_length, MPI_INT, sosed, 3, MPI_COMM_WORLD);
      return local_vec;
    }
    iter++;
    i = i/2;
    iter_length *= 2;
  }
  return local_vec;
}

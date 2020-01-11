// Copyright 2019 Makarova Viktoria
#include <mpi.h>
#include <random>
#include <utility>
#include <vector>
#include "../../../modules/task_3/makarova_v_raz_sort/razr_sort.h"

int pow10(int pow) {
    int res = 1;
    for (int i = 0; i < pow; ++i)
        res *= 10;

    return res;
}

int pow2(int pow) {
    int res = 1;
    for (int i = 0; i < pow; ++i)
        res *= 2;

    return res;
}

int getRaz(int num, int raz) {
    num = num % pow10(raz + 1);
    num = num / pow10(raz);

    return num;
}

std::vector<int> RazrIteration(std::vector<int> src, int raz) {
    int size = src.size();
    std::vector<int> sorted(size);

    int counter[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // get raz count
    for (int i = 0; i < size; ++i) {
        int razr = getRaz(src[i], raz);
        counter[razr]++;
    }
    // for each raz val
    int sorted_pos = 0;
    for (int raz_val = 0; raz_val < 10; ++raz_val) {
        int src_pos = 0;  // pos in src
        if (counter[raz_val] != 0) {
            do {
                while (getRaz(src[src_pos], raz) != raz_val)
                    src_pos++;

                sorted[sorted_pos] = src[src_pos];

                sorted_pos++;
                src_pos++;
                counter[raz_val]--;
            } while (counter[raz_val]);
        }
    }

    return sorted;
}

std::vector<int> RazrSort(std::vector<int> src) {
    std::vector<int> sorted(src);
    for (int i = 0; i < 4; ++i) {
        sorted = RazrIteration(sorted, i);
    }

    return sorted;
}

std::vector<int> GetRandVector(int size) {
    std::mt19937 generator;
    std::random_device device;
    generator.seed(device());
    std::vector<int> dest(size);

    for (int i = 0; i < size; i++) {
      dest[i] = generator() % 10000;
    }

    return dest;
}

bool isSorted(std::vector<int> src) {
    int size = src.size();
    for (int i = 1; i < size; i++)
    if (src[i] < src[i - 1])
      return false;

  return true;
}

std::vector<int> Merge(std::vector<int> src1, std::vector<int> src2) {
      int size1 = src1.size();
      int size2 = src2.size();
      std::vector<int> tmp(size1 + size2);
      int i = 0, j = 0;
      for (int k = 0; k < size1+size2; k++) {
            if (i > size1-1) {
                  int a = src2[j];
                  tmp[k] = a;
                  j++;
            } else if (j > size2-1) {
                  int a = src1[i];
                  tmp[k] = a;
                  i++;
            } else if (src1[i] < src2[j]) {
                  int a = src1[i];
                  tmp[k] = a;
                  i++;
            } else {
                  int b = src2[j];
                  tmp[k] = b;
                  j++;
            }
      }

      return tmp;
}

std::vector<int> ParallSort(std::vector<int> src) {
  int mpi_rank, mpi_size;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  int length, rem;
  if (mpi_rank == 0) {
    length = src.size()/mpi_size;
    rem = src.size() % mpi_size;
  }

  MPI_Bcast(&length,
            1,
            MPI_INT,
            0,
            MPI_COMM_WORLD);

  std::vector<int> local_vec(length);

  MPI_Scatter(src.data(),
              length,
              MPI_INT,
              local_vec.data(),
              length,
              MPI_INT,
              0,
              MPI_COMM_WORLD);

  if (mpi_rank == 0 && rem != 0) {
      local_vec.insert(local_vec.end(), src.end()-rem, src.end());
  }

  local_vec = RazrSort(local_vec);

  int i = mpi_size;
  int num_op = 1;
  int num_op_length = length;
  int part;
  int loc_size = length;
  if (mpi_rank == 0)
      loc_size += rem;
  while (i > 1) {
      if (i % 2 == 1) {
          if (mpi_rank == 0) {
              MPI_Status status1;
              std::vector<int>tmp_vec(num_op_length);
              part = pow2(num_op - 1) * (i - 1);
              MPI_Recv(tmp_vec.data(),
                       num_op_length,
                       MPI_INT, part,
                       1,
                       MPI_COMM_WORLD,
                       &status1);
              local_vec = Merge(local_vec, tmp_vec);
              loc_size += num_op_length;
          }
          if (mpi_rank == pow2(num_op - 1) * (i - 1)) {
              MPI_Send(local_vec.data(),
                       num_op_length,
                       MPI_INT,
                       0,
                       1,
                       MPI_COMM_WORLD);
              return local_vec;
          }
      }

      if (mpi_rank % pow2(num_op) == 0) {
          part = mpi_rank + pow2(num_op-1);
          std::vector<int> tmp_vec(num_op_length);
          MPI_Status status3;
          MPI_Recv(tmp_vec.data(),
                   num_op_length,
                   MPI_INT,
                   part,
                   3,
                   MPI_COMM_WORLD,
                   &status3);
          local_vec = Merge(local_vec, tmp_vec);
          loc_size += num_op_length;
      }
      if (mpi_rank % pow2(num_op) != 0) {
          part = mpi_rank - pow2(num_op-1);
          MPI_Send(local_vec.data(),
                   num_op_length,
                   MPI_INT,
                   part,
                   3,
                   MPI_COMM_WORLD);
          return local_vec;
      }
      num_op++;
      i = i/2;
      num_op_length *= 2;
  }

  return local_vec;
}

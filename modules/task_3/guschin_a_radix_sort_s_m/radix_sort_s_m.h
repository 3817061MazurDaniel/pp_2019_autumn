// Copyright 2019 Guschin Alexander
#ifndef MODULES_TASK_3_GUSCHIN_A_RADIX_SORT_S_M_RADIX_SORT_S_M_H_
#define MODULES_TASK_3_GUSCHIN_A_RADIX_SORT_S_M_RADIX_SORT_S_M_H_
#include <mpi.h>
#include <ctime>
#include <random>
#include <string>
#include <vector>

int D_heap_cntr(int root, int size);

template <class T>
void Fill_random(T* vec, int size) {
  std::mt19937 gen(time(0));
  for (int i = 0; i < size; ++i) vec[i] = static_cast<T>(gen());
}

template <class T>
std::vector<T> Radix_sort(std::vector<T> st) {
  int b_len = sizeof(T);
  bool is_signed = std::is_signed<T>::value;

  std::uint16_t* test_int16 = new std::uint16_t(1);
  bool is_lit_end =
      *(reinterpret_cast<std::uint8_t*>(test_int16)) == 0 ? false : true;
  delete test_int16;
  std::uint8_t* ptr = reinterpret_cast<std::uint8_t*>(&st[0]);
  int size = st.size();
  std::vector<T> res(size);
  for (int k = 0; k < b_len; ++k) {
    int count[256] = {0};
    if (is_lit_end) {
      for (int i = 0; i < size; ++i) count[*(ptr + k + i * b_len)]++;
    } else {
      for (int i = 0; i < size; ++i)
        count[*(ptr + b_len - 1 - k + i * b_len)]++;
    }

    int shift = 0;
    if (is_signed && k == b_len - 1) {
      for (int i = 128; i < 256; ++i) shift += count[i];
    }

    int sum = 0;
    for (int i = 0; i < 256; ++i) {
      int tmp = count[i];
      count[i] = sum;
      sum += tmp;
    }

    if (is_lit_end) {
      for (int i = 0; i < size; ++i) {
        res[(count[*(ptr + k + i * b_len)] + shift) % size] = st[i];
        count[*(ptr + k + i * b_len)]++;
      }
    } else {
      for (int i = 0; i < size; ++i) {
        res[(count[*(ptr + b_len - 1 - k + i * b_len)] + shift) % size] = st[i];
        count[*(ptr + b_len - 1 - k + i * b_len)]++;
      }
    }
    st = res;
  }

  return res;
}

template <class T>
void Merge(T* res, T* vec_1, T* vec_2, int first_size, int second_size) {
  int i = 0, j = 0;
  while (i < first_size && j < second_size) {
    if (vec_1[i] < vec_2[j]) {
      res[i + j] = vec_1[i];
      ++i;
    } else {
      res[i + j] = vec_2[j];
      ++j;
    }
  }
  while (i < first_size) {
    res[i + j] = vec_1[i];
    ++i;
  }
  while (j < second_size) {
    res[i + j] = vec_2[j];
    ++j;
  }
}

template <class T>
std::vector<T> P_radix_sort(std::vector<T> st) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int b_len = sizeof(T);

  int vec_size;
  if (rank == 0) {
    vec_size = st.size();
  }
  MPI_Bcast(&vec_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int sort_size, sort_rem;
  sort_size = vec_size / size;
  sort_rem = vec_size % size;

  std::vector<T> local_vector(sort_size);
  if (rank == 0) local_vector.resize(sort_size + sort_rem);

  MPI_Scatter(reinterpret_cast<std::uint8_t*>(&st[0]) + sort_rem * b_len,
              sort_size * b_len, MPI_CHAR, &local_vector[0], sort_size * b_len,
              MPI_CHAR, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    for (int i = sort_size; i < sort_rem + sort_size; ++i)
      local_vector[i] = st[i - sort_size];
  }
  std::vector<T> sort_res(Radix_sort(local_vector));
  int self_cntr = sort_size;
  if (rank == 0) self_cntr += sort_rem;

  T* left_cntr = nullptr;
  T* rigth_cntr = nullptr;
  T* rigth_merge = nullptr;

  std::vector<T> total(
      (D_heap_cntr(rank * 2 + 2, size) + D_heap_cntr(rank * 2 + 1, size)) *
          sort_size + self_cntr);

  if (rank * 2 + 1 < size) {
    int child_w = D_heap_cntr(rank * 2 + 1, size);
    left_cntr = new T[child_w * sort_size];
    MPI_Status status;
    MPI_Recv(reinterpret_cast<std::uint8_t*>(left_cntr),
             child_w * sort_size * b_len, MPI_CHAR, rank * 2 + 1, 0,
             MPI_COMM_WORLD, &status);
  }
  if (rank * 2 + 2 < size) {
    int child_w = D_heap_cntr(rank * 2 + 2, size);
    rigth_cntr = new T[child_w * sort_size];
    MPI_Status status;
    MPI_Recv(reinterpret_cast<std::uint8_t*>(rigth_cntr),
             child_w * sort_size * b_len, MPI_CHAR, rank * 2 + 2, 0,
             MPI_COMM_WORLD, &status);
  }

  if (rank * 2 + 1 >= size && rank * 2 + 2 >= size && rank != 0) {
    MPI_Send(reinterpret_cast<std::uint8_t*>(&sort_res[0]),
             sort_res.size() * b_len,
             MPI_CHAR, (rank - 1) / 2, 0, MPI_COMM_WORLD);
  } else {
    if (rank * 2 + 2 < size) {
      rigth_merge =
          new T[D_heap_cntr(rank * 2 + 2, size) * sort_size + self_cntr];
      Merge(rigth_merge, rigth_cntr, &sort_res[0],
            D_heap_cntr(rank * 2 + 2, size) * sort_size, self_cntr);
      Merge(&total[0], rigth_merge, left_cntr,
            D_heap_cntr(rank * 2 + 2, size) * sort_size + self_cntr,
            D_heap_cntr(rank * 2 + 1, size) * sort_size);
    } else {
      Merge(&total[0], &sort_res[0], left_cntr,
            self_cntr,
            D_heap_cntr(rank * 2 + 1, size) * sort_size);
    }
    if (rank != 0) {
      MPI_Send(reinterpret_cast<std::uint8_t*>(&total[0]), total.size() * b_len,
               MPI_CHAR, (rank - 1) / 2, 0, MPI_COMM_WORLD);
    }
  }
  if (left_cntr != nullptr) delete[] left_cntr;
  if (rigth_cntr != nullptr) delete[] rigth_cntr;
  if (rigth_merge != nullptr) delete[] rigth_merge;
  return total;
}
#endif  // MODULES_TASK_3_GUSCHIN_A_RADIX_SORT_S_M_RADIX_SORT_S_M_H_

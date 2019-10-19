// Copyright 2019 Tanskii Yuriy
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <stdexcept>
#include <algorithm>
#include "../../../modules/task_1/tanskii_u_min_str/min_str.h"

std::vector<int> getRandomMatrix(int m, int n) {
  if (m <= 0 || n <= 0) {
    throw std::runtime_error("Matrix size <= 0");
  }
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  int size_m = m * n;
  std::vector<int> matrix(size_m);
  for (int i = 0; i < size_m; ++i) {
    matrix[i] = gen() % 10;
  }
  return matrix;
}
std::vector<int> getMinValInMatrix(const std::vector<int>& matrix, int m, int n) {
  if (m <= 0 || n <= 0) {
    throw std::runtime_error("Matrix size <= 0");
  }
  std::vector<int> result_vec;
  for (int i = 0; i < m; i++) {
    int first_elem_row = i * n;
    int last_elem_row = (i + 1) * n;
    int min = matrix[first_elem_row];
    for (int j = first_elem_row; j < last_elem_row; j++) {
      if (matrix[j] < min) {
        min = matrix[j];
      }
    }
    result_vec.push_back(min);
    }
  return result_vec;
}
std::vector<int> getParallelMinInMatrix(const std::vector<int>& matrix, int m, int n) {
  if (m <= 0 || n <= 0) {
    throw std::runtime_error("Matrix size <= 0");
  }
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> result_vec(m);
  std::vector<int> vec_min_val;
  int delta_lines;
  int delta_elems;
  int additional_lines;
  int additional_elems;
  if (size != 0) {
    if (static_cast<int>(m / size) <= 0) {
      if (rank == 0) {
        result_vec = getMinValInMatrix(matrix, m, n);
        return result_vec;
    } else {
        return result_vec;
        }
    } else {
      delta_lines = m / size;
      delta_elems = delta_lines * n;
      additional_lines = m % size;
      additional_elems = additional_lines * n;
      }
  }
  int lines_per_proc = delta_lines;
  int elems_per_proc = delta_elems;
  std::vector<int> proc_vec(elems_per_proc);
  if (rank == 0) {
  lines_per_proc = delta_lines + additional_lines;
  elems_per_proc = delta_elems + additional_elems;
  for (int i = 1; i < size; i++) {
    MPI_Send(&matrix[additional_elems] + delta_elems * i, delta_elems, MPI_INT, i, 0, MPI_COMM_WORLD);
  }
  proc_vec = std::vector<int>(matrix.begin(), matrix.begin() + elems_per_proc);
  } else {
    MPI_Status Status;
    MPI_Recv(&proc_vec[0], elems_per_proc, MPI_INT, 0, 0, MPI_COMM_WORLD, &Status);
    }
  vec_min_val = getMinValInMatrix(proc_vec, lines_per_proc, n);
  if (rank != 0) {
    MPI_Send(&vec_min_val[0], lines_per_proc, MPI_INT, 0, 0, MPI_COMM_WORLD);
  } else {
    result_vec = vec_min_val;
    MPI_Status Status;
    for (int i = 1; i < size; i++) {
      std::vector<int> buff(delta_lines);
      MPI_Recv(&buff[0], delta_lines, MPI_INT, i, 0, MPI_COMM_WORLD, &Status);
      result_vec.insert(result_vec.end(), buff.begin(), buff.end());
    }
  }
  return result_vec;
}


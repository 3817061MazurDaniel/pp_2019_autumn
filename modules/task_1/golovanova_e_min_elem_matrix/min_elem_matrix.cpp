// Copyright 2019 Golovanova Elena
#include <mpi.h>
#include <algorithm>
#include <random>
#include <ctime>
#include <vector>
#include <iostream>
#include "../../../modules/task_1/golovanova_e_min_elem_matrix/min_elem_matrix.h"

std::vector<int> getRandomMatrix(int n, int m) {
  if (m > 0 && n > 0) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> matrix(m * n);
    for (int i = 0; i < m * n; i++) {
      matrix[i] = gen() % 100;
      std::cout << matrix[i];
    }
    return matrix;
  } else {
    throw "Error";
  }
}

int MinElemMatrix(const std::vector<int> &matrix, int m, int n) {
  int size, rank, min, del;
  min = matrix[0];
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector <int> del_rez(size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int del_min;
  del = (n*m) / size;
  std::vector <int> del_matrix(del);
  MPI_Scatter(&matrix[0], del, MPI_INT, &del_matrix[0], del, MPI_INT, 0, MPI_COMM_WORLD);
  del_min = *std::min_element(del_matrix.begin(), del_matrix.end());
  MPI_Gather(&del_min, 1, MPI_INT, &del_rez[0], 1, MPI_INT, 0, MPI_COMM_WORLD);
  min = *std::min_element(del_rez.begin(), del_rez.end());
  if ((rank == 0)&((m*n) % size != 0)) {
    for (int i = del * size; i < m*n; i++) {
      if (matrix[i] < min)
        min = matrix[i];
    }
  }
  return min;
}

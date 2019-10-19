// Copyright 2019 Rezantsev Sergey

#include "../../../modules/task_1/rezantsev_s_min_matrix/min_matrix.h"
#include <mpi.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

std::vector<std::vector<int>> getRandMatrix(int n, int m) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<std::vector<int>> matrix(n, std::vector<int>(m));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      matrix[i][j] = gen() % 100;
    }
  }
  return matrix;
}

std::vector<int> getOrdMinOfMatrix(const std::vector<std::vector<int>> &a,
                                   int n, int m) {
  std::vector<int> x(m, 1000);
  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++) x[i] = std::min(x[i], a[j][i]);
  return x;
}

std::vector<int> getMinOfMatrix(const std::vector<std::vector<int>> &a, int n,
                                int m) {
  int size, rank;
  std::vector<int> res(m, 1000);
  std::vector<int> tmp(m, 1000);
  std::vector<int> temp(m, 1000);
  std::vector<int> str(n);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    if (m <= 0) throw std::runtime_error("m <=0");
    if (n <= 0) throw std::runtime_error("n <=0");
  }
  if (rank == 0) {
    for (int i = 0; i < m; i++) {
      if (i % size) {
        for (int j = 0; j < n; j++) str[j] = a[j][i];
        MPI_Send(&str[0], n, MPI_INT, i % size, 1, MPI_COMM_WORLD);
      }
    }
  }
  MPI_Status status;
  std::vector<int> b(n);
  if (rank == 0) {
    for (int i = 0; i < m; i += size) {
      for (int j = 0; j < n; j++) str[j] = a[j][i];
      temp[i] = std::min(*min_element(str.begin(), str.end()), temp[i]);
    }
  } else {
    if (rank < m) {
      for (int i = rank; i < m; i += size) {
        MPI_Recv(&b[0], n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        res[i] = std::min(res[i], *min_element(b.begin(), b.end()));
      }
      MPI_Send(&res[0], m, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }
  }
  if ((rank == 0) && (rank < m) && (m > 1)) {
    for (int proc = 1; proc < std::min(size, m); proc++) {
      MPI_Recv(&res[0], m, MPI_INT, proc, 2, MPI_COMM_WORLD, &status);
      for (int j = proc; j < m; j += size) {
        tmp[j] = res[j];
      }
    }
  }
  if (rank == 0) {
    for (int i = 0; i < m; i++) res[i] = std::min(temp[i], tmp[i]);
  }
  return res;
}

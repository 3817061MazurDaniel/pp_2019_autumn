// Copyright 2019 SIlenko Dmitrii

#include <mpi.h>
#include <iostream>
#include <random>
#include <ctime>
#include <numeric>
#include <vector>
#include <stdexcept>
#include "../../../modules/task_1/silenko_d_column_sum_matrix/column_sum_matrix.h"

std::vector<int> getRandomMatrixE(const int n, const int m) {
  if (n <= 0) {
    throw "Wrong rows";
  } else if (m <= 0) {
    throw "wrong columns";
  }
  std::vector <int> Matrix(n * m);
  for (int i = 0; i < n*m; i++) {
    Matrix[i] = 1;
  }
  return Matrix;
}

std::vector<int> getRandomMatrixO(const int n, const int m) {
  if (n <= 0) {
    throw "Wrong rows";
  } else if (m <= 0) {
    throw "wrong columns";
  }
  std::vector <int> Matrix(n * m);
  int j = 1;
  int k = 0;
  for (int i = 0; i < n*m; i++) {
    if ((j - 1) % (n) == 0) {
      k++;
    }
    Matrix[i] = k;
    j++;
  }
  return Matrix;
}

std::vector <int> ColumnSumMatrix(const std::vector <int> &a, int n, int m) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;
  int errors;
  const int delta = m / size;
  const int ost = m % size;

  if (rank == 0) {
    if (a.size() != (size_t)n * (size_t)m) {
      errors = -1;
    } else if (n <= 0) {
      errors = -2;
    } else if (m <= 0) {
      errors = -3;
    } else {
      errors = 0;
    }
    for (int i = 1; i < size; i++)
      MPI_Send(&errors, 1, MPI_INT, i, 8, MPI_COMM_WORLD);
  } else {
    MPI_Recv(&errors, 1, MPI_INT, 0, 8, MPI_COMM_WORLD, &status);
  }

  switch (errors) {
  case 0:
    break;
  case -1:
    throw std::runtime_error("Size doesn't match description");
  case -2:
    throw std::runtime_error("Number of rows 0");
  case -3:
    throw std::runtime_error("Nubmer of columns 0");
  }

  if ((rank == 0) && (delta > 0)) {
    for (int i = 1; i < size; i++) {
        MPI_Send(&a[n * ost] + i * n * delta, n * delta, MPI_INT, i, 5, MPI_COMM_WORLD);
    }
  }

  std::vector <int> b(delta * n, 0);
  if (rank == 0) {
    b = std::vector <int>(a.begin(), a.begin() + ost * n + delta * n);
  } else {
    if (delta > 0) {
      MPI_Recv(&b[0], delta * n, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);
    }
  }

  int vec = b.size() / n;
  std::vector<int> sum(vec, 0);
  for (int i = 0; i < vec; i++)
    for (int j = i * n; j < (i + 1) * n; j++)
      sum[i] += b[j];

  std::vector<int> ans(m, 0);
  if (rank == 0) {
    for (int i = 0; i < vec; i++) {
      ans[i] = sum[i];
    }
    if (delta > 0) {
      for (int i = 1; i < size; i++) {
        MPI_Recv(&ans[vec] + (i - 1) * delta, delta, MPI_INT, i, 9, MPI_COMM_WORLD, &status);
      }
    }
  } else {
    if (delta > 0)
      MPI_Send(&sum[0], delta, MPI_INT, 0, 9, MPI_COMM_WORLD);
  }

  return ans;
}

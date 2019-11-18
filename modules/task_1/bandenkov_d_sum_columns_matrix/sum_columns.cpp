// Copyright 2019 Bandenkov Daniil
#include <mpi.h>
#include <vector>
#include <iostream>
#include <random>
#include <ctime>
#include <numeric>
#include <stdexcept>
#include "../../../modules/task_1/bandenkov_d_sum_columns_matrix/sum_columns.h"

std::vector<int> getRandomMatrix(const int rows, const int cols) {
  if (rows < 1 || cols < 1)
    throw - 1;
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> Matrix(rows * cols);
  for (int i = 0; i < rows * cols; i++)
    Matrix[i] = gen() % 100;
  return Matrix;
}

std::vector<int> getSequintialSum(const std::vector<int> Matrix, int rows, int cols) {
  std::vector<int> result(cols);
  int sum;
  for (int i = 0; i < cols; i++) {
    sum = 0;
    for (int j = 0; j < rows; j++) {
      sum += Matrix[rows * i + j];
    }
    result[i] = sum;
  }
  return result;
}

std::vector<int> getParallelSum(const std::vector<int> MatA, int rows, int cols) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int delta = cols / size;
  const int rem = cols % size;

  if ((MatA.size() != (size_t)rows * (size_t)cols) || (rows <= 0) || (cols <= 0)) {
    throw - 1;
  }

  if (rows == 1)
    return MatA;

  std::vector <int> MatB(delta * rows, 0);

  if (delta > 0) {
    if (rank == 0) {
      for (int i = 1; i < size; i++) {
        MPI_Send(&MatA[rows * (rem + i * delta)], rows * delta, MPI_INT, i, 1, MPI_COMM_WORLD);
      }
    } else {
      MPI_Status status;
      MPI_Recv(&MatB[0], delta * rows, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }
  }
  std::vector<int> res;
  if (rank == 0) {
    MPI_Status status;
    int sum = 0;
    std::vector<int> elem(rows);
    MatB = std::vector<int>(MatA.begin(), MatA.begin() + rows * (rem + delta));

    for (int i = 0; i < delta + rem; i++) {
      for (int j = 0; j < rows; j++) {
        sum += MatB[i * rows + j];
      }
      res.push_back(sum);
      sum = 0;
    }

    if (delta > 0) {
      for (int i = 1; i < size; i++) {
        MPI_Recv(&elem[0], delta, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
        res.insert(res.end(), elem.begin(), elem.begin() + delta);
      }
    }
  } else if (delta > 0) {
    int sum = 0;
    for (int i = 0; i < delta; i++) {
      for (int j = 0; j < rows; j++) {
        sum += MatB[i * rows + j];
      }
      res.push_back(sum);
      sum = 0;
    }
    MPI_Send(&res[0], delta, MPI_INT, 0, 2, MPI_COMM_WORLD);
  }
  return res;
}

// Copyright 2019 Myshkin Andrew
#include <mpi.h>
#include <random>
#include <vector>
#include <stdexcept>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/myshkin_a_min_matrix/min_matrix.h"


std::vector<int> getConsistentMatrix(int m, int n) {
  if (m <= 0 || n <= 0)
    throw std::runtime_error("Error Matrix size");
  std::vector<int> matrix(m * n);
  for (int i = 0; i < n*m; i++) {
    matrix[i] = i;
  }
  return matrix;
}



std::vector<int> getRandomMatrix(int m, int n) {
  if (m <= 0 || n <= 0)
    throw std::runtime_error("Error Matrix size");
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> matrix(m * n);
  for (int i = 0; i < n*m; i++)
    matrix[i] = gen() % 100;
  return matrix;
}


int getParallelMinMatrix(const std::vector<int> miv, int rows, int cols) {
  int size, rank;
  int result = 0;
  int min;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int sizem = rows * cols;
  const int delta = sizem / size;
  const int remainder = sizem % size;

  std::vector<int> buf1(delta);

  if (sizem < size) {
    if (rank == 0) {
      result = *std::min_element(miv.begin(), miv.end());
      return result;
    } else {
      return result;
    }
  }


  if (rank == 0) {
    if (static_cast<int>(miv.size()) != rows * cols) {
      int error = -1;
      for (int proc = 1; proc < size; proc++)
        MPI_Send(&error, 1, MPI_INT, proc, -1, MPI_COMM_WORLD);
      throw std::runtime_error("Matrix dimensions are incorrect");
    }
  }


  if (rank == 0) {
    for (int proc = 1; proc < size; proc++) {
      MPI_Send(&miv[0] + proc * delta + remainder, delta, MPI_INT, proc, 0, MPI_COMM_WORLD);
    }
    std::vector<int> miv_L = std::vector<int>(miv.begin(), miv.begin() + delta + remainder);
    min = *std::min_element(miv_L.begin(), miv_L.end());
  } else {
    MPI_Status Status;
    MPI_Recv(&buf1[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &Status);
    min = *std::min_element(buf1.begin(), buf1.end());
  }

  MPI_Reduce(&min, &result, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
  return result;
}

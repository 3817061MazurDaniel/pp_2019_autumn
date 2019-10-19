// Copyright 2019 Mityagina Daria
#include "../../../modules/task_1/mityagina_d_sum_of_matrix_elements/sum_of_matrix_elements.h"
#include <mpi.h>
#include <math.h>
#include <ctime>
#include <vector>
#include <random>
#include <stdexcept>

int Work(int size, std::vector<int> matrix) {
  int sum_res = 0;
  int rank, p_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p_size);
  const int calculate_part = size / p_size;
  const int dop = size % p_size;
  int error;
  MPI_Status status;

  if (rank == 0) {
    if (size <= 0) {
      error = 1;
    } else {
      error = 0;
    }
    for (int proc = 1; proc < p_size; ++proc)
      MPI_Send(&error, 1, MPI_INT, proc, 5, MPI_COMM_WORLD);
  } else {
    MPI_Recv(&error, 1, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);
  }
  switch (error) {
  case 0:
    break;
  case 1:
    throw std::runtime_error("size <= 0");
  }
  std::vector<int> recieved = std::vector<int>(calculate_part, 0);

  if (rank == 0) {
    if (calculate_part != 0) {
      for (int proc = 1; proc < p_size; proc++)
        MPI_Send(&matrix[dop] + proc * calculate_part, calculate_part, MPI_INT, proc, 0, MPI_COMM_WORLD);
    }
  }
  if (rank == 0) {
    recieved.resize(calculate_part + dop);
    recieved = std::vector<int>(matrix.begin(), matrix.begin() + calculate_part + dop);
  } else if (calculate_part != 0) {
      MPI_Recv(&recieved[0], calculate_part, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }
  int part_sum = SumOfMatrixElementsPartly(recieved);
  MPI_Barrier(MPI_COMM_WORLD);
  if (calculate_part != 0)
    MPI_Reduce(&part_sum, &sum_res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  else
    sum_res = part_sum;

  return sum_res;
}

int SumOfMatrixElementsPartly(std::vector<int> matrix) {
  int sum = 0;
  int size = matrix.size();
  for (int i = 0; i < size; i++)
    sum += matrix[i];
  return sum;
}

std::vector<int> getMatrix(int size) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> new_matrix(size);
  for (int i = 0; i < size; ++i) {
    new_matrix[i] = gen() % 100;
  }
  return new_matrix;
}

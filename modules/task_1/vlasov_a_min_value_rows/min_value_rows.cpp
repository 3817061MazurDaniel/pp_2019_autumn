// Copyright 2019 Vlasov Andrey
#include <mpi.h>
#include <random>
#include <ctime>
#include <vector>
#include "../../../modules/task_1/vlasov_a_min_value_rows/min_value_rows.h"

std::vector<int> getRandomMatrix(int m, int n) {
  if (m > 0 && n > 0) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> matrix(m * n);
    for (int i = 0; i < m * n; i++)
      matrix[i] = gen() % 100;
    return matrix;
  } else {
      throw - 1;
  }
}

std::vector<int> MinValuesRowsSequential(const std::vector<int> &matrix, int m, int n) {
  std::vector<int> rez(m);
  for (int i = 0; i < m; i++) {
    rez[i] = matrix[i*n];
    for (int j = 1; j < n; j++)
      if (matrix[i*n + j] < rez[i])
        rez[i] = matrix[i*n + j];
  }
  return rez;
}

std::vector<int> MinValuesRowsParallel(const std::vector<int> &matrix, int m, int n) {
  int size, rank, local_m;
  std::vector<int> rez(m);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  local_m = m / size;
  std::vector<int> local_rez(local_m);
  std::vector<int> local_matrix(local_m * n);
  MPI_Scatter(&matrix[0], local_m * n, MPI_INT, &local_matrix[0], local_m * n, MPI_INT, 0, MPI_COMM_WORLD);
  for (int i = 0; i < local_m; i++) {
    local_rez[i] = local_matrix[i*n];
    for (int j = 1; j < n; j++) {
      if (local_matrix[i*n + j] < local_rez[i]) {
        local_rez[i] = local_matrix[i*n + j];
      }
    }
  }
  MPI_Gather(&local_rez[0], local_m, MPI_INT, &rez[0], local_m, MPI_INT, 0, MPI_COMM_WORLD);
  if ((rank == 0) & (m % size != 0)) {
    for (int i = local_m * size; i < m; i++) {
      rez[i] = matrix[i*n];
      for (int j = 1; j < n; j++)
        if (matrix[i*n + j] < rez[i])
          rez[i] = matrix[i*n + j];
    }
  }
  return rez;
}

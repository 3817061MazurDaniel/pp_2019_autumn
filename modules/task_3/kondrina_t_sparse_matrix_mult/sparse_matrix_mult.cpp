// Copyright 2019 Kondrina Tatyana

#include "../../../modules/task_3/kondrina_t_sparse_matrix_mult/sparse_matrix_mult.h"

#include <mpi.h>

#include <ctime>
#include <random>
#include <vector>

SparseMatrix CCS(const std::vector<double> new_matrix, const int new_columns,
                 const int new_rows) {
  SparseMatrix result;
  result.columns = new_columns;
  result.rows = new_rows;
  result.nnz = 0;

  result.JA.push_back(0);
  for (auto column = 0; column < new_columns; column++) {
    int nnz_count = 0;
    for (auto i = column; i <= (new_rows - 1) * new_columns + column;
         i += new_columns) {
      if (new_matrix[i] != 0) {
        nnz_count++;
        result.val.push_back(new_matrix[i]);
        result.IA.push_back((i - column) / new_columns);
      }
    }
    result.JA.push_back(result.JA.back() + nnz_count);
    result.nnz += nnz_count;
  }

  return result;
}

const std::vector<double> operator*(const SparseMatrix& A,
                                    const SparseMatrix& B) {
  if (A.columns != B.rows) {
    throw "incorrect size";
  }

  std::vector<double> result(A.rows * B.columns, 0);

  for (auto column = 0; column < A.columns; column++) {
    for (auto b_column = 0; b_column < B.columns; b_column++) {
      for (auto i = A.JA[column]; i <= A.JA[column + 1] - 1; i++) {
        if (B.JA[b_column + 1] - B.JA[b_column] == 0) {
          continue;
        }

        for (auto j = B.JA[b_column]; j <= B.JA[b_column + 1] - 1; j++) {
          if (B.IA[j] == column) {
            result[A.IA[i] * B.columns + b_column] += A.val[i] * B.val[j];
          }
        }
      }
    }
  }

  return result;
}

std::vector<double> multiply(SparseMatrix A, SparseMatrix B) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size == 1) {
    return A * B;
  }

  MPI_Bcast(&A.columns, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&A.rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&A.nnz, 1, MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Bcast(&B.columns, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&B.rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&B.nnz, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (A.nnz == 0 || B.nnz == 0) {
    if (rank == 0) {
      return A * B;
    } else {
      return std::vector<double>();
    }
  }

  if (A.columns != B.rows) {
    throw "incorrect size";
  }

  if (A.columns < size) {
    if (rank == 0) {
      return A * B;
    } else {
      return std::vector<double>();
    }
  }

  if (rank != 0) {
    A.val.resize(A.nnz);
    A.IA.resize(A.nnz);
    A.JA.resize(A.columns + 1);
  }
  MPI_Bcast(&A.val[0], A.nnz, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&A.IA[0], A.nnz, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&A.JA[0], A.columns + 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank != 0) {
    B.val.resize(B.nnz);
    B.IA.resize(B.nnz);
    B.JA.resize(B.columns + 1);
  }
  MPI_Bcast(&B.val[0], B.nnz, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&B.IA[0], B.nnz, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&B.JA[0], B.columns + 1, MPI_INT, 0, MPI_COMM_WORLD);

  int delta = A.columns / size;
  int left_bound = rank * delta;
  int right_bound = (rank + 1) * delta;

  if (rank == size - 1) {
    right_bound = A.columns;
  }

  std::vector<double> local_result(A.rows * B.columns);

  for (auto column = left_bound; column < right_bound; column++) {
    for (auto b_column = 0; b_column < B.columns; b_column++) {
      for (auto i = A.JA[column]; i <= A.JA[column + 1] - 1; i++) {
        if (B.JA[b_column + 1] - B.JA[b_column] == 0) {
          continue;
        }

        for (auto j = B.JA[b_column]; j <= B.JA[b_column + 1] - 1; j++) {
          if (B.IA[j] == column) {
            local_result[A.IA[i] * B.columns + b_column] += A.val[i] * B.val[j];
          }
        }
      }
    }
  }

  std::vector<double> global_result;
  if (rank == 0) {
    global_result.resize(A.rows * B.columns);
  }

  if (rank == 0) {
    MPI_Reduce(&local_result[0], &global_result[0], A.rows * B.columns,
               MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  } else {
    MPI_Reduce(&local_result[0], MPI_IN_PLACE, A.rows * B.columns, MPI_DOUBLE,
               MPI_SUM, 0, MPI_COMM_WORLD);
  }

  return global_result;
}

std::vector<double> randMatrix(const int columns, const int rows) {
  if (rows <= 0 || columns <= 0) {
    throw "incorrect size";
  }

  std::srand(std::time(nullptr));
  std::vector<double> result(columns * rows);

  for (auto i = 0; i < rows * columns; i++) {
    double rand_val = static_cast<double>(std::rand() % 50 + 1);
    rand_val < 4 ? result[i] = rand_val : result[i] = 0;
  }
  return result;
}

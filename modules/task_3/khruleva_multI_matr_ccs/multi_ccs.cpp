// Copyright 2019 Khruleva Anastasia

#include <mpi.h>
#include <vector>
#include <iostream>
#include <random>
#include <ctime>
#include "../../../modules/task_3/khruleva_multI_matr_ccs/multi_ccs.h"

MatrixCCS CCS(const std::vector<double> Mart_N, const int Col_N,
  const int Rows_N) {
  MatrixCCS All;
  All.Col = Col_N;
  All.Rows = Rows_N;
  All.xxx = 0;

  All.JA.push_back(0);
  for (auto col = 0; col < Col_N; col++) {
    int xxx_count = 0;
    for (auto i = col; i <= (Rows_N - 1) * Col_N + col;
      i += Col_N) {
      if (Mart_N[i] != 0) {
        xxx_count++;
        All.val.push_back(Mart_N[i]);
        All.IA.push_back((i - col) / Col_N);
      }
    }
    All.JA.push_back(All.JA.back() + xxx_count);
    All.xxx += xxx_count;
  }

  return All;
}

const std::vector<double> operator*(const MatrixCCS& X,
  const MatrixCCS& Y) {
  if (X.Col != Y.Rows) {
    throw "invalid size";
  }

  std::vector<double> All(X.Rows * Y.Col, 0);

  for (auto col = 0; col < X.Col; col++) {
    for (auto y_col = 0; y_col < Y.Col; y_col++) {
      for (auto i = X.JA[col]; i <= X.JA[col + 1] - 1; i++) {
        if (Y.JA[y_col + 1] - Y.JA[y_col] == 0) {
          continue;
        }

        for (auto j = Y.JA[y_col]; j <= Y.JA[y_col + 1] - 1; j++) {
          if (Y.IA[j] == col) {
            All[X.IA[i] * Y.Col + y_col] += X.val[i] * Y.val[j];
          }
        }
      }
    }
  }

  return All;
}

std::vector<double> multiplication(MatrixCCS X, MatrixCCS Y) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size == 1) {
    return X * Y;
  }

  MPI_Bcast(&X.Col, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&X.Rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&X.xxx, 1, MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Bcast(&Y.Col, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&Y.Rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&Y.xxx, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (X.xxx == 0 || Y.xxx == 0) {
    if (rank == 0) {
      return X * Y;
    } else {
      return std::vector<double>();
    }
  }

  if (X.Col != Y.Rows) {
    throw "invalid size";
  }

  if (X.Col < size) {
    if (rank == 0) {
      return X * Y;
    } else {
      return std::vector<double>();
    }
  }

  if (rank != 0) {
    X.val.resize(X.xxx);
    X.IA.resize(X.xxx);
    X.JA.resize(X.Col + 1);
  }
  MPI_Bcast(&X.val[0], X.xxx, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&X.IA[0], X.xxx, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&X.JA[0], X.Col + 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank != 0) {
    Y.val.resize(Y.xxx);
    Y.IA.resize(Y.xxx);
    Y.JA.resize(Y.Col + 1);
  }
  MPI_Bcast(&Y.val[0], Y.xxx, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&Y.IA[0], Y.xxx, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&Y.JA[0], Y.Col + 1, MPI_INT, 0, MPI_COMM_WORLD);

  int delta = X.Col / size;
  int left_bound = rank * delta;
  int right_bound = (rank + 1) * delta;

  if (rank == size - 1) {
    right_bound = X.Col;
  }

  std::vector<double> local_result(X.Rows * Y.Col);

  for (auto col = left_bound; col < right_bound; col++) {
    for (auto y_col = 0; y_col < Y.Col; y_col++) {
      for (auto i = X.JA[col]; i <= X.JA[col + 1] - 1; i++) {
        if (Y.JA[y_col + 1] - Y.JA[y_col] == 0) {
          continue;
        }

        for (auto j = Y.JA[y_col]; j <= Y.JA[y_col + 1] - 1; j++) {
          if (Y.IA[j] == col) {
            local_result[X.IA[i] * Y.Col + y_col] += X.val[i] * Y.val[j];
          }
        }
      }
    }
  }

  std::vector<double> global_result;
  if (rank == 0) {
    global_result.resize(X.Rows * Y.Col);
  }

  if (rank == 0) {
    MPI_Reduce(&local_result[0], &global_result[0], X.Rows * Y.Col,
      MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  } else {
    MPI_Reduce(&local_result[0], MPI_IN_PLACE, X.Rows * Y.Col, MPI_DOUBLE,
      MPI_SUM, 0, MPI_COMM_WORLD);
  }

  return global_result;
}

std::vector<double> randomMatrix(const int Col, const int Rows) {
  if (Rows <= 0 || Col <= 0) {
    throw "invalid size";
  }

  std::srand(std::time(nullptr));
  std::vector<double> All(Col * Rows);

  for (auto i = 0; i < Rows * Col; i++) {
    double rand_val = static_cast<double>(std::rand() % 50 + 1);
    rand_val < 4 ? All[i] = rand_val : All[i] = 0;
  }

  return All;
}

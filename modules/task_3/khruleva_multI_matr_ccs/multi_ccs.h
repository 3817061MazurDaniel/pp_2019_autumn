// Copyright 2019 Khruleva Anastasia
#ifndef MODULES_TASK_3_KHRULEVA_MULTI_MATR_CCS_MULTI_CCS_H_
#define MODULES_TASK_3_KHRULEVA_MULTI_MATR_CCS_MULTI_CCS_H_
#include <mpi.h>
#include <vector>
struct MatrixCCS {
  std::vector<double> val;
  std::vector<int> JA;
  std::vector<int> IA;
  int Col, Rows, xxx;

  friend const std::vector<double> operator*(const MatrixCCS& X,
    const MatrixCCS& Y);
};

MatrixCCS CCS(const std::vector<double> Mart_N,
  const int Col_N, const int Rows_N);

std::vector<double> multiplication(MatrixCCS X, MatrixCCS Y);
std::vector<double> randomMatrix(const int col, const int rows);

#endif  // MODULES_TASK_3_KHRULEVA_MULTI_MATR_CCS_MULTI_CCS_H_

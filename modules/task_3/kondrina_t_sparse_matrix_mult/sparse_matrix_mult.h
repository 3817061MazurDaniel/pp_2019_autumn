// Copyright 2019 Kondrina Tatyana

#ifndef MODULES_TASK_3_KONDRINA_T_SPARSE_MATRIX_MULT_SPARSE_MATRIX_MULT_H_
#define MODULES_TASK_3_KONDRINA_T_SPARSE_MATRIX_MULT_SPARSE_MATRIX_MULT_H_

#include <vector>

struct SparseMatrix {
  std::vector<double> val;
  std::vector<int> JA;
  std::vector<int> IA;
  int columns, rows, nnz;

  friend const std::vector<double> operator*(const SparseMatrix& A,
                                             const SparseMatrix& B);
};

SparseMatrix CCS(const std::vector<double> new_matrix,
                          const int new_columns, const int new_rows);

std::vector<double> multiply(SparseMatrix A, SparseMatrix B);
std::vector<double> randMatrix(const int columns, const int rows);

#endif  // MODULES_TASK_3_KONDRINA_T_SPARSE_MATRIX_MULT_SPARSE_MATRIX_MULT_H_

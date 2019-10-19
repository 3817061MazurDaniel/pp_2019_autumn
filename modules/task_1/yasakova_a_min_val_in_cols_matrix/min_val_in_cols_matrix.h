// Copyright 2019 Yasakova Anastasia
#ifndef MODULES_TASK_1_YASAKOVA_A_MIN_VAL_IN_COLS_MATRIX_MIN_VAL_IN_COLS_MATRIX_H_
#define MODULES_TASK_1_YASAKOVA_A_MIN_VAL_IN_COLS_MATRIX_MIN_VAL_IN_COLS_MATRIX_H_

#include <vector>

std::vector <int> getRandomMatrix(int cols, int rows);
std::vector <int> ParallelMinInColsMatrix(const std::vector <int>& a, int rows, int cols);
std::vector <int> TransposeMatrix(const std::vector <int>& a, int rows, int cols);
std::vector <int> SequentialMinInColsMatrix(const std::vector <int>& a, int rows, int cols);

#endif  // MODULES_TASK_1_YASAKOVA_A_MIN_VAL_IN_COLS_MATRIX_MIN_VAL_IN_COLS_MATRIX_H_

// Copyright 2019 Korobeinikov Aleksei
#ifndef MODULES_TASK_1_KOROBEINIKOV_A_MAX_IN_MATRIX_MAX_IN_MATRIX_H_
#define MODULES_TASK_1_KOROBEINIKOV_A_MAX_IN_MATRIX_MAX_IN_MATRIX_H_

#include <vector>

std::vector <int> GetMatrix(int rows, int column);

std::vector <int> TranspositionOfMatrix(std::vector <int> vec, int rows);

std::vector <int> GetMaxValueInColumnOfMatrix(std::vector<int> matrix,
    int rows, int columns);

std::vector <int> CheckResult(std::vector<int> matrix, int rows, int columns);

#endif  // MODULES_TASK_1_KOROBEINIKOV_A_MAX_IN_MATRIX_MAX_IN_MATRIX_H_

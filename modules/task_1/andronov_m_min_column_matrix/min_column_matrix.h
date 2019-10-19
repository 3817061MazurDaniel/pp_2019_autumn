// Copyright 2019 Andronov Maxim
#ifndef MODULES_TASK_1_ANDRONOV_M_MIN_COLUMN_MATRIX_MIN_COLUMN_MATRIX_H_
#define MODULES_TASK_1_ANDRONOV_M_MIN_COLUMN_MATRIX_MIN_COLUMN_MATRIX_H_

#include <vector>

std::vector <int> GetRandomMatrix(int rows, int column);

std::vector <int> GetTransposeMatrix(std::vector <int> Matrix, int rows, int columns);

std::vector <int> GetSequentialMinValueColumn(std::vector <int> Matrix, int rows, int columns);

std::vector <int> GetParallelMinValueColumn(std::vector <int> Matrix, int rows, int columns);

#endif  // MODULES_TASK_1_ANDRONOV_M_MIN_COLUMN_MATRIX_MIN_COLUMN_MATRIX_H_


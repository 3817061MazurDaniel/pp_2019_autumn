// Copyright 2019 Okmyanskiy Andrey
#ifndef MODULES_TASK_1_OKMYANSKIY_A_SUM_VAL_IN_ROWS_SUM_VAL_IN_ROWS_H_
#define MODULES_TASK_1_OKMYANSKIY_A_SUM_VAL_IN_ROWS_SUM_VAL_IN_ROWS_H_

#include <mpi.h>
#include <vector>
#include <ctime>

std::vector<int> getRandomMatrix(int m, int n);
std::vector<int> getParallelSumValInRows(const std::vector<int> matr, int rows, int cols);
std::vector<int> getSequintialSumValInRows(const std::vector<int> matr, int rows, int cols);

#endif  // MODULES_TASK_1_OKMYANSKIY_A_SUM_VAL_IN_ROWS_SUM_VAL_IN_ROWS_H_

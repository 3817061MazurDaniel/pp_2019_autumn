// Copyright 2019 Khruleva Anastasia
#ifndef MODULES_TASK_1_KHRULEVA_A_ROWS_MAX_ROWS_MAX_MPI_H_
#define MODULES_TASK_1_KHRULEVA_A_ROWS_MAX_ROWS_MAX_MPI_H_

#include <vector>

std::vector<int> getMatr(const int rows, const int cols);
std::vector<int> getMaxMatr(const std::vector<int>& matr, int rows, int cols);
std::vector<int> getParallelMatr(const std::vector<int>& matr, int rows, int cols);

#endif  // MODULES_TASK_1_KHRULEVA_A_ROWS_MAX_ROWS_MAX_MPI_H_



// Copyright 2019 Zhivaev Artem
#ifndef MODULES_TASK_1_ZHIVAEV_A_SUM_COLS_SUM_COLS_H_
#define MODULES_TASK_1_ZHIVAEV_A_SUM_COLS_SUM_COLS_H_

#include <vector>

std::vector<int> getRandomMatrix(int rows, int cols);
std::vector<int> sumColumnsOneProc(const int* matrix, int rows, int cols);
std::vector<int> sumColumns(int* matrix, int rows, int cols);

#endif  // MODULES_TASK_1_ZHIVAEV_A_SUM_COLS_SUM_COLS_H_


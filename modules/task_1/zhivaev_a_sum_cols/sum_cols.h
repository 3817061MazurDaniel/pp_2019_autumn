// Copyright 2019 Zhivaev Artem
#ifndef MODULES_TASK_1_ZHIVAEV_A_SUM_COLS_SUM_COLS_H_
#define MODULES_TASK_1_ZHIVAEV_A_SUM_COLS_SUM_COLS_H_

#include <vector>

std::vector<std::vector<int>> getRandomMatrix(const int rows,
                                              const int cols);
std::vector<int> summColumnsOneProc(std::vector<std::vector<int>>);
std::vector<int> summColumns(std::vector<std::vector<int>>);

#endif  // MODULES_TASK_1_ZHIVAEV_A_SUM_COLS_SUM_COLS_H_


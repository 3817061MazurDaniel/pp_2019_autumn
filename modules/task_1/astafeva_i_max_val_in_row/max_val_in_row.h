// Copyright 2019 Astafeva Irina

#ifndef MODULES_TASK_1_ASTAFEVA_I_MAX_VAL_IN_ROW_MAX_VAL_IN_ROW_H_
#define MODULES_TASK_1_ASTAFEVA_I_MAX_VAL_IN_ROW_MAX_VAL_IN_ROW_H_

#include <vector>

std::vector<int> getRandomMatrix(int rows, int columns);

std::vector<int> getMaxInRows(const std::vector<int> matrix, int rows, int columns);

std::vector<int> getMaxInRowsPar(const std::vector<int> matrix, int rows, int columns);

#endif  // MODULES_TASK_1_ASTAFEVA_I_MAX_VAL_IN_ROW_MAX_VAL_IN_ROW_H_

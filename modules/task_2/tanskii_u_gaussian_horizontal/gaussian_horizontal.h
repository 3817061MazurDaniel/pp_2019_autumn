// Copyright 2019 Tanskii Yuriy
#ifndef MODULES_TASK_2_TANSKII_U_GAUSSIAN_HORIZONTAL_GAUSSIAN_HORIZONTAL_H_
#define MODULES_TASK_2_TANSKII_U_GAUSSIAN_HORIZONTAL_GAUSSIAN_HORIZONTAL_H_

#include <vector>

std::vector<int> getRandomMatrix(const int rows, const int cols);
std::vector<double> gaussianSequential(const std::vector<double>& vec, int rows, int cols);
std::vector<double> gaussianParallel(const std::vector<double>& vec, int rows, int cols);

#endif  // MODULES_TASK_2_TANSKII_U_GAUSSIAN_HORIZONTAL_GAUSSIAN_HORIZONTAL_H_

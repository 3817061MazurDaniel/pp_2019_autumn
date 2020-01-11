// Copyright 2019 Obolenskiy Arseniy
#ifndef MODULES_TASK_2_OBOLENSKIY_A_GAUSSIAN_ELIMINATION_VERTICAL_GAUSSIAN_ELIMINATION_H_
#define MODULES_TASK_2_OBOLENSKIY_A_GAUSSIAN_ELIMINATION_VERTICAL_GAUSSIAN_ELIMINATION_H_
#include <vector>

std::vector <double> getRandomMatrix(int rows, int cols, double min_value, double max_value);
std::vector <double> solveSequential(const std::vector <double> &a, size_t rows, size_t cols);
std::vector <double> solveParallel(const std::vector <double> &a, size_t rows, size_t cols);

#endif  // MODULES_TASK_2_OBOLENSKIY_A_GAUSSIAN_ELIMINATION_VERTICAL_GAUSSIAN_ELIMINATION_H_

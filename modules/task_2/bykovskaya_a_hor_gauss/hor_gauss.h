// Copyright 2019 Bykovskaya Alina
#ifndef MODULES_TASK_2_BYKOVSKAYA_A_HOR_GAUSS_HOR_GAUSS_H_
#define MODULES_TASK_2_BYKOVSKAYA_A_HOR_GAUSS_HOR_GAUSS_H_
#include <vector>

std::vector<double> getRandomMatrix(int rows, int cols, double min, double max);
std::vector<double> parallelGauss(const std::vector<double> &a, int rows, int cols);
std::vector<double> Gauss(const std::vector<double> &b, int rows, int cols);

#endif  // MODULES_TASK_2_BYKOVSKAYA_A_HOR_GAUSS_HOR_GAUSS_H_

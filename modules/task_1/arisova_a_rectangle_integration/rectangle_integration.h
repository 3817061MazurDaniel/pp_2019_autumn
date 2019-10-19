// Copyright 2019 Arisova Anasrasiia
#ifndef MODULES_TASK_1_ARISOVA_A_RECTANGLE_INTEGRATION_RECTANGLE_INTEGRATION_H_
#define MODULES_TASK_1_ARISOVA_A_RECTANGLE_INTEGRATION_RECTANGLE_INTEGRATION_H_

#include <vector>

double getParallelIntegration(std::vector<double> vec, double(*F)(double), double a, double b, int N);
double getSequentialIntegration(std::vector<double> vec, double(*F)(double), double l_h);

#endif  // MODULES_TASK_1_ARISOVA_A_RECTANGLE_INTEGRATION_RECTANGLE_INTEGRATION_H_

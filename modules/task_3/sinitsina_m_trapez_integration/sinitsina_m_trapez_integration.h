// Copyright 2019 Sinitsina Maria
#ifndef MODULES_TASK_3_SINITSINA_M_TRAPEZ_INTEGRATION_SINITSINA_M_TRAPEZ_INTEGRATION_H_
#define MODULES_TASK_3_SINITSINA_M_TRAPEZ_INTEGRATION_SINITSINA_M_TRAPEZ_INTEGRATION_H_

#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

double ParallelIntegr(double(*func)(std::vector<double>), std::vector <double> x,
    std::vector <double> y, const int n);
double SequentialIntegr(double(*func)(std::vector<double>), std::vector <double> x,
    std::vector <double> y, const int n);

#endif  // MODULES_TASK_3_SINITSINA_M_TRAPEZ_INTEGRATION_SINITSINA_M_TRAPEZ_INTEGRATION_H_

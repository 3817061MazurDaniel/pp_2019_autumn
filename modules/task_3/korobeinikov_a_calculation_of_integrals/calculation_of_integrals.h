// Copyright 2019 Korobeinikov Aleksei
#ifndef MODULES_TASK_3_KOROBEINIKOV_A_CALCULATION_OF_INTEGRALS_CALCULATION_OF_INTEGRALS_H_
#define MODULES_TASK_3_KOROBEINIKOV_A_CALCULATION_OF_INTEGRALS_CALCULATION_OF_INTEGRALS_H_
#include <vector>
#include <algorithm>
#include <utility>

double ParallelVersion(double (*func)(std::vector<double>), std::vector <std::pair<double, double>> v,
    std::vector <int> distr);
double SequentialVersion(double (*func)(std::vector<double>), std::vector <std::pair<double, double>> v,
    std::vector <int> distr);

#endif  // MODULES_TASK_3_KOROBEINIKOV_A_CALCULATION_OF_INTEGRALS_CALCULATION_OF_INTEGRALS_H_

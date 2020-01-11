// Copyright 2019 Kriukov Dmitry
#ifndef MODULES_TASK_3_KRIUKOV_D_MULTIDIMENSIONAL_MONTE_KARLO_MULTIDIMENSIONAL_MONTE_KARLO_H_
#define MODULES_TASK_3_KRIUKOV_D_MULTIDIMENSIONAL_MONTE_KARLO_MULTIDIMENSIONAL_MONTE_KARLO_H_

#include<vector>

double multidimensionalIntegration(std::vector<double> start_point, double side, double(*pfunc)(std::vector<double>),
                                    bool(*parea)(std::vector<double>), unsigned int dimension, int point_count);
double multidimensionalIntegrationSequential(std::vector<double> start_point, double side,
    double(*pfunc)(std::vector<double>), bool(*parea)(std::vector<double>), unsigned int dimension, int point_count);

#endif  // MODULES_TASK_3_KRIUKOV_D_MULTIDIMENSIONAL_MONTE_KARLO_MULTIDIMENSIONAL_MONTE_KARLO_H_

// Copyright 2019 Kriukov Dmitry
#ifndef MODULES_TASK_2_KRIUKOV_D_FIXED_POINT_ITERATION_FIXED_POINT_ITERATION_H_
#define MODULES_TASK_2_KRIUKOV_D_FIXED_POINT_ITERATION_FIXED_POINT_ITERATION_H_

#include <vector>

std::vector<double> getRandomVector(int sz);
std::vector<double> getNormalMatrix(int sz);
std::vector<double> fixedPointIterationParralel(std::vector<double> matrix,
                                                std::vector<double> free_term, int var_count, double err);
std::vector<double> fixedPointIterationSequential(std::vector<double> matrix,
                                                  std::vector<double> free_term, int var_count, double err);
double getScalarProduct(const std::vector<double>& a, const std::vector<double>& b);
double getAbsError(std::vector<double> matrix,
                   std::vector<double> free_term, std::vector<double> resvec, int var_count);

#endif  // MODULES_TASK_2_KRIUKOV_D_FIXED_POINT_ITERATION_FIXED_POINT_ITERATION_H_

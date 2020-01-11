// Copyright 2019 Savosina
#ifndef MODULES_TASK_2_SAVOSINA_A_SEIDEL_METHOD_SEIDEL_METHOD_H_
#define MODULES_TASK_2_SAVOSINA_A_SEIDEL_METHOD_SEIDEL_METHOD_H_

#include <vector>

std::vector<double> getRandomVector(int n);
void PrintVector(std::vector<double> a, int n);
double Norma(const std::vector<double> x);
std::vector<double> Transparent(std::vector<double> a);

std::vector<double> getSequentialOperations(std::vector<double> a,
                                            std::vector<double> b, int n);
std::vector<double> getParallelOperations(std::vector<double> a,
                                            std::vector<double> b, int n);

#endif  // MODULES_TASK_2_SAVOSINA_A_SEIDEL_METHOD_SEIDEL_METHOD_H_

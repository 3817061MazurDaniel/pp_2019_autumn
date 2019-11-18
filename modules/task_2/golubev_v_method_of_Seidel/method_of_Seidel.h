// Copyright 2019 Golubev Vladislav
#ifndef MODULES_TASK_2_GOLUBEV_V_METHOD_OF_SEIDEL_METHOD_OF_SEIDEL_H_
#define MODULES_TASK_2_GOLUBEV_V_METHOD_OF_SEIDEL_METHOD_OF_SEIDEL_H_

#include <mpi.h>
#include <cstdlib>
#include <vector>
#include <random>
using std::vector;
double EvklNorm(const std::vector<double>& x);

std::vector<double> SequentialZeidel(const vector<vector<double> >& A, const vector<double>& b, int n, double eps);
std::vector<double> ParallelZeidel(const vector<vector<double> >& A, const vector<double>& _b, int _n, double eps);

#endif  // MODULES_TASK_2_GOLUBEV_V_METHOD_OF_SEIDEL_METHOD_OF_SEIDEL_H_

// Copyright 2019 Kukushkina Ksenia
#ifndef MODULES_TASK_2_KUKUSHKINA_K_SIMPLE_ITERATIONS_SIMPLE_ITERATIONS_H_
#define MODULES_TASK_2_KUKUSHKINA_K_SIMPLE_ITERATIONS_SIMPLE_ITERATIONS_H_

#include <mpi.h>
#include <vector>

double Norm(const std::vector<double>& b);
std::vector<double> Simple_Iterations(std::vector<double> A, std::vector<double> b, double precision);
std::vector<double> Simple_Iterations_MPI(std::vector<double> A, std::vector<double> b, double precision);

#endif  // MODULES_TASK_2_KUKUSHKINA_K_SIMPLE_ITERATIONS_SIMPLE_ITERATIONS_H_

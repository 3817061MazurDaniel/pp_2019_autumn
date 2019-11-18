// Copyright 2019 Tihomirova Mariya

#ifndef MODULES_TASK_2_TIHOMIROVA_M_JACOBI_JACOBI_H_
#define MODULES_TASK_2_TIHOMIROVA_M_JACOBI_JACOBI_H_

#include <mpi.h>
#include <vector>

double getNorm(const std::vector <double>& A, const std::vector<double> x, const std::vector <double>& b);
std::vector<double> Jacobi(const std::vector <double>& A, std::vector <double> x,
  double eps, int n, int rank, int part, int numprocs);
std::vector <double> Parallel(std::vector<double> matrix, std::vector <double> b, int Size, double eps);

#endif  // MODULES_TASK_2_TIHOMIROVA_M_JACOBI_JACOBI_H_

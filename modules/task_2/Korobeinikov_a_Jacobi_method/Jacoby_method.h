// Copyright 2019 Korobeinikov Aleksei
#ifndef MODULES_TASK_2_KOROBEINIKOV_A_JACOBI_METHOD_JACOBY_METHOD_H_
#define MODULES_TASK_2_KOROBEINIKOV_A_JACOBI_METHOD_JACOBY_METHOD_H_
#include <vector>

double scalar(const std::vector<double>& a, const std::vector<double>& b);

double getNormForAns(const std::vector <double>& A,
    const std::vector<double> x, const std::vector <double>& b);

std::vector<double> Jacobi_Solve(const std::vector <double>& A,
    std::vector <double> x, double eps,
    int n, int rank, int part, int numprocs);

std::vector <double> GetSolveOfTask(std::vector<double> matrix,
    std::vector <double> b, int Size, double eps);

#endif  // MODULES_TASK_2_KOROBEINIKOV_A_JACOBI_METHOD_JACOBY_METHOD_H_

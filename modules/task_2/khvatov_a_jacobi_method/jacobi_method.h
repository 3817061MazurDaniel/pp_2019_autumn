// Copyright 2019 Khvatov Alexander
#ifndef MODULES_TASK_2_KHVATOV_A_JACOBI_METHOD_JACOBI_METHOD_H_
#define MODULES_TASK_2_KHVATOV_A_JACOBI_METHOD_JACOBI_METHOD_H_

double* seq_jacobi_method(double* A, double *b, int n, double epsilon);
double* par_jacobi_method(double* A, double *b, int n, double epsilon);

#endif  // MODULES_TASK_2_KHVATOV_A_JACOBI_METHOD_JACOBI_METHOD_H_

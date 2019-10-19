// Copyright 2019 Tihomirova Mariya
#ifndef MODULES_TASK_1_TIHOMIROVA_M_TRAPEZOID_INTEGRATION_TRAPEZOID_INTEGRATION_H_
#define MODULES_TASK_1_TIHOMIROVA_M_TRAPEZOID_INTEGRATION_TRAPEZOID_INTEGRATION_H_
#include <mpi.h>
#include <math.h>

double Trapezoid_area(double(*f)(double), double x1, double x2, double h);
double  Parallelization(double(*f)(double), double a, double b, int n);

#endif  //  MODULES_TASK_1_TIHOMIROVA_M_TRAPEZOID_INTEGRATION_TRAPEZOID_INTEGRATION_H_


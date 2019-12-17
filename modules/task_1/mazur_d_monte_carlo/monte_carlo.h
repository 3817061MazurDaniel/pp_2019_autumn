// Copyright 2019 Mazur Daniil
#ifndef MODULES_TASK_1_MAZUR_D_MONTE_CARLO_MONTE_CARLO_H_
#define MODULES_TASK_1_MAZUR_D_MONTE_CARLO_MONTE_CARLO_H_

#include <mpi.h>
#include <math.h>
#include <iostream>


double parIntegral(int nPoints, double(*func)(double), double x1, double x2);
double soloIntegral(int nPoints, double(*func)(double), double x1, double x2);


#endif  // MODULES_TASK_1_MAZUR_D_MONTE_CARLO_MONTE_CARLO_H_

#ifndef MODULES_TASK_1_MAZUR_D_MONTE_CARLO_H_
#define MODULES_TASK_1_MAZUR_D_MONTE_CARLO_H_

#include <iostream>
#include <mpi.h>
#include <math.h>
using namespace std;


double getIntegral(int nPoints,  double(*func)(double), double x1, double x2);



#endif  // MODULES_TASK_1_MAZUR_D_MONTE_CARLO_H_

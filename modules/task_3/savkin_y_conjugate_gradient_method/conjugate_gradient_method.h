// Copyright 2019 Savkin Yuriy
#ifndef MODULES_TASK_3_SAVKIN_Y_CONJUGATE_GRADIENT_METHOD_CONJUGATE_GRADIENT_METHOD_H_
#define MODULES_TASK_3_SAVKIN_Y_CONJUGATE_GRADIENT_METHOD_CONJUGATE_GRADIENT_METHOD_H_

#include <mpi.h>
#include <vector>

double** getRandomMatrix(int size, double miv, double mav, unsigned seed);

double* getRandomVector(int size, double miv, double mav, unsigned seed);

std::vector<double> conjugateGradientMethod(double** a, const double* b, int size);

std::vector<double> conjugateGradientMethodOneProc(double** a, const double* b, int size);

#endif  // MODULES_TASK_3_SAVKIN_Y_CONJUGATE_GRADIENT_METHOD_CONJUGATE_GRADIENT_METHOD_H_

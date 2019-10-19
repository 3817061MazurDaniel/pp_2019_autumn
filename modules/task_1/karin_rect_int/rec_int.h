// Copyright 2019 Karin Timofey
#ifndef MODULES_TASK_1_KARIN_RECT_INT_REC_INT_H_
#define MODULES_TASK_1_KARIN_RECT_INT_REC_INT_H_

double GetRand();
double QuadrFunInt(double a, double b, double c, double k1, double k2);
double Integration(double(*f)(double), double a, double b, int k);
double ParIntegration(double(*f)(double), double a, double b, int k);

#endif  // MODULES_TASK_1_KARIN_RECT_INT_REC_INT_H_




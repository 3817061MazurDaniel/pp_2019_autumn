// Copyright 2019 Silenko Dmitrii
#ifndef MODULES_TASK_3_SILENKO_D_CONVEX_HULL_GRAHAM_CONVEX_HULL_GRAHAM_H_
#define MODULES_TASK_3_SILENKO_D_CONVEX_HULL_GRAHAM_CONVEX_HULL_GRAHAM_H_

#include <stack>

double** getRandomMas(int count);

bool SravnenieMore(int num, double* _mid, double* fi, double* r);
bool SravnenieLess(int num, double* _mid, double* fi, double* r);

double** QuickSort(double** mas, int first, int last, double* fi, double* r);

double det(double* p1, double* p2, double* p3, int s);
std::stack<double*> ConvSort(double** mas, const int count);

#endif  // MODULES_TASK_3_SILENKO_D_CONVEX_HULL_GRAHAM_CONVEX_HULL_GRAHAM_H_

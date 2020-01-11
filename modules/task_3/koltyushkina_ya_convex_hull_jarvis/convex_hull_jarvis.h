// Copyright 2019 Koltyushkina Yanina
#ifndef MODULES_TASK_3_KOLTYUSHKINA_YA_CONVEX_HULL_JARVIS_CONVEX_HULL_JARVIS_H_
#define MODULES_TASK_3_KOLTYUSHKINA_YA_CONVEX_HULL_JARVIS_CONVEX_HULL_JARVIS_H_


double** RandomHull(int size);

bool PointMore(int ind, double* mid, double* fi, double* r);
bool PointLess(int ind, double* mid, double* fi, double* r);

double** QuickSort(double** arr, int first, int last, double* fi, double* r);

double cosvec(double* p1, double* p2, double* p3);
double** ConvexHull(double** arr, const int nump);

#endif  // MODULES_TASK_3_KOLTYUSHKINA_YA_CONVEX_HULL_JARVIS_CONVEX_HULL_JARVIS_H_

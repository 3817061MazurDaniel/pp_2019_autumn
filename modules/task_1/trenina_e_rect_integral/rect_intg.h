// Copyright 2019 Trenina Elizaveta
#ifndef MODULES_TASK_1_TRENINA_E_RECT_INTEGRAL_RECT_INTG_H_
#define MODULES_TASK_1_TRENINA_E_RECT_INTEGRAL_RECT_INTG_H_


double get_l_rect_intg_parallel(double l, double r, int n, double(*f)(double));
double get_l_rect_intg_sequential(double l, double r, int n, double(*f)(double));

double get_midpoint_intg_sequential(double l, double r, int n, double(*f)(double));
double get_midpoint_intg_parallel(double l, double r, int n, double(*f)(double));

#endif  // MODULES_TASK_1_TRENINA_E_RECT_INTEGRAL_RECT_INTG_H_

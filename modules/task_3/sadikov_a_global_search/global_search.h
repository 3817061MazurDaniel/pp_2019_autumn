// Copyright 2019 Sadikov Artem
#ifndef MODULES_TASK_3_SADIKOV_A_GLOBAL_SEARCH_GLOBAL_SEARCH_H_
#define MODULES_TASK_3_SADIKOV_A_GLOBAL_SEARCH_GLOBAL_SEARCH_H_

#include <vector>

std::vector<double> split_vec(double a, double b, int proc);
std::vector<double> get_res(double a, double b, double r, double error,
                            double (*func)(double));
std::vector<double> test_func(std::vector<double> test_points);
double calc_new_point(double x_l, double x_r, double func_x_l,
                        double func_x_r, double M, int max_num, int size);
double calc_M(double r, std::vector<double> intervals,
                std::vector<double> test_points);
double interval_R(double a, double b, double x_l, double x_r, double func_x_l,
                    double func_x_r, double M);
bool equal(std::vector<double> actual, std::vector<double> expected);

#endif  // MODULES_TASK_3_SADIKOV_A_GLOBAL_SEARCH_GLOBAL_SEARCH_H_

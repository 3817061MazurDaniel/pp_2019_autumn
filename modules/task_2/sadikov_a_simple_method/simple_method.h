// Copyright 2019 Sadikov Artem
#ifndef MODULES_TASK_2_SADIKOV_A_SIMPLE_METHOD_SIMPLE_METHOD_H_
#define MODULES_TASK_2_SADIKOV_A_SIMPLE_METHOD_SIMPLE_METHOD_H_

#include <vector>

std::vector<double> get_rand_matrix(int size);
std::vector<double> solve_simple(std::vector<double> delta_a, std::vector<double> x,
                 double error, int size, int rank, int row_count,
                 int size_proc);
bool is_equal(std::vector<double> x, std::vector<double> y);
std::vector<double> get_res(std::vector<double> matrix, int size, double error);

#endif  // MODULES_TASK_2_SADIKOV_A_SIMPLE_METHOD_SIMPLE_METHOD_H_

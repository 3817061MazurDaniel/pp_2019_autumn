// Copyright 2019 Yasakova Anastasia
#ifndef MODULES_TASK_3_YASAKOVA_A_LINEAR_FILTERING_LINEAR_FILTERING_H_
#define MODULES_TASK_3_YASAKOVA_A_LINEAR_FILTERING_LINEAR_FILTERING_H_

#include <vector>

std::vector <int> getImage(int cols, int rows);
int CountPixel(const std::vector<int> mtx, const std::vector<int> mask, int i, int j, int rows, int cols);
std::vector <int> ParallelLinearFilter(const std::vector <int> mask, const std::vector <int> a, int rows, int cols);
std::vector <int> SequentialLinearFilter(const std::vector <int> mask, const std::vector <int> a, int rows, int cols);

#endif  // MODULES_TASK_3_YASAKOVA_A_LINEAR_FILTERING_LINEAR_FILTERING_H_

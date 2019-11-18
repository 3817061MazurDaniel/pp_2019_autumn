// Copyright 2019 Yasakova Anastasia
#ifndef MODULES_TASK_2_YASAKOVA_A_INCREASE_CONTRAST_INCREASE_CONTRAST_H_
#define MODULES_TASK_2_YASAKOVA_A_INCREASE_CONTRAST_INCREASE_CONTRAST_H_

#include <vector>

std::vector <int> getImage(int cols, int rows);
std::vector <int> ParallelIncreaseContrast(const std::vector <int>& a, int rows, int cols, int contrast);
std::vector <int> SequentialIncreaseContrast(const std::vector <int>& a, int rows, int cols, int contrast);
#endif  // MODULES_TASK_2_YASAKOVA_A_INCREASE_CONTRAST_INCREASE_CONTRAST_H_

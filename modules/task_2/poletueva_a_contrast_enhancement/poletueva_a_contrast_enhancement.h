// Copyright 2019 Poletueva Anastasia
#ifndef MODULES_TASK_2_POLETUEVA_A_CONTRAST_ENHANCEMENT_POLETUEVA_A_CONTRAST_ENHANCEMENT_H_
#define MODULES_TASK_2_POLETUEVA_A_CONTRAST_ENHANCEMENT_POLETUEVA_A_CONTRAST_ENHANCEMENT_H_

#define Error -1

#include<vector>

std::vector<int> ContrastFilterParallel(const std::vector <int> a, int rows, int cols, int _correction);
std::vector<int> ContrastFilter(const std::vector <int>& a, int rows, int cols, int _correction);
std::vector<int> getRandomM(int rows, int cols);

#endif  // MODULES_TASK_2_POLETUEVA_A_CONTRAST_ENHANCEMENT_POLETUEVA_A_CONTRAST_ENHANCEMENT_H_

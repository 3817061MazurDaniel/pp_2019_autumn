// Copyright 2019 Guseva Catherine
#ifndef MODULES_TASK_3_GUSEVA_E_HISTOGRAM_STRETCHING_HISTOGRAM_STRETCHING_H_
#define MODULES_TASK_3_GUSEVA_E_HISTOGRAM_STRETCHING_HISTOGRAM_STRETCHING_H_

#include <vector>

std::vector<int> getRandomImage(int cols, int rows);
std::vector<int> HistogramSequential(std::vector<int> src, int cols, int rows);
std::vector<int> HistogramParallel(std::vector<int> src, int cols, int rows);
int getMax(std::vector<int> src);
int getMin(std::vector<int> src);
void changePixle(std::vector<int>* temp, int max_value, int min_value);

#endif  // MODULES_TASK_3_GUSEVA_E_HISTOGRAM_STRETCHING_HISTOGRAM_STRETCHING_H_

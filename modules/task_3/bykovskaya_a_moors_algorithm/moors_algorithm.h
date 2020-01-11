// Copyright 2019 Bykovskaya Alina
#ifndef MODULES_TASK_3_BYKOVSKAYA_A_MOORS_ALGORITHM_MOORS_ALGORITHM_H_
#define MODULES_TASK_3_BYKOVSKAYA_A_MOORS_ALGORITHM_MOORS_ALGORITHM_H_

#include <vector>

std::vector<int> getRandomGraph(int size);
std::vector<int> Transpose(const std::vector<int>& g, int n);
std::vector<int> ParallelMoor(const std::vector<int>& g, int source,
                                int* flag = nullptr);
std::vector<int> Moors_algorithm(const std::vector<int>& g, int source,
                                int* flag = nullptr);

#endif  // MODULES_TASK_3_BYKOVSKAYA_A_MOORS_ALGORITHM_MOORS_ALGORITHM_H_

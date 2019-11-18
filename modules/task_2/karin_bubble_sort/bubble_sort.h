// Copyright 2019 Karin Timofey
#include <vector>
#ifndef MODULES_TASK_2_KARIN_BUBBLE_SORT_BUBBLE_SORT_H_
#define MODULES_TASK_2_KARIN_BUBBLE_SORT_BUBBLE_SORT_H_

std::vector<int> GetRandVec(int size);
std::vector<int> BubbleSort(const std::vector<int>& vect, int length);
std::vector<int> GetMinVec(const std::vector<int>& local_vec, int local_size,
                            const std::vector<int>& neig_vec, int neig_size);
std::vector<int> GetMaxVec(const std::vector<int>& local_vec, int local_size,
                            const std::vector<int>& neig_vec, int neig_size);
std::vector<int> ParBubbleSort(const std::vector<int>& vect);

#endif  // MODULES_TASK_2_KARIN_BUBBLE_SORT_BUBBLE_SORT_H_

// Copyright 2019 Karin Timofey
#ifndef MODULES_TASK_3_KARIN_RADIX_INT_SIMPLE_SORT_KARIN_RADIX_INT_SIMPLE_SORT_H_
#define MODULES_TASK_3_KARIN_RADIX_INT_SIMPLE_SORT_KARIN_RADIX_INT_SIMPLE_SORT_H_

#include <vector>
#include <string>

std::vector<int> GetRandVec(int size);
std::vector<int> Radix_sort_iter(const std::vector<int>& source, int n, int iter);
std::vector<int> Radix_sort(const std::vector<int>& source);
std::vector<int> Merge_sort(const std::vector<int>& loc_vec, int loc_size,
                            const std::vector<int>& neig_vec, int neig_size);
std::vector<int> Par_Radix_sort(const std::vector<int>& source);
int pow2(int st);

#endif  // MODULES_TASK_3_KARIN_RADIX_INT_SIMPLE_SORT_KARIN_RADIX_INT_SIMPLE_SORT_H_

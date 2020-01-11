// Copyright 2019 Zinkov Artem
#ifndef MODULES_TASK_3_ZINKOV_RADIX_SORT_MERGE_BATCHER_RADIX_SORT_MERGE_BATCHER_H_
#define MODULES_TASK_3_ZINKOV_RADIX_SORT_MERGE_BATCHER_RADIX_SORT_MERGE_BATCHER_H_

#include <mpi.h>
#include <vector>

std::vector<int> getRandomVector(int  size);
std::vector<int> merge_batcher(std::vector<int> global_vec, int size_vec);
std::vector<int> merge_even(const std::vector<int>& vec1, const std::vector<int>& vec2);
std::vector<int> merge_odd(const std::vector<int>& vec1, const std::vector<int>& vec2);
std::vector<int> transpos(std::vector<int> vec, int even_size, int odd_size);
std::vector<int> merge(std::vector<int> vec, int even_size, int odd_size);
std::vector<int> radix_sort(std::vector<int> vec);
std::vector<int> shuffle(std::vector<int> vec);

#endif  // MODULES_TASK_3_ZINKOV_RADIX_SORT_MERGE_BATCHER_RADIX_SORT_MERGE_BATCHER_H_

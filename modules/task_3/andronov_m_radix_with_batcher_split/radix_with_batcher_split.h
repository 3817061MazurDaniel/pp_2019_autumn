// Copyright 2019 Andronov Maxim
#ifndef MODULES_TASK_3_ANDRONOV_M_RADIX_WITH_BATCHER_SPLIT_RADIX_WITH_BATCHER_SPLIT_H_
#define MODULES_TASK_3_ANDRONOV_M_RADIX_WITH_BATCHER_SPLIT_RADIX_WITH_BATCHER_SPLIT_H_

#include <mpi.h>
#include <vector>

std::vector<int> GetRandomVector(int size);

std::vector<int> RadixSort(std::vector<int> array);

std::vector<int> ParallelRadixSortBatcherSplit(std::vector<int> array,
                                                    int size_arr);

#endif  // MODULES_TASK_3_ANDRONOV_M_RADIX_WITH_BATCHER_SPLIT_RADIX_WITH_BATCHER_SPLIT_H_

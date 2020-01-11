// Copyright 2019 Kurakin Mikhail
#ifndef MODULES_TASK_3_KURAKIN_M_BATCHER_SORT_BATCHER_SORT_H_
#define MODULES_TASK_3_KURAKIN_M_BATCHER_SORT_BATCHER_SORT_H_

#include <mpi.h>
#include <vector>

void GatComparators(std::vector<int> procs_up, std::vector<int> procs_down);
void getOddEvenSortNet(std::vector<int> procs);
void CreateSortNet(int numProcs);
void CreateArray(std::vector<int> *array);
void BatcherSort(std::vector<int> *arrIn);


#endif  // MODULES_TASK_3_KURAKIN_M_BATCHER_SORT_BATCHER_SORT_H_

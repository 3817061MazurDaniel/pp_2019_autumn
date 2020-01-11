// Copyright 2019 Trenina Elizaveta
#ifndef MODULES_TASK_3_TRENINA_E_SHELL_BATCHER_SORT_SHELL_BATCHER_SORT_H_
#define MODULES_TASK_3_TRENINA_E_SHELL_BATCHER_SORT_SHELL_BATCHER_SORT_H_

#include <vector>

typedef struct {
    int rank1;
    int rank2;
} pair;


std::vector<int> GetRandomVector(int n);

std::vector<int> Shell(std::vector<int> arr);

std::vector<int> Shell_Batcher(std::vector<int> arr);

void batcher(int proc_num);
void build_Network(std::vector<int> procsVec);
void build_Connection(std::vector<int>UpProcVec, std::vector<int>DownProcVec);

#endif  // MODULES_TASK_3_TRENINA_E_SHELL_BATCHER_SORT_SHELL_BATCHER_SORT_H_

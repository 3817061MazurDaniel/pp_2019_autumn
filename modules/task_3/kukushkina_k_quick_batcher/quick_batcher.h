// Copyright 2019 Kukushkina Ksenia
#ifndef MODULES_TASK_3_KUKUSHKINA_K_QUICK_BATCHER_QUICK_BATCHER_H_
#define MODULES_TASK_3_KUKUSHKINA_K_QUICK_BATCHER_QUICK_BATCHER_H_

#include <mpi.h>
#include <vector>

void generateRand(std::vector<int>* vec);
bool isSorted(const std::vector<int>& vec, int n);
void quickSort(std::vector<int>* vec, int low, int high);
void addComp(std::vector<int> upvec, std::vector<int> downvec);
void buildNet(std::vector<int> allranks);
void quickBatcher(std::vector<int>* vec);

#endif  // MODULES_TASK_3_KUKUSHKINA_K_QUICK_BATCHER_QUICK_BATCHER_H_

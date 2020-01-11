// Copyright 2019 Suslov Egor
#ifndef MODULES_TASK_2_SUSLOV_E_PRODUCERS_CONSUMERS_PRODUCERS_CONSUMERS_H_
#define MODULES_TASK_2_SUSLOV_E_PRODUCERS_CONSUMERS_PRODUCERS_CONSUMERS_H_

#include <vector>

int* Create_dinamic_massiv_from_vector(std::vector<int> vec);
int getPositive_elem(std::vector<int> vector, int count_size_vector);
std::vector<int> getRandomVector(int  size);
int Consumer(int *buffer, int buffer_size, int rank_proc, int* resurce);
int Producer(int *buffer, int buffer_size, int rank_proc, int resurce);

#endif  // MODULES_TASK_2_SUSLOV_E_PRODUCERS_CONSUMERS_PRODUCERS_CONSUMERS_H_

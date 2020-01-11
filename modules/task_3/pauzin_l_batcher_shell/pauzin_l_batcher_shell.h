// Copyright 2019 Pauzin Leonid
#ifndef MODULES_TASK_3_PAUZIN_L_BATCHER_SHELL_PAUZIN_L_BATCHER_SHELL_H_
#define MODULES_TASK_3_PAUZIN_L_BATCHER_SHELL_PAUZIN_L_BATCHER_SHELL_H_

#include <mpi.h>
#include <vector>

std::vector<int> getRandomVector(int size);
std::vector <int> ShellSort(const std::vector <int> &vec);
std::vector<int> evenFunc(const std::vector<int>& vec1, const std::vector<int>& vec2);
std::vector<int> oddFunc(const std::vector<int>& vec1, const std::vector<int>& vec2);
std::vector<int> transposition(std::vector<int> vec, int even_size, int odd_size);
std::vector<int> merge(std::vector<int> vec, int even_size, int odd_size);
std::vector<int> permutation(std::vector<int> vec);

std::vector<int> batcherParallel(std::vector<int> global_vec, int size_vec);
#endif  // MODULES_TASK_3_PAUZIN_L_BATCHER_SHELL_PAUZIN_L_BATCHER_SHELL_H_

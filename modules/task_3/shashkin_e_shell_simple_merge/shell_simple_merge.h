// Copyright 2019 Shashkin Evgeny
#ifndef MODULES_TASK_3_SHASHKIN_E_SHELL_SIMPLE_MERGE_SHELL_SIMPLE_MERGE_H_
#define MODULES_TASK_3_SHASHKIN_E_SHELL_SIMPLE_MERGE_SHELL_SIMPLE_MERGE_H_

#include <mpi.h>
#include <vector>

std::vector<int> GetRandomVector(int size);
std::vector<int> ShellSort(const std::vector<int> &vec, int vec_size);
std::vector<int> ShellSimpleMerge(std::vector<int> vec, int vec_size);
std::vector<int> SimpleMerge(const std::vector<int> &vec1, const std::vector<int> &vec2, int vec1_size, int vec2_size);

#endif  // MODULES_TASK_3_SHASHKIN_E_SHELL_SIMPLE_MERGE_SHELL_SIMPLE_MERGE_H_

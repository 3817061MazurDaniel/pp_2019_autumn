// Copyright 2019 Mityagina Daria
#ifndef MODULES_TASK_3_MITYAGINA_D_QUICK_SORT_QUICK_SORT_P_H_
#define MODULES_TASK_3_MITYAGINA_D_QUICK_SORT_QUICK_SORT_P_H_
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

std::vector<int> getRandomVector(int vectorSize);
std::vector<int> quick_s(std::vector<int> _vector, int left, int right);
std::vector<int> Merge_my_vectors(std::vector<int> my_vector1, std::vector<int> my_vector2, int m, int n);
std::vector<int> main_work(std::vector<int> my_vector, int N);
int part(std::vector<int> _vector, int left, int right);
std::vector<int> part_vect(std::vector<int> _vector, int left, int right);

#endif  // MODULES_TASK_3_MITYAGINA_D_QUICK_SORT_QUICK_SORT_P_H_

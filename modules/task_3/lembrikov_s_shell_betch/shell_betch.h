// Copyright 2019 Lembrikov Stepan
#ifndef MODULES_TASK_3_LEMBRIKOV_S_SHELL_BETCH_SHELL_BETCH_H_
#define MODULES_TASK_3_LEMBRIKOV_S_SHELL_BETCH_SHELL_BETCH_H_

#include <mpi.h>
#include <iostream>
#include <random>
#include <vector>

std::vector<int> getRandomVector(int n);
std::vector<int> getOtrVector(int n);
std::vector<int> ShellSort(const std::vector <int> &mas, int size);
std::vector<int> Shell(std::vector <int> mas);

#endif  // MODULES_TASK_3_LEMBRIKOV_S_SHELL_BETCH_SHELL_BETCH_H_

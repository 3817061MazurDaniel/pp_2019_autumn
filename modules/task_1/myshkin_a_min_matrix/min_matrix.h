// Copyright 2019 Myshkin Andrew
#ifndef MODULES_TASK_1_MYSHKIN_A_MIN_MATRIX_MIN_MATRIX_H_
#define MODULES_TASK_1_MYSHKIN_A_MIN_MATRIX_MIN_MATRIX_H_

#include <mpi.h>
#include <vector>

std::vector<int> getConsistentMatrix(int m, int n);
std::vector<int> getRandomMatrix(int m, int n);
int getParallelMinMatrix(const std::vector<int> miv, int rows, int cols);  // miv - matrix in vector

#endif  // MODULES_TASK_1_MYSHKIN_A_MIN_MATRIX_MIN_MATRIX_H_

// Copyright 2019 Myshkin Andrew
#ifndef MODULES_TASK_3_MYSHKIN_A_SHELL_SORT_SHELL_SORT_H_
#define MODULES_TASK_3_MYSHKIN_A_SHELL_SORT_SHELL_SORT_H_

#include <mpi.h>
#include <vector>

int* getRandomArray(int sizeA);
int getMinArray(int* buffer, int sizeA);
int* ShellSortSenq(int* buffer, int length);

int* parallelShellSort(int* buffer, int length);


#endif  // MODULES_TASK_3_MYSHKIN_A_SHELL_SORT_SHELL_SORT_H_

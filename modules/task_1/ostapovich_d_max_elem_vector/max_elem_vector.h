// Copyright 2019 Ostapovich Denis

#ifndef MODULES_TASK_1_OSTAPOVICH_D_MAX_ELEM_VECTOR_MAX_ELEM_VECTOR_H_
#define MODULES_TASK_1_OSTAPOVICH_D_MAX_ELEM_VECTOR_MAX_ELEM_VECTOR_H_

#include <vector>

const int VMAX_POSITIVE = 0;
const int VMAX_NEGATIVE = 1;
const int VMAX_MIXED = 2;

std::vector<int> GetVector(int size, int mode);
int SequentialMaxValue(std::vector<int> vec);
int ParallelMaxValue(std::vector<int> vec);

#endif  // MODULES_TASK_1_OSTAPOVICH_D_MAX_ELEM_VECTOR_MAX_ELEM_VECTOR_H_

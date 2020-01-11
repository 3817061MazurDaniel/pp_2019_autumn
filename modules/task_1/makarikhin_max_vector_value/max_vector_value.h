// Copyright 2019 Makarikhin Semen
#ifndef MODULES_TASK_1_MAKARIKHIN_MAX_VECTOR_VALUE_MAX_VECTOR_VALUE_H_
#define MODULES_TASK_1_MAKARIKHIN_MAX_VECTOR_VALUE_MAX_VECTOR_VALUE_H_

#include <mpi.h>
#include <vector>

std::vector<int> getRandomVector(int sz);

int GetMaxVectorValue(std::vector<int> vec);
int GetMaxVectorValueParallel(std::vector<int> global_vec, int size_vector);

#endif  // MODULES_TASK_1_MAKARIKHIN_MAX_VECTOR_VALUE_MAX_VECTOR_VALUE_H_



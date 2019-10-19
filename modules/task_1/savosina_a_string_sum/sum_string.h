// Copyright 2019 Savosina
#ifndef MODULES_TASK_1_SAVOSINA_A_STRING_SUM_SUM_STRING_H_
#define MODULES_TASK_1_SAVOSINA_A_STRING_SUM_SUM_STRING_H_

#include <string>
#include <vector>

std::vector<char> getRandomVector(int n);
int getSequentialOperationsVector(std::vector<char> str);
int getParallelOperationsVector(std::vector<char> global_str, int sz);
int getParallelOperationsVectorTry(std::vector<char> global_str, int sz);

#endif  // MODULES_TASK_1_SAVOSINA_A_STRING_SUM_SUM_STRING_H_

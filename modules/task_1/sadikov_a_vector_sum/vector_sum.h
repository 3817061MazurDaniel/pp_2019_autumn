// Copyright 2019 Sadikov Artem
#ifndef MODULES_TASK_1_SADIKOV_A_VECTOR_SUM_VECTOR_SUM_H_
#define MODULES_TASK_1_SADIKOV_A_VECTOR_SUM_VECTOR_SUM_H_

#include <vector>

std::vector<int> getRandomVector(int length);
int getSum(std::vector<int> vec);
int getSumMPI(std::vector<int> vec, int len);

#endif  // MODULES_TASK_1_SADIKOV_A_VECTOR_SUM_VECTOR_SUM_H_

// Copyright 2019 Dudchenko Anton

#ifndef MODULES_TASK_2_DUDCHENKO_A_VERT_MAT_ON_VECTOR_VERT_MAT_ON_VECTOR_H_
#define MODULES_TASK_2_DUDCHENKO_A_VERT_MAT_ON_VECTOR_VERT_MAT_ON_VECTOR_H_

#include <vector>

std::vector<int> getRandVector(std::size_t size);

std::vector<int> getRandMatrix(std::size_t size_n, std::size_t size_m);

std::vector<int> multiMatOnVec(std::vector<int> mat, std::size_t rows, std::size_t columns, std::vector<int> vec);

std::vector<int> multiMatOnVecSeq(std::vector<int> mat, std::size_t rows, std::size_t columns, std::vector<int> vec);

#endif  // MODULES_TASK_2_DUDCHENKO_A_VERT_MAT_ON_VECTOR_VERT_MAT_ON_VECTOR_H_

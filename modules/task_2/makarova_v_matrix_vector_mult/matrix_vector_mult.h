// Copyright 2019 Makarova Viktoria

#ifndef MODULES_TASK_2_MAKAROVA_V_MATRIX_VECTOR_MULT_MATRIX_VECTOR_MULT_H_
#define MODULES_TASK_2_MAKAROVA_V_MATRIX_VECTOR_MULT_MATRIX_VECTOR_MULT_H_

#include <vector>

std::vector<int> getRandomVector(int n);
std::vector<int> getRandomMatrix(int rows, int columns);
std::vector<int> multiply(std::vector<int> vector,
                          std::vector<int> matrix,
                          int cols, int rows);
std::vector<int> multiply_seq(std::vector<int> vector,
                              std::vector<int> matrix,
                              int cols, int rows);

#endif  // MODULES_TASK_2_MAKAROVA_V_MATRIX_VECTOR_MULT_MATRIX_VECTOR_MULT_H_

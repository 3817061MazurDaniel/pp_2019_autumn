// Copyright 2019 Sinitsina Maria
#ifndef MODULES_TASK_2_SINITSINA_M_MATRIX_VECTOR_HOR_MULIP_MATRIX_VECTOR_HOR_MULIP_H_
#define MODULES_TASK_2_SINITSINA_M_MATRIX_VECTOR_HOR_MULIP_MATRIX_VECTOR_HOR_MULIP_H_

#include <mpi.h>
#include <iostream>
#include <random>
#include <vector>

std::vector<int> RandomVector(int n);
std::vector<int> RandomMatrix(int row, int col);
std::vector<int> ConstVector(int n, int c);
std::vector<int> MatrixOnVectorMultip(const std::vector <int> &matr, int row, int col, const std::vector <int> &vect);

#endif  // MODULES_TASK_2_SINITSINA_M_MATRIX_VECTOR_HOR_MULIP_MATRIX_VECTOR_HOR_MULIP_H_

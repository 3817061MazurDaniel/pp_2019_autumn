// Copyright 2019 Zinkov Artem
#ifndef MODULES_TASK_2_ZINKOV_MATRIX_MULTIPLICATION_HOR_A_VER_B_MATRIX_MULTIPLICATION_HOR_A_VER_B_H_
#define MODULES_TASK_2_ZINKOV_MATRIX_MULTIPLICATION_HOR_A_VER_B_MATRIX_MULTIPLICATION_HOR_A_VER_B_H_

#include <mpi.h>
#include <vector>

std::vector<int> getRandomMatrix(int  matrix_side_size);
std::vector<int> getMatrixMultiplication(std::vector<int> matrix_a, std::vector<int> matrix_b, int matrix_side_size);
std::vector<int> getMatrixMultiplicationParellel(std::vector<int> matrix_a,
  std::vector<int> matrix_b, int matrix_side_size);

#endif  // MODULES_TASK_2_ZINKOV_MATRIX_MULTIPLICATION_HOR_A_VER_B_MATRIX_MULTIPLICATION_HOR_A_VER_B_H_

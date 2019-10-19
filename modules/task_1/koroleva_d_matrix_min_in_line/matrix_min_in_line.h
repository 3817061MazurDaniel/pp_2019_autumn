// Copyright 2018 Koroleva Daria
#ifndef MODULES_TASK_1_KOROLEVA_D_MATRIX_MIN_IN_LINE_MATRIX_MIN_IN_LINE_H_
#define MODULES_TASK_1_KOROLEVA_D_MATRIX_MIN_IN_LINE_MATRIX_MIN_IN_LINE_H_

#include <vector>

std::vector<int> getRandomMatrix(int m, int n);
std::vector<int> getMinsInMatrixLines(const std::vector<int>& matr,
                                      int m, int n);
std::vector<int> getParallelMinsInMatrixLines(const std::vector<int>& matr,
                                              int m, int n);

#endif  // MODULES_TASK_1_KOROLEVA_D_MATRIX_MIN_IN_LINE_MATRIX_MIN_IN_LINE_H_

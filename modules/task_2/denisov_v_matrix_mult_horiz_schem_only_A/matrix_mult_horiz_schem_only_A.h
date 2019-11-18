// Copyright 2019 Denisov Vladislav
#ifndef MODULES_TASK_2_DENISOV_V_MATRIX_MULT_HORIZ_SCHEM_ONLY_A_MATRIX_MULT_HORIZ_SCHEM_ONLY_A_H_
#define MODULES_TASK_2_DENISOV_V_MATRIX_MULT_HORIZ_SCHEM_ONLY_A_MATRIX_MULT_HORIZ_SCHEM_ONLY_A_H_


#include <vector>

std::vector<int> createRandomMatrix(int sizeSide);
std::vector<int> getMatrixMultSeq(std::vector<int> matrixA, std::vector<int> matrixB, int sizeSide);
std::vector<int> getMatrixMultPar(std::vector<int> matrixA, std::vector<int> matrixB, int sizeSide);
void printMatrix(std::vector<int> matrix, int sizeSide);

#endif  // MODULES_TASK_2_DENISOV_V_MATRIX_MULT_HORIZ_SCHEM_ONLY_A_MATRIX_MULT_HORIZ_SCHEM_ONLY_A_H_

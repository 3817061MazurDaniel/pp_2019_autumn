  // Copyright 2019 Boganov Sergey
#ifndef MODULES_TASK_3_BOGANOV_S_STRASSEN_ALGORITHM_STRASSEN_ALGORITHM_H_
#define MODULES_TASK_3_BOGANOV_S_STRASSEN_ALGORITHM_STRASSEN_ALGORITHM_H_

#include <vector>

std::vector<double> getRandomMatrix(int n, double min_rand, double max_rand);
std::vector<double> getSequentialSquareMatrixAddition(int n, const std::vector<double> &A,
const std::vector<double> &B);
std::vector<double> getSequentialSquareMatrixSubtraction(int n, const std::vector<double> &A,
const std::vector<double> &B);
std::vector<double> getSequentialSquareMatrixMultiplication(int n, const std::vector<double> &A,
const std::vector<double> &B);
int getNewSize(int n);
std::vector<double> getExpandSquareMatrix(int n, const std::vector<double> &A);
void getFourMatrixBlocks(int n, const std::vector<double> &A, std::vector<double> *A11,
std::vector<double> *A12, std::vector<double> *A21, std::vector<double> *A22);
std::vector<double> getJoinBlocksIntoMatrix(int n_block, const std::vector<double> &A11,
const std::vector<double> &A12, const std::vector<double> &A21, const std::vector<double> &A22);
std::vector<double> getSequentialStrassenMatrixMultiplication(int n, const std::vector<double> &A,
const std::vector<double> &B);
std::vector<double> getSequentialStrassenMatrixMultiplicationAlgorithm(int n, const std::vector<double> &A,
const std::vector<double> &B);
std::vector<double> getParallelStrassenMatrixMultiplication(int n, const std::vector<double> &A,
const std::vector<double> &B);
std::vector<double> getParallelStrassenMatrixMultiplicationAlgorithm(int n, const std::vector<double> &A,
const std::vector<double> &B, int proc_size);

#endif  // MODULES_TASK_3_BOGANOV_S_STRASSEN_ALGORITHM_STRASSEN_ALGORITHM_H_

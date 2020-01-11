// Copyright 2019 Okmyanskiy Andrey

#ifndef MODULES_TASK_3_OKMYANSKIY_A_CANNON_ALGORITHM_CANNON_ALGORITHM_H_
#define MODULES_TASK_3_OKMYANSKIY_A_CANNON_ALGORITHM_CANNON_ALGORITHM_H_

#include <mpi.h>
#include <vector>

std::vector<double> getRandomMatrix(int size);
std::vector<double> Multyplication(const std::vector<double> A, const std::vector<double> B);
std::vector<double> Add(std::vector<double> A, std::vector<double> B, int Size);
std::vector<double> getParallelMult(const std::vector<double> A,
    const std::vector<double> B, const int matrSize);

#endif  // MODULES_TASK_3_OKMYANSKIY_A_CANNON_ALGORITHM_CANNON_ALGORITHM_H_

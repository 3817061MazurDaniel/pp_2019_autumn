// Copyright 2019 Kudryashov Nikita
#ifndef MODULES_TASK_3_KUDRYASHOV_N_FOX_ALGORITHM_FOX_ALGORITHM_H_
#define MODULES_TASK_3_KUDRYASHOV_N_FOX_ALGORITHM_FOX_ALGORITHM_H_

#include <mpi.h>
#include <vector>

std::vector<double> fox_mult(std::vector<double> a, unsigned int a_size, std::vector<double> b, unsigned int b_size);
std::vector<double> subtask_matr_mult(std::vector<double> a_block, unsigned int a_block_size,
    std::vector<double> b_block, unsigned int b_block_size);  // Matrix multiplication.
bool is_square(int size);

#endif  // MODULES_TASK_3_KUDRYASHOV_N_FOX_ALGORITHM_FOX_ALGORITHM_H_

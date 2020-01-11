// Copyright 2019 Lembrikov Stepan
#ifndef MODULES_TASK_2_LEMBRIKOV_S_GOR_MAT_ON_VEC_GOR_MAT_ON_VEC_H_
#define MODULES_TASK_2_LEMBRIKOV_S_GOR_MAT_ON_VEC_GOR_MAT_ON_VEC_H_

#include <mpi.h>
#include <iostream>
#include <random>
#include <vector>

std::vector<int> getRandomVector(int n);
std::vector<int> getIdentityVector(int n);
std::vector<int> getConstVector(int n, int c);
std::vector<int> MatrixOnVector(const std::vector <int> &matr, int rows,
                                int cols, const std::vector <int> &vect);

#endif  // MODULES_TASK_2_LEMBRIKOV_S_GOR_MAT_ON_VEC_GOR_MAT_ON_VEC_H_

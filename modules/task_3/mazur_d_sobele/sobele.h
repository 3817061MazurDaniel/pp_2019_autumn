// Copyright 2019 Mazur Daniil
#ifndef MODULES_TASK_3_MAZUR_D_SOBELE_SOBELE_H_
#define MODULES_TASK_3_MAZUR_D_SOBELE_SOBELE_H_

#include <mpi.h>
#include <random>
#include <vector>

std::vector<int> randomMatrix(int rows, int cols);

int sobeleFilt(std::vector <int> matrix, int cols, int pos);

std::vector<int> soloSobele(std::vector<int> matrix, int rows, int cols);

std::vector<int> parSobele(std::vector<int> matrix, int rows, int cols);

#endif  // MODULES_TASK_3_MAZUR_D_SOBELE_SOBELE_H_

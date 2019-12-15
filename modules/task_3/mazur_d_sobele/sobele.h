// Copyright 2019 Golubev Vladislav
#ifndef MODULES_TASK_3_MAZUR_D_SOBELE_SOBELE_H_
#define MODULES_TASK_3_MAZUR_D_SOBELE_SOBELE_H_

#include <mpi.h>
#include <random>
#include <vector>

#define DEBUG

std::vector<unsigned int> randomMatrix(int rows, int cols);

unsigned int sobeleFilt(std::vector <unsigned int> matrix, int cols, int pos);

std::vector<unsigned int> soloSobele(std::vector<unsigned int> matrix, int rows, int cols);

std::vector<unsigned int> parSobele(std::vector<unsigned int> matrix, int rows, int cols);

#endif  // MODULES_TASK_3_MAZUR_D_SOBELE_SOBELE_H_

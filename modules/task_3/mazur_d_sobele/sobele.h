// Copyright 2019 Golubev Vladislav
#ifndef MODULES_TASK_3_MAZUR_D_SOBELE_SOBELE_H
#define MODULES_TASK_3_MAZUR_D_SOBELE_SOBELE_H

#include <mpi.h>
#include <random>
#include <vector>

#define DEBUG

std::vector<unsigned short> randomMatrix(int rows, int cols);

unsigned short sobeleFilt(std::vector <unsigned short> matrix, int cols, int pos);

std::vector<unsigned short> soloSobele(std::vector<unsigned short> matrix, int rows, int cols);

std::vector<unsigned short> parSobele(std::vector<unsigned short> matrix, int rows, int cols);
//int parSobele(std::vector<unsigned short> matrix, int rows, int cols);

#endif  // MODULES_TASK_3_MAZUR_D_SOBELE_SOBELE_H

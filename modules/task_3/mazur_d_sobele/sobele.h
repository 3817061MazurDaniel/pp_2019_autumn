// Copyright 2019 Mazur Daniil
#ifndef MODULES_TASK_3_MAZUR_D_SOBELE_SOBELE_H_
#define MODULES_TASK_3_MAZUR_D_SOBELE_SOBELE_H_

#include <mpi.h>
#include <random>
#include <vector>

std::vector<unsigned char> randomMatrix(int rows, int cols);

unsigned char sobeleFilt(std::vector <unsigned char> matrix, int cols, int pos);

std::vector<unsigned char> soloSobele(std::vector<unsigned char> matrix, int rows, int cols);

std::vector<unsigned char> parSobele(std::vector<unsigned char> matrix, int rows, int cols);

#endif  // MODULES_TASK_3_MAZUR_D_SOBELE_SOBELE_H_

// Copyright 2019 Koltyushkina Yanina

#ifndef MODULES_TASK_2_KOLTYUSHKINA_YA_GAUSS_VERT_GAUSS_VERT_H_
#define MODULES_TASK_2_KOLTYUSHKINA_YA_GAUSS_VERT_GAUSS_VERT_H_

#include <vector>

std::vector <double> RandomMatrix(int _size);
int maxind(int it, int _size, std::vector <double> mtr);
std::vector<double> PrGauss(std::vector<double> mtr, int _size);
std::vector<double> ObrGauss(std::vector<double> mtr, int _size);
int Proverka(std::vector<double> mtr, int _size);
bool NullStr(std::vector<double> mtr, int str, int _size);
std::vector<double> All(std::vector<double> mtr, int _size);
#endif  // MODULES_TASK_2_KOLTYUSHKINA_YA_GAUSS_VERT_GAUSS_VERT_H_

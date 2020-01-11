// Copyright 2019 Rezantsev Sergey
#ifndef MODULES_TASK_2_REZANTSEV_S_HOR_GAUSS_HOR_GAUSS_H_
#define MODULES_TASK_2_REZANTSEV_S_HOR_GAUSS_HOR_GAUSS_H_
#include <vector>
std::vector<double> getRandMatrix(int n);
std::vector<double> getGauss(const std::vector<double> &a, int n);
std::vector<double> getParGauss(const std::vector<double> &a, int n);
bool isItTrueAnswer(const std::vector<double> &a, int n, const std::vector<double> &x);
#endif  // MODULES_TASK_2_REZANTSEV_S_HOR_GAUSS_HOR_GAUSS_H_


// Copyright 2019 Bandenkov Daniil
#ifndef MODULES_TASK_3_BANDENKOV_D_RECTANG_INTEG_RECTANG_INTEG_H_
#define MODULES_TASK_3_BANDENKOV_D_RECTANG_INTEG_RECTANG_INTEG_H_
#include <vector>
#include <algorithm>
#include <utility>

double RectParall(double (*func)(std::vector<double>), std::vector <std::pair<double,
  double>> cordinate, const int leN);
double RectSequen(double (*func)(std::vector<double>), std::vector <std::pair<double,
  double>> cordinate, const int leN);

#endif  // MODULES_TASK_3_BANDENKOV_D_RECTANG_INTEG_RECTANG_INTEG_H_

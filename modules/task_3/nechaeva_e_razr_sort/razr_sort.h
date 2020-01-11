// Copyright 2019 Nechaeva Ekaterina
#ifndef MODULES_TASK_3_NECHAEVA_E_RAZR_SORT_RAZR_SORT_H_
#define MODULES_TASK_3_NECHAEVA_E_RAZR_SORT_RAZR_SORT_H_

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

std::vector<double> RazrSort(std::vector<double> src, std::vector<double> dest, int byte, int size);
std::vector<double> RazrSortLast(std::vector<double> src, std::vector<double> dest, int byte, int size);
std::vector<double> DoubleSortWin(std::vector<double> src, int size);
std::vector<double> DoubleSortLin(std::vector<double> src, int size);
std::vector<double> Rand(std::vector<double> mas, int size);
bool Tru(std::vector<double> mas, int size);
std::vector<double> ParallSort(std::vector<double> src, int size);
std::vector<double> Merge(std::vector<double> mas1, std::vector<double> mas2, int size1, int size2);

#endif  // MODULES_TASK_3_NECHAEVA_E_RAZR_SORT_RAZR_SORT_H_

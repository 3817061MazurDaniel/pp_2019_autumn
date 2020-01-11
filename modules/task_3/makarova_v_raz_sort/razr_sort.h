// Copyright 2019 Makarova Viktoria
#ifndef MODULES_TASK_3_MAKAROVA_V_RAZ_SORT_RAZR_SORT_H_
#define MODULES_TASK_3_MAKAROVA_V_RAZ_SORT_RAZR_SORT_H_

#include <string>
#include <vector>
#include <cstdlib>

std::vector<int> RazrIteration(std::vector<int> src, int raz);
std::vector<int> RazrSort(std::vector<int> src);
std::vector<int> Merge(std::vector<int> src1, std::vector<int> src2);
std::vector<int> ParallSort(std::vector<int> src);

int pow10(int pow);
int getRaz(int num, int raz);

std::vector<int> GetRandVector(int size);
bool isSorted(std::vector<int> src);


#endif  // MODULES_TASK_3_MAKAROVA_V_RAZ_SORT_RAZR_SORT_H_

/*
 * Copyright (C) 2019 LOOGIN. All Rights Reserved.
 */
#ifndef MODULES_TASK_1_LUGIN_M_MOST_CLOSER_ELEMENTS_CLOSERELEM_H_
#define MODULES_TASK_1_LUGIN_M_MOST_CLOSER_ELEMENTS_CLOSERELEM_H_

#include <mpi.h>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>

std::vector<int> ISearch(const std::vector<int>& vect);
std::vector<int> Search(const std::vector<int>& vect, int posFirstElem);
std::vector<int> Compare(const std::vector<int> &curVect, int comp[]);
std::string OutPutVector(std::vector<int> v);

#endif  // MODULES_TASK_1_LUGIN_M_MOST_CLOSER_ELEMENTS_CLOSERELEM_H_

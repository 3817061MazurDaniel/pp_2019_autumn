// Copyright 2019 Antipin Alexander
#ifndef MODULES_TASK_1_ANTIPIN_A_SCALAR_PRODUCT_SCALAR_PRODUCT_H_
#define MODULES_TASK_1_ANTIPIN_A_SCALAR_PRODUCT_SCALAR_PRODUCT_H_

#define MAX_NUMBER 200
#define MIN_NUMBER -100


#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <vector>
#include <iostream>


std::vector<int> creatRandomVector(const int v_size);

int getSequentialScalarProduct(const std::vector<int>& a, const std::vector<int>& b);

int getParallelScalarProduct(const std::vector<int>& a, const std::vector<int>& b);


#endif  // MODULES_TASK_1_ANTIPIN_A_SCALAR_PRODUCT_SCALAR_PRODUCT_H_

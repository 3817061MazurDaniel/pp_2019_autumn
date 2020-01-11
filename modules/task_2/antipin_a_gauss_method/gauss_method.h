// Copyright 2019 Antipin Alexander

#ifndef MODULES_TASK_2_ANTIPIN_A_GAUSS_METHOD_GAUSS_METHOD_H_
#define MODULES_TASK_2_ANTIPIN_A_GAUSS_METHOD_GAUSS_METHOD_H_

#define MAX_NUMBER 200
#define MIN_NUMBER -100
#define ORDER 10000


#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <cmath>
#include <random>
#include <vector>
#include <iostream>

class Matrix {
 private:
    std::vector<double> mem;
    int n;
 public:
    Matrix();
    explicit Matrix(const std::vector<double>& vec);
    explicit Matrix(const std::vector<std::vector<double>>& mat);
    explicit Matrix(const int n);
    Matrix(const Matrix& mat);
    ~Matrix();
    int getMatrixSize();
    std::vector<double>* getMemOfMatrix();
    double& getElem(const int row, const int col);
    std::vector<double> getSequentialSolution(const std::vector<double>& coefVec);
    std::vector<double> getParallelSolution(const std::vector<double>& coefVec);
};

double myAbs(const double& number);
void getRandomVector(std::vector<double>* vec);
double isZero(const double number);

#endif  // MODULES_TASK_2_ANTIPIN_A_GAUSS_METHOD_GAUSS_METHOD_H_

// Copyright 2019 Denisov Vladislav
#ifndef MODULES_TASK_3_DENISOV_V_GRADIENT_METHOD_GRADIENT_METHOD_H_
#define MODULES_TASK_3_DENISOV_V_GRADIENT_METHOD_GRADIENT_METHOD_H_


#include <vector>

std::vector<double> createRandomMatrix(int sizeSide);
std::vector<double> createRandomVector(int size);

double vectorMult(const std::vector<double>& vectorA, const std::vector<double>& vectorB);
std::vector<double> matrixVectorMult(const std::vector<double>& matrix, const std::vector<double>& vector);

std::vector<double> getSolveSeq(const std::vector<double>& matrix, const std::vector<double>& vector,
    int size);
std::vector<double> getSolvePar(const std::vector<double>& matrix, const std::vector<double>& vector,
    int sizeSide);

void printMatrix(std::vector<double> matrix, int size);
void printVector(std::vector<double> vector, int size);
void printSystem(std::vector<double> matrix, std::vector<double> vector, int size);

#endif  // MODULES_TASK_3_DENISOV_V_GRADIENT_METHOD_GRADIENT_METHOD_H_

// Copyright 2019 Mityagina Daria
#ifndef MODULES_TASK_1_MITYAGINA_D_SUM_OF_MATRIX_ELEMENTS_SUM_OF_MATRIX_ELEMENTS_H_
#define MODULES_TASK_1_MITYAGINA_D_SUM_OF_MATRIX_ELEMENTS_SUM_OF_MATRIX_ELEMENTS_H_

#include <stdlib.h>
#include <stdio.h>
#include <vector>

// основная работа
int Work(int size, std::vector<int> matrix);

// работа с отдельной частью, вычисление суммы
int SumOfMatrixElementsPartly(std::vector<int> matrix);

std::vector<int> getMatrix(int size);

#endif  // MODULES_TASK_1_MITYAGINA_D_SUM_OF_MATRIX_ELEMENTS_SUM_OF_MATRIX_ELEMENTS_H_

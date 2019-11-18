// Copyright 2019 Iamshchikov Ivan
#ifndef MODULES_TASK_1_IAMSHCHIKOV_I_COLUMNS_MIN_COLUMNS_MIN_H_
#define MODULES_TASK_1_IAMSHCHIKOV_I_COLUMNS_MIN_COLUMNS_MIN_H_

#include <mpi.h>
#include <vector>
#include <iostream>

void getRandomMatrix(std::vector<int>* matrix, int n, int m);
void transposeMatrix(const std::vector<int>* matrix,
                 std::vector<int>* row, int row_num, int col_num);
int getMinElem(const int* arr, int n);
int getInvolvedProcNumber(int n, int procnum);
MPI_Comm chooseComm(int n, int procnum, MPI_Comm c);
void setSendcount(std::vector<int>* elem_count, std::vector<int>* col_count,
                  int k, int n, int m);
void setDispl(std::vector<int>* elem_displ, std::vector<int>* elem_count,
              std::vector<int>* col_displ, std::vector<int>* col_count);
std::vector<int> getMinElementsSequential
                 (const std::vector<int>* matrix,
                 int row_num, int col_num);
std::vector<int> getMinElementsParallel
                 (const std::vector<int>* matrix,
                 int row_num, int col_num);

#endif  // MODULES_TASK_1_IAMSHCHIKOV_I_COLUMNS_MIN_COLUMNS_MIN_H_

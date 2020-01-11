// Copyright 2019 Suslov Egor
#ifndef MODULES_TASK_3_SUSLOV_E_MATRIX_CRS_MATRIX_CRS_H_
#define MODULES_TASK_3_SUSLOV_E_MATRIX_CRS_MATRIX_CRS_H_

#include <vector>

struct crsMatrix {
    int N;
    int NZ;
    double* Value;
    int* Col;
    int* RowIndex;
};
void InitializeMatrix(int N, int NZ, crsMatrix* mtx);
void FreeMatrix(crsMatrix* mtx);
double** mult_norm_matr(double** A, double** B, int N);
double** create_norm_mtr(crsMatrix A);
int MultiplicateMPI(crsMatrix* A, crsMatrix* B, crsMatrix* C);
void create_part_crs_C(int row_peredali, crsMatrix* A, crsMatrix* B, crsMatrix* C);
void MultiplicateGustafson(crsMatrix A, crsMatrix B, crsMatrix* C);
void GenerateRegularCRS(int N, int cntInRow, crsMatrix* mtx);

#endif  // MODULES_TASK_3_SUSLOV_E_MATRIX_CRS_MATRIX_CRS_H_


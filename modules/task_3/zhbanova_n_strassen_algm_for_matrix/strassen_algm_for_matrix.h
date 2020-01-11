
#ifndef MODULES_TASK_3_ZHBANOVA_N_STRASSEN_ALGM_FOR_MATRIX_STRASSEN_ALGM_FOR_MATRIX_H_
#define MODULES_TASK_3_ZHBANOVA_N_STRASSEN_ALGM_FOR_MATRIX_STRASSEN_ALGM_FOR_MATRIX_H_

// Copyright 2019 Zhbanova Nadezhda

double* MemoryVectorMatrix(int N);
void RandMatrix(double* matrix1, int N);
void PrintMatrix(double* matrix, int N);

double* Trivial_alghorithm(double* matrix1, double* matrix2, int N);

double* Add2(double* matrix1, double* matrix2, int N);
double* Add4(double* matrix1, double* matrix2, double* matrix3, double* matrix4, int N);
double* Sub2(double* matrix1, double* matrix2, int N);
double* Add3Sub1(double* matrix1, double* matrix2, double* matrix3, double* matrix4, int N);

double* Strassen_alg(double* matrix1, double* matrix2, int N);

double* Strassen_alg_parall(double* matrix1, double* matrix2, int N);

#endif  // MODULES_TASK_3_ZHBANOVA_N_STRASSEN_ALGM_FOR_MATRIX_STRASSEN_ALGM_FOR_MATRIX_H_

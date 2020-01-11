// Copyright 2019 Zhivaev Artem
#ifndef MODULES_TASK_3_ZHIVAEV_A_STRASSEN_ALG_STRASSEN_ALG_H_
#define MODULES_TASK_3_ZHIVAEV_A_STRASSEN_ALG_STRASSEN_ALG_H_

#include <mpi.h>

// mA - matrix A; mB - matrix B; mC - matrixC;
// C = A * B
// A, B and C has side x side size
void multSeq(double* mA, double* mB, double* mC, int mSide);
void multStrassen(double* mA, double* mB, double* mC, int mSide, MPI_Comm comm = MPI_COMM_WORLD);
void multStrassenTwoProc(double* mA, double* mB, double* mC, int mSide, MPI_Comm comm = MPI_COMM_WORLD);
void multStrassenFourProc(double* mA, double* mB, double* mC, int mSide, MPI_Comm comm = MPI_COMM_WORLD);
void multStrassenOneProc(double* mA, double* mB, double* mC, int mSide);

// Helper Functions
// C = A + B
void addMatrix(double* mA, double* mB, double* mC, int mSide);
// C = A - B
void subMatrix(double* mA, double* mB, double* mC, int mSide);
// M = (M11 M12)
//     (M21 M22)
void splitMatrix(double* m, double* m11, double* m12, double* m21, double* m22, int side);
// (M11 M12) = M
// (M21 M22)
void formMatrix(double* m, double* m11, double* m12, double* m21, double* m22, int side);

#endif  // MODULES_TASK_3_ZHIVAEV_A_STRASSEN_ALG_STRASSEN_ALG_H_


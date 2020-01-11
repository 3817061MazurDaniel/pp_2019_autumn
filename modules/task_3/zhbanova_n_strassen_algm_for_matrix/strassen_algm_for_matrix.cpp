// Copyright 2019 Zhbanova Nadezhda

#include "../../../modules/task_3/zhbanova_n_strassen_algm_for_matrix/strassen_algm_for_matrix.h"
#include <math.h>
#include <time.h>
#include <cmath>
#include <random>
#include <iostream>
#include "./mpi.h"

double* MemoryVectorMatrix(int N) {
  int N_N = N * N;
  double* matrix = new double[N_N];
  return matrix;
}

void RandMatrix(double* matrix1, int N) {
  std::random_device rnd_device;
  std::mt19937 mersenne_engine(rnd_device());
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix1[i * N + j] = mersenne_engine() % 10 + 0.5;
    }
  }
}

void PrintMatrix(double* matrix, int N) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      std::cout << matrix[i * N + j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

double* Trivial_alghorithm(double* matrix1, double* matrix2, int N) {
  double* Rez = MemoryVectorMatrix(N);
  double sum;
  for (int i = 0; i < N; i++)
    for (int j = 0; j <  N; j++) {
      sum = 0;
      for (int k = 0; k <  N; k++)
      sum += matrix1[i * N  + k] * matrix2[k * N + j];
      Rez[i * N + j] = sum;
    }
  return Rez;
}

double* Add2(double* matrix1, double* matrix2, int N) {
  double* Rez = MemoryVectorMatrix(N);
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      Rez[i * N + j] = matrix1[i * N + j] + matrix2[i * N + j];
  return Rez;
}

double* Add4(double* matrix1, double* matrix2, double* matrix3, double* matrix4, int N) {
  double* Rez = MemoryVectorMatrix(N);
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      Rez[i * N + j] = matrix1[i * N + j] + matrix2[i * N + j] + matrix3[i * N + j] + matrix4[i * N + j];
  return Rez;
}

double* Sub2(double* matrix1, double* matrix2, int N) {
  double* Rez = MemoryVectorMatrix(N);
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      Rez[i * N + j] = matrix1[i * N + j] - matrix2[i * N + j];
  return Rez;
}

double* Add3Sub1(double* matrix1, double* matrix2, double* matrix3, double* matrix4, int N) {
  double* Rez = MemoryVectorMatrix(N);
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      Rez[i * N + j] = matrix1[i * N + j] + matrix2[i * N + j] + matrix3[i * N + j] - matrix4[i * N + j];
  return Rez;
}

double* Strassen_alg(double* matrix1, double* matrix2, int N) {
  double* Rez;
  if (N <= 4) {
    Rez = Trivial_alghorithm(matrix1, matrix2, N);
  } else {
    Rez = MemoryVectorMatrix(N);
    N = N / 2;
    double* A[4];
    double* B[4];
    double* C[4];
    double* P[7];

    //  Allocate memory for auxiliary matrices
    for (int i = 0; i < 4; i++) {
      A[i] = MemoryVectorMatrix(N);
      B[i] = MemoryVectorMatrix(N);
    }

    //  We break matrixes into 4 blocks
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++) {
        int index_new = i * N + j, index_old = 2 * i * N + j, N_N = 2 * N * N;
        A[0][index_new] = matrix1[index_old];
        A[1][index_new] = matrix1[index_old + N];
        A[2][index_new] = matrix1[index_old + N_N];
        A[3][index_new] = matrix1[index_old + N_N + N];

        B[0][index_new] = matrix2[index_old];
        B[1][index_new] = matrix2[index_old + N];
        B[2][index_new] = matrix2[index_old + N_N];
        B[3][index_new] = matrix2[index_old + N_N + N];
      }

    //  We carry out multiplications of 7 pieces (recursively)
    double* TMP = Add2(A[0], A[3], N);
    double* _TMP = Add2(B[0], B[3], N);
    P[0] = Strassen_alg(TMP, _TMP, N);
    delete[] TMP;
    delete[] _TMP;

    TMP = Add2(A[2], A[3], N);
    P[1] = Strassen_alg(TMP, B[0], N);
    delete[] TMP;

    TMP = Sub2(B[1], B[3], N);
    P[2] = Strassen_alg(A[0], TMP, N);
    delete[] TMP;

    TMP = Sub2(B[2], B[0], N);
    P[3] = Strassen_alg(A[3], TMP, N);
    delete[] TMP;

    TMP = Add2(A[0], A[1], N);
    P[4] = Strassen_alg(TMP, B[3], N);
    delete[] TMP;

    TMP = Sub2(A[2], A[0], N);
    _TMP = Add2(B[0], B[1], N);
    P[5] = Strassen_alg(TMP, _TMP, N);
    delete[] TMP;
    delete[] _TMP;

    TMP = Sub2(A[1], A[3], N);
    _TMP = Add2(B[2], B[3], N);
    P[6] = Strassen_alg(TMP, _TMP, N);
    delete[] TMP;
    delete[] _TMP;

    //  Find the resulting values (blocks)
    C[0] = Add3Sub1(P[0], P[3], P[6], P[4], N);
    C[1] = Add2(P[2], P[4], N);
    C[2] = Add2(P[1], P[3], N);
    C[3] = Add3Sub1(P[0], P[2], P[5], P[1], N);
    //  We form the resulting matrix
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++) {
        Rez[i * 2 * N + j] = C[0][i * N + j];
        Rez[i * 2 * N + j + N] = C[1][i * N + j];
        Rez[i * 2 * N + j + 2 * N * N] = C[2][i * N + j];
        Rez[i * 2 * N + j + 2 * N * N + N] = C[3][i * N + j];
      }

    //  Releasing Allocated Memory
    for (int i = 0; i < 4; i++) {
      delete[] A[i];
      delete[] B[i];
      delete[] C[i];
    }
    for (int i = 0; i < 7; i++)
      delete[] P[i];
  }
  return Rez;
}

double* Strassen_alg_parall(double* matr_A, double* matr_B, int N) {
  double * matr_Rez_Par, ** A, ** B, ** TMP_Rez;
  int sqr, new_N;
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status Status;

  if (N <= 64) {
    return matr_Rez_Par = Strassen_alg(matr_A, matr_B, N);
  } else {
    if (rank == 0) {
      matr_Rez_Par = MemoryVectorMatrix(N);
      //  Memory allocation for auxiliary matrices and partitioning of matrices into blocks
      sqr = sqrt(size), new_N = N / sqr;
      A = new double* [size], B = new double* [size];
      for (int i = 0; i < size; i++) {
        A[i] = MemoryVectorMatrix(new_N);
        B[i] = MemoryVectorMatrix(new_N);
      }
      for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
          A[sqr * (i / new_N) + j / new_N][(i % new_N) * new_N + (j % new_N)] = matr_A[i * N + j];
          B[sqr * (i / new_N) + j / new_N][(i % new_N) * new_N + (j % new_N)] = matr_B[i * N + j];
        }
      //  Distribution of data to other processes
      MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
      for (int i = 1; i < size; i++) {
        int coef_A = sqr * (i / sqr), coef_B = i % sqr;
        for (int j = 0; j < sqr; j++) {
          MPI_Send(A[coef_A], new_N * new_N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
          MPI_Send(B[coef_B], new_N * new_N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
          coef_A++;
          coef_B += sqr;
        }
      }
      //  Swap pointers for tedious computing
      for (int i = 0; i < sqr; i++) {
        double* TMP = B[i];
        B[i] = B[i * sqr];
        B[i * sqr] = TMP;
      }
    }
    //  Receiving data from the root process and generating the necessary data
    if (size != 0) {
      MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
      sqr = sqrt(size), new_N = N / sqr;
      A = new double* [sqr], B = new double* [sqr];
      for (int i = 0; i < sqr; i++) {
        A[i] = MemoryVectorMatrix(new_N);
        B[i] = MemoryVectorMatrix(new_N);
      }
      for (int i = 0; i < sqr; i++) {
      MPI_Recv(A[i], new_N * new_N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Status);
      MPI_Recv(B[i], new_N * new_N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Status);
      }
    }
    //  each process calculating its piece of matrix
    TMP_Rez = new double* [sqr + 1];
    for (int i = 0; i < sqr; i++) {
      TMP_Rez[i + 1] = Strassen_alg(A[i], B[i], new_N);
    }
    if (size == 4)
      TMP_Rez[0] = Add2(TMP_Rez[1], TMP_Rez[2], new_N);
    if (size == 16)
      TMP_Rez[0] = Add4(TMP_Rez[1], TMP_Rez[2], TMP_Rez[3], TMP_Rez[4], new_N);
    //  Free up auxiliary memory
    if (rank == 0) {
      for (int i = 0; i < size; i++) {
        delete[] A[i];
        delete[] B[i];
      }
    } else {
      for (int i = 0; i < sqr; i++) {
        delete[] A[i];
        delete[] B[i];
      }
    }
    for (int i = 1; i < sqr + 1; i++) {
      delete[] TMP_Rez[i];
    }
    delete[] A;
    delete[] B;
    //  Sending result to 0 process
    if (rank != 0) {
      MPI_Send(TMP_Rez[0], new_N * new_N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
      delete[] TMP_Rez[0];
    }
    if (rank == 0) {
      int coef = sqrt(size);
      //  write down the result of our work
      for (int i = 0; i < new_N; i++)
        for (int j = 0; j < new_N; j++)
          matr_Rez_Par[coef * i * new_N + j] = TMP_Rez[0][i * new_N + j];
      for (int k = 1; k < size; k++) {
        //  accept and record the results of other processes
        MPI_Recv(TMP_Rez[0], new_N * new_N, MPI_DOUBLE, k, 0, MPI_COMM_WORLD, &Status);
        for (int i = 0; i < new_N; i++)
          for (int j = 0; j < new_N; j++)
            matr_Rez_Par[(k / coef) * new_N * N + (k % coef) * new_N + coef * i * new_N + j]
              = TMP_Rez[0][i * new_N + j];
      }
      return matr_Rez_Par;
      delete[] TMP_Rez[0];
      delete[] matr_Rez_Par;
    }
    delete[] TMP_Rez;
  }
  return matr_Rez_Par;
}

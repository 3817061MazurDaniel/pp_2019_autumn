// Copyright 2019 Maximova Irina
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cmath>
#include "./fox.h"

TEST(Fox, Test_on_Matrix_size_4) {
  int size = 4;
  double tmp;
  double* A = &tmp;
  double* B = &tmp;
  double* C = &tmp;
  double* CFox = &tmp;
  int rank, procNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);

  int cartSize = static_cast<int>(std::sqrt(procNum));
  if (cartSize * cartSize == procNum && size % cartSize == 0) {
    if (rank == 0) {
      A = new double[size * size];
      B = new double[size * size];
      RandomOperandMatrix(A, B, size);
      C = new double[size * size];
      CFox = new double[size * size];
      SequentialAlgorithm(A, B, C, size);
    }
    Fox(A, B, CFox, size);

    if (rank == 0) {
      for (int i = 0; i < size * size; ++i) ASSERT_NEAR(CFox[i], C[i], 0.0001);
    }

    if (rank == 0) {
      delete[] A;
      delete[] B;
      delete[] C;
      delete[] CFox;
    }
  }
}

TEST(Fox, Test_on_Matrix_size_16) {
  int size = 16;
  double tmp;
  double* A = &tmp;
  double* B = &tmp;
  double* C = &tmp;
  double* CFox = &tmp;
  int rank, procNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);

  int cartSize = static_cast<int>(std::sqrt(procNum));
  if (cartSize * cartSize == procNum && size % cartSize == 0) {
    if (rank == 0) {
      A = new double[size * size];
      B = new double[size * size];
      RandomOperandMatrix(A, B, size);
      C = new double[size * size];
      CFox = new double[size * size];
      SequentialAlgorithm(A, B, C, size);
    }
    Fox(A, B, CFox, size);

    if (rank == 0) {
      for (int i = 0; i < size * size; ++i) ASSERT_NEAR(CFox[i], C[i], 0.0001);
    }

    if (rank == 0) {
      delete[] A;
      delete[] B;
      delete[] C;
      delete[] CFox;
    }
  }
}

TEST(Fox, Test_on_Matrix_size_64) {
  int size = 64;
  double temp;
  double* A = &temp;
  double* B = &temp;
  double* C = &temp;
  double* CFox = &temp;
  int rank, procNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);

  int cartSize = static_cast<int>(std::sqrt(procNum));
  if (cartSize * cartSize == procNum && size % cartSize == 0) {
    if (rank == 0) {
      A = new double[size * size];
      B = new double[size * size];
      RandomOperandMatrix(A, B, size);
      C = new double[size * size];
      CFox = new double[size * size];
      SequentialAlgorithm(A, B, C, size);
    }
    Fox(A, B, CFox, size);

    if (rank == 0) {
      for (int i = 0; i < size * size; ++i) ASSERT_NEAR(CFox[i], C[i], 0.0001);
    }

    if (rank == 0) {
      delete[] A;
      delete[] B;
      delete[] C;
      delete[] CFox;
    }
  }
}

TEST(Fox, Throw_Matrix_size_0) {
  int size = 0;
  double tmp;
  double* M = &tmp;
  ASSERT_ANY_THROW(Fox(M, M, M, size));
  ASSERT_ANY_THROW(SequentialAlgorithm(M, M, M, size));
}

TEST(Fox, Throw_Matrix_size_11) {
  int size = 13;
  double tmp;
  double* M = &tmp;
  int procNum;
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);

  if (procNum > 1) {
    ASSERT_ANY_THROW(Fox(M, M, M, size));
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}

// Copyright 2019 Zhbanova Nadezhda

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include "./strassen_algm_for_matrix.h"

TEST(strassen_algm_for_matrix, check_Trivial_alghorithm) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size < 2) {
    int N = 4;
    double* matr_a = MemoryVectorMatrix(N);
    double* matr_b = MemoryVectorMatrix(N);
    double* matr_c = MemoryVectorMatrix(N);

    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        matr_a[i * N + j] = i + 1;
        matr_b[i * N + j] = i + 1;
        matr_c[i * N + j] = (i + 1) * 10;
      }
    }
    double* matr_trivial = Trivial_alghorithm(matr_a, matr_b, N);
    for (int k = 0; k < N; k++)
      for (int l = 0; l < N; l++)
        ASSERT_TRUE(matr_c[k * N + l] == matr_trivial[k * N + l]);
  }
}

TEST(strassen_algm_for_matrix, check_Strassen_algorithm) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size < 2) {
    int N = 4;
    double* matr_A = MemoryVectorMatrix(N);
    double* matr_B = MemoryVectorMatrix(N);
    RandMatrix(matr_A, N);
    RandMatrix(matr_B, N);

    double* matr_trivial = Trivial_alghorithm(matr_A, matr_B, N);
    double* matr_strassen = Strassen_alg(matr_A, matr_B, N);
    for (int k = 0; k < N; k++)
      for (int l = 0; l < N; l++)
        ASSERT_TRUE(matr_strassen[k * N + l] == matr_trivial[k * N + l]);
  }
}

TEST(strassen_algm_for_matrix, check_Strassen_alg_parall) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    int N = 4;
    double* matr_a = MemoryVectorMatrix(N);
    double* matr_b = MemoryVectorMatrix(N);
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        matr_a[i * N + j] = i + 1;
        matr_b[i * N + j] = i + 1;
      }
    }
    double* matr_trivial = Trivial_alghorithm(matr_a, matr_b, N);
    double* matr_strassen_parall = Strassen_alg_parall(matr_a, matr_b, N);
    for (int k = 0; k < N; k++)
      for (int l = 0; l < N; l++)
        ASSERT_TRUE(matr_strassen_parall[k * N + l] == matr_trivial[k * N + l]);
  }
}

TEST(strassen_algm_for_matrix, check_func_Add_two_matrix) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size < 2) {
    int N = 4;
    double* matr_a = MemoryVectorMatrix(N);
    double* matr_b = MemoryVectorMatrix(N);
    double* matr_c = MemoryVectorMatrix(N);

    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        matr_a[i * N + j] = i + 1;
        matr_b[i * N + j] = i + 1;
        matr_c[i * N + j] = (i + 1) *2;
      }
    }
    double* matr_add2 = Add2(matr_a, matr_b, N);
    for (int k = 0; k < N; k++)
      for (int l = 0; l < N; l++)
        ASSERT_TRUE(matr_c[k * N + l] == matr_add2[k * N + l]);
  }
}

TEST(strassen_algm_for_matrix, check_func_Add_four_matrix) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size < 2) {
    int N = 4;
    double* matr_a = MemoryVectorMatrix(N);
    double* matr_b = MemoryVectorMatrix(N);
    double* matr_c = MemoryVectorMatrix(N);

    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        matr_a[i * N + j] = i + 1;
        matr_b[i * N + j] = i + 1;
        matr_c[i * N + j] = (i + 1) * 4;
      }
    }
    double* matr_add4 = Add4(matr_a, matr_a, matr_b, matr_b, N);
    for (int k = 0; k < N; k++)
      for (int l = 0; l < N; l++)
        ASSERT_TRUE(matr_c[k * N + l] == matr_add4[k * N + l]);
  }
}

TEST(strassen_algm_for_matrix, check_func_Sub_two_matrix) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size < 2) {
    int N = 4;
    double* matr_a = MemoryVectorMatrix(N);
    double* matr_b = MemoryVectorMatrix(N);
    double* matr_c = MemoryVectorMatrix(N);

    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        matr_a[i * N + j] = i + 1;
        matr_b[i * N + j] = i + 1;
        matr_c[i * N + j] = 0;
      }
    }
    double* matr_sub2 = Sub2(matr_a, matr_b, N);
    for (int k = 0; k < N; k++)
      for (int l = 0; l < N; l++)
        ASSERT_TRUE(matr_c[k * N + l] == matr_sub2[k * N + l]);
  }
}

TEST(strassen_algm_for_matrix, check_func_Add_three_Sub_one_matrix) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size < 2) {
    int N = 4;
    double* matr_a = MemoryVectorMatrix(N);
    double* matr_b = MemoryVectorMatrix(N);
    double* matr_c = MemoryVectorMatrix(N);

    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        matr_a[i * N + j] = i + 1;
        matr_b[i * N + j] = i + 1;
        matr_c[i * N + j] = (i + 1) * 2;
      }
    }
    double* matr_add3sub1 = Add3Sub1(matr_a, matr_a, matr_b, matr_b, N);
    for (int k = 0; k < N; k++)
      for (int l = 0; l < N; l++)
        ASSERT_TRUE(matr_c[k * N + l] == matr_add3sub1[k * N + l]);
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



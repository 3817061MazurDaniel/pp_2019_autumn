// Copyright 2019 Okmyanskiy Andrey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "./sum_val_in_rows.h"

TEST(Parallel_Sum_Val_In_Rows_MPI, Test_Sum_Val_In_Rows_15x15) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int matrix_size_n = 15;
  const int matrix_size_m = 15;

  if (rank == 0) {
      global_vec = getRandomMatrix(matrix_size_m, matrix_size_n);
  }

  std::vector<int>matr_1 = getParallelSumValInRows(global_vec, matrix_size_m, matrix_size_n);

  if (rank == 0) {
      std::vector<int>matr_2 = getSequintialSumValInRows(global_vec, matrix_size_m, matrix_size_n);
      ASSERT_EQ(matr_1, matr_2);
  }
}

TEST(Parallel_Sum_Val_In_Rows_MPI, Test_Sum_Val_In_Rows_100x100) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int matrix_size_n = 100;
  const int matrix_size_m = 100;

  if (rank == 0) {
      global_vec = getRandomMatrix(matrix_size_m, matrix_size_n);
  }

  std::vector<int>matr_1 = getParallelSumValInRows(global_vec, matrix_size_m, matrix_size_n);

  if (rank == 0) {
      std::vector<int>matr_2 = getSequintialSumValInRows(global_vec, matrix_size_m, matrix_size_n);
      ASSERT_EQ(matr_1, matr_2);
  }
}

TEST(Parallel_Sum_Val_In_Rows_MPI, Test_Matrix_Size_Zero) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int matrix_size_n = 20;
  const int matrix_size_m = 0;

  if (rank == 0) {
      ASSERT_ANY_THROW(getRandomMatrix(matrix_size_m, matrix_size_n));
  }
}

TEST(Parallel_Sum_Val_In_Rows_MPI, Test_Matrix_Size_Negative) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> matrix;
  const int matrix_size_n = -13;
  const int matrix_size_m = 7;

  if (rank == 0) {
      ASSERT_ANY_THROW(getRandomMatrix(matrix_size_m, matrix_size_n));
  }
}

TEST(Parallel_Sum_Val_In_Rows_MPI, Test_Matrix_Wrong_Size) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> matrix;
  const int matrix_size_n = 12;
  const int matrix_size_m = 10;

  if (rank == 0) {
      matrix = getRandomMatrix(15, 15);
  }

  ASSERT_ANY_THROW(getParallelSumValInRows(matrix, matrix_size_m, matrix_size_n));
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

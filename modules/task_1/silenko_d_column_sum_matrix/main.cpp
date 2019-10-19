// Copyright 2019 Silenko Dmitrii

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_1/silenko_d_column_sum_matrix/column_sum_matrix.h"

TEST(Column_sum_matrix_MPI, test_on_wrong_matrix_1) {
  std::vector <int> a = getRandomMatrixE(1, 1);
  std::vector <int> res;
  EXPECT_ANY_THROW(res = ColumnSumMatrix(a, 1, 2));
}

TEST(Column_sum_matrix_MPI, test_on_wrong_matrix_2) {
  EXPECT_ANY_THROW(std::vector <int> a = getRandomMatrixE(0, 1));
}

TEST(Column_sum_matrix_MPI, test_on_wrong_matrix_3) {
  std::vector <int> a = getRandomMatrixE(1, 2);
  std::vector <int> res;
  EXPECT_ANY_THROW(res = ColumnSumMatrix(a, 0, 2));
}

TEST(Column_sum_matrix_MPI, test_on_matrix_2_x_2_o) {
  std::vector <int> a = getRandomMatrixO(2, 2);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector <int> sum = ColumnSumMatrix(a, 2, 2);
  std::vector <int> res = { 2, 4 };
  if (rank == 0) {
    EXPECT_EQ(res, sum);
  }
}

TEST(Column_sum_matrix_MPI, test_on_matrix_4_x_4_e) {
  std::vector <int> a = getRandomMatrixE(4, 4);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector <int> sum = ColumnSumMatrix(a, 4, 4);
  std::vector <int> res = {4, 4, 4, 4 };
  if (rank == 0) {
    EXPECT_EQ(res[0], sum[0]);
  }
}

TEST(Column_sum_matrix_MPI, test_on_rectangular_matrix_1) {
  std::vector <int> a = getRandomMatrixO(3, 4);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector <int> sum = ColumnSumMatrix(a, 3, 4);
  std::vector <int> res = { 3, 6, 9, 12 };
  if (rank == 0) {
    EXPECT_EQ(res, sum);
  }
}

TEST(Column_sum_matrix_MPI, test_on_rectangular_matrix_2) {
  std::vector <int> a = getRandomMatrixO(6, 2);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector <int> sum = ColumnSumMatrix(a, 6, 2);
  std::vector <int> res = { 6, 12 };
  if (rank == 0) {
    EXPECT_EQ(res, sum);
  }
}

TEST(Column_sum_matrix_MPI, test_on_my_matrix) {
  std::vector <int> a(4);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  a[0] = 24;
  a[1] = 47;
  a[2] = 19;
  a[3] = -20;
  std::vector <int> sum = ColumnSumMatrix(a, 2, 2);
  std::vector <int> res = { 71, -1 };
  if (rank == 0) {
    EXPECT_EQ(res, sum);
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

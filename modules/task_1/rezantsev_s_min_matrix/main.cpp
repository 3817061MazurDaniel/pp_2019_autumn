// Copyright 2019 Rezantsev Sergey
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <vector>
#include "../../../modules/task_1/rezantsev_s_min_matrix/min_matrix.h"

TEST(Min_Matrix_MPI, test_on_matrix_10x10) {
  std::vector<std::vector<int>> a = getRandMatrix(10, 10);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> res = getOrdMinOfMatrix(a, 10, 10);
  std::vector<int> minimum = getMinOfMatrix(a, 10, 10);
  if (rank == 0) {
    EXPECT_EQ(res, minimum);
  }
}

TEST(Min_Matrix_MPI, test_on_matrix_15x15) {
  std::vector<std::vector<int>> a = getRandMatrix(15, 15);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> res = getOrdMinOfMatrix(a, 15, 15);
  std::vector<int> minimum = getMinOfMatrix(a, 15, 15);
  if (rank == 0) {
    EXPECT_EQ(res, minimum);
  }
}

TEST(Min_Matrix_MPI, test_on_minus_one_matrix_10x10) {
  std::vector<std::vector<int>> a = getRandMatrix(10, 10);
  int rank;
  a[5][5] = -1;
  std::vector<int> res = getOrdMinOfMatrix(a, 10, 10);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> minimum = getMinOfMatrix(a, 10, 10);
  if (rank == 0) {
    EXPECT_EQ(res, minimum);
  }
}

TEST(Min_Matrix_MPI, test_on_matrix_10x1) {
  std::vector<std::vector<int>> a = getRandMatrix(10, 1);
  int rank;
  std::vector<int> res = getOrdMinOfMatrix(a, 10, 1);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> minimum = getMinOfMatrix(a, 10, 1);
  if (rank == 0) {
    EXPECT_EQ(res, minimum);
  }
}

TEST(Min_Matrix_MPI, test_on_matrix_1x10) {
  std::vector<std::vector<int>> a(1, std::vector<int>(10));
  for (int i = 0; i < 10; i++) a[0][i] = -i - 1;
  int rank;
  a[0][9] = 10;
  std::vector<int> res = getOrdMinOfMatrix(a, 1, 10);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> minimum = getMinOfMatrix(a, 1, 10);
  if (rank == 0) {
    EXPECT_EQ(res, minimum);
  }
}

TEST(Min_Matrix_MPI, test_on_matrix_100x100) {
  std::vector<std::vector<int>> a = getRandMatrix(100, 100);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> res = getOrdMinOfMatrix(a, 100, 100);
  std::vector<int> minimum = getMinOfMatrix(a, 100, 100);
  if (rank == 0) {
    EXPECT_EQ(res, minimum);
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

// Copyright 2019 Soboleva Julia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_1/soboleva_j_max_in_matrix_row/max_in_matrix_row.h"

TEST(Parallel_Operations_MPI, Test_Matrix_Size) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int m = 17;
  int n = 40;
  size_t size = m * n;

  std::vector<int> a = getRandMatrix(m, n);
  ASSERT_EQ(size, a.size());
}

TEST(Parallel_Operations_MPI, Test_Throws_When_Size_Is_Negative) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int m = -17;
  int n = 40;

  ASSERT_ANY_THROW(std::vector<int> a = getRandMatrix(m, n););
}

TEST(Parallel_Operations_MPI, Test_Max_Values_Are_Correct) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int m = 3;
  int n = 4;
  std::vector<int> a = { -10000, -1000, -100, -10,
                        1768, -1768, 9, -9,
                        5, 5, 5, 5,  };
  std::vector<int> vec = { -10, 1768, 5};

  std::vector<int> res = getMaxInMatrixRows(a, m, n);

  EXPECT_EQ(vec, res);
}

TEST(Parallel_Operations_MPI, Test_Parallel_Maxs_Are_Correct_For_Small_Matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> a;
  int m = 17;
  int n = 8;

  if (rank == 0) {
    a = getRandMatrix(m, n);
  }

  std::vector<int> resPar = getParMaxInMatrixRows(a, m, n);

  if (rank == 0) {
    std::vector<int> res = getMaxInMatrixRows(a, m, n);
    ASSERT_EQ(resPar, res);
  }
}

TEST(Parallel_Operations_MPI, Test_Parallel_Maxs_Are_Correct_For_Big_Matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> a;
  const int m = 396;
  const int n = 7;

  if (rank == 0) {
    a = getRandMatrix(m, n);
  }

  std::vector<int> resPar = getParMaxInMatrixRows(a, m, n);

  if (rank == 0) {
    std::vector<int> res = getMaxInMatrixRows(a, m, n);
    ASSERT_EQ(resPar, res);
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

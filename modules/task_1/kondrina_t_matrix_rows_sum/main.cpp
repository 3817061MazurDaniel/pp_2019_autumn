// Copyright 2019 Kondrina Tatyana

#include <gtest/gtest.h>
#include <mpi.h>

#include <gtest-mpi-listener.hpp>
#include <vector>

#include "../../../modules/task_1/kondrina_t_matrix_rows_sum/matrix_rows_sum.h"

TEST(Matrix_Rows_Sum_MPI, Test_Matrix_Seq) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    const int columns = 4, rows = 3;
    std::vector<int> testMatrix{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    std::vector<int> expectedResult{10, 26, 42};
    std::vector<int> result = rowsSumSeq(testMatrix, columns, rows);

    ASSERT_EQ(expectedResult, result);
  }
}

TEST(Matrix_Rows_Sum_MPI, Test_Matrix_10x9) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int columns = 10, rows = 9;

  std::vector<int> testMatrix;
  if (rank == 0) {
    testMatrix = randomMatrix(columns, rows);
  }
  std::vector<int> result;

  result = rowsSum(testMatrix, columns, rows);

  if (rank == 0) {
    std::vector<int> exp_result = rowsSumSeq(testMatrix, columns, rows);
    ASSERT_EQ(result, exp_result);
  }
}

TEST(Matrix_Rows_Sum_MPI, Test_Matrix_4x5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int columns = 4, rows = 5;

  std::vector<int> testMatrix;
  if (rank == 0) {
    testMatrix = randomMatrix(columns, rows);
  }
  std::vector<int> result;

  result = rowsSum(testMatrix, columns, rows);

  if (rank == 0) {
    std::vector<int> exp_result = rowsSumSeq(testMatrix, columns, rows);
    ASSERT_EQ(result, exp_result);
  }
}

TEST(Matrix_Rows_Sum_MPI, Test_Matrix_50x60) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int columns = 50, rows = 60;

  std::vector<int> testMatrix;
  if (rank == 0) {
    testMatrix = randomMatrix(columns, rows);
  }
  std::vector<int> result;

  result = rowsSum(testMatrix, columns, rows);

  if (rank == 0) {
    std::vector<int> exp_result = rowsSumSeq(testMatrix, columns, rows);
    ASSERT_EQ(result, exp_result);
  }
}

TEST(Matrix_Rows_Sum_MPI, Test_Matrix_10x45) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int columns = 10, rows = 45;

  std::vector<int> testMatrix;
  if (rank == 0) {
    testMatrix = randomMatrix(columns, rows);
  }
  std::vector<int> result;

  result = rowsSum(testMatrix, columns, rows);

  if (rank == 0) {
    std::vector<int> exp_result = rowsSumSeq(testMatrix, columns, rows);
    ASSERT_EQ(result, exp_result);
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

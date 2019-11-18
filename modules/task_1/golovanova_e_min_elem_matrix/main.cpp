// Copyright 2019 Golovanova Elena
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "../../../modules/task_1/golovanova_e_min_elem_matrix/min_elem_matrix.h"

TEST(min_elem_matrix, test_1_Negative_rows) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    ASSERT_ANY_THROW(getRandomMatrix(-10, 10));
  }
}

TEST(min_elem_matrix, test2_Negative_columns) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    ASSERT_ANY_THROW(getRandomMatrix(10, -10));
  }
}

TEST(min_elem_matrix, test3_Positive_value) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> A(20);
  for (int i = 0; i < 20; i++)
    A[i] = i+1;
  int A_min = 1;
  int rez = MinElemMatrix(A, 4, 5);
  if (rank == 0) {
    EXPECT_EQ(rez, A_min);
  }
}

TEST(min_elem_matrix, test4_Negative_value) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> A(9);
  A[0] = -1;
  A[1] = -5;
  A[2] = -8;
  A[3] = -3;
  A[4] = -2;
  A[5] = -7;
  A[6] = -4;
  A[7] = -12;
  A[8] = -2856;
  int A_min = -2856;
  int rez = MinElemMatrix(A, 3, 3);
  if (rank == 0) {
    EXPECT_EQ(rez, A_min);
  }
}

TEST(min_elem_matrix, test5_Mixed_value) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> A(9);
  A[0] = -1;
  A[1] = 895;
  A[2] = -8;
  A[3] = 563;
  A[4] = -79;
  A[5] = 17;
  A[6] = -24;
  A[7] = 52;
  A[8] = -22;
  int A_min = -79;
  int rez = MinElemMatrix(A, 3, 3);
  if (rank == 0) {
    EXPECT_EQ(rez, A_min);
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

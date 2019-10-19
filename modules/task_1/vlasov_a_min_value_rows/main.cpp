// Copyright 2019 Vlasov Andrey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_1/vlasov_a_min_value_rows/min_value_rows.h"

TEST(min_value_rows_MPI, test1_negative_size_rows_on_matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    ASSERT_ANY_THROW(getRandomMatrix(-10, 10));
  }
}

TEST(min_value_rows_MPI, test1_negative_size_columns_on_matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    ASSERT_ANY_THROW(getRandomMatrix(10, -10));
  }
}

TEST(min_value_rows_MPI, test3_positive_values) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> M(25);
  for (int i = 0; i < 25; i++)
    M[i] = i;
  std::vector<int> M_min = { 0, 5, 10, 15, 20 };
  std::vector<int> rez = MinValuesRowsParallel(M, 5, 5);
  if (rank == 0) {
    EXPECT_EQ(rez, M_min);
  }
}

TEST(min_value_rows_MPI, test4_mixed_values) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> M(12);
  M[0] = -2;
  M[1] = -1;
  M[2] = 5;
  M[3] = 7;
  M[4] = 1;
  M[5] = 10;
  M[6] = 3;
  M[7] = -1;
  M[8] = -5;
  M[9] = 3;
  M[10] = 2;
  M[11] = 7;
  std::vector<int> M_min = { -2, 1, -5, 2 };
  std::vector<int> rez = MinValuesRowsParallel(M, 4, 3);
  if (rank == 0) {
    EXPECT_EQ(rez, M_min);
  }
}

TEST(min_value_rows_MPI, test5_negative_values) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> M(12);
  M[0] = -5;
  M[1] = -100;
  M[2] = -30;
  M[3] = -90;
  M[4] = -10;
  M[5] = -1;
  M[6] = -70;
  M[7] = -1;
  M[8] = -500;
  M[9] = -3;
  M[10] = -20;
  M[11] = -7;
  std::vector<int> M_min = { -100, -90, -500, -20 };
  std::vector<int> rez = MinValuesRowsParallel(M, 4, 3);
  if (rank == 0) {
    EXPECT_EQ(rez, M_min);
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

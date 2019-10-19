// Copyright 2019 Tanskii Yuriy
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./min_str.h"



TEST(Parallel_Operations_MPI, Test_parallel_solve_small) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int m = 5;
  const int n = 5;

  if (rank == 0) {
    global_vec = getRandomMatrix(m, n);
  }
  std::vector<int> vec_min_val = getParallelMinInMatrix(global_vec, m, n);
  if (rank == 0) {
  std::vector<int> reference_vec = getMinValInMatrix(global_vec, m, n);
  ASSERT_EQ(reference_vec, vec_min_val);
  }
}
TEST(Parallel_Operations_MPI, Test_parallel_solve_large) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int m = 100;
  const int n = 100;
  if (rank == 0) {
    global_vec = getRandomMatrix(m, n);
  }
  std::vector<int> vec_min_val = getParallelMinInMatrix(global_vec, m, n);
  if (rank == 0) {
    std::vector<int> reference_vec = getMinValInMatrix(global_vec, m, n);
    ASSERT_EQ(reference_vec, vec_min_val);
  }
}
TEST(Parallel_Operations_MPI, Test_Correct_Solve) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int m = 4;
  const int n = 4;
  if (rank == 0) {
  std::vector<int> global_vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
  std::vector<int> vec_min_val = getMinValInMatrix(global_vec, m, n);
  std::vector<int> reference_vec = { 1, 5, 9, 13 };
  ASSERT_EQ(reference_vec, vec_min_val);
  }
}
TEST(Parallel_Operations_MPI, Test_size_below_zero) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int m = 4;
  const int n = -3;
  if (rank == 0) {
    ASSERT_ANY_THROW(global_vec = getRandomMatrix(m, n));
  }
}
TEST(Parallel_Operations_MPI, Test_correct_Size) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int m = 4;
  const int n = 5;
  size_t matr_size = m * n;
  if (rank == 0) {
    global_vec = getRandomMatrix(m, n);
  }
  if (rank == 0) {
    ASSERT_EQ(matr_size, global_vec.size());
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


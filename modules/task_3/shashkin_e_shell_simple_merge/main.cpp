// Copyright 2019 Shashkin Evgeny
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "../../task_3/shashkin_e_shell_simple_merge/shell_simple_merge.h"

TEST(Shell_simple_merge, test_on_const_vector_equal) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector <int> tmp(10);
  std::vector <int> res = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  std::vector <int> res_parallel(10);
  tmp[0] = 9;
  tmp[1] = 8;
  tmp[2] = 7;
  tmp[3] = 3;
  tmp[4] = 5;
  tmp[5] = 6;
  tmp[6] = 4;
  tmp[7] = 2;
  tmp[8] = 0;
  tmp[9] = 1;
  res_parallel = ShellSimpleMerge(tmp, 10);
  if (rank == 0) {
    EXPECT_EQ(res_parallel, res);
  }
}

TEST(Shell_simple_merge, test_on_const_vector_with_negative_numbers_equal) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector <int> tmp(10);
  std::vector <int> res = { -5, -4, -3, -2, -1, 0, 1, 2, 3, 4 };
  std::vector <int> res_parallel(10);
  tmp[0] = 4;
  tmp[1] = 3;
  tmp[2] = 2;
  tmp[3] = -2;
  tmp[4] = 0;
  tmp[5] = 1;
  tmp[6] = -1;
  tmp[7] = -3;
  tmp[8] = -5;
  tmp[9] = -4;
  res_parallel = ShellSimpleMerge(tmp, 10);
  if (rank == 0) {
    EXPECT_EQ(res_parallel, res);
  }
}

TEST(Shell_simple_merge, test_on_const_vector_not_equal) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector <int> tmp(10);
  std::vector <int> res = { 1, 0, 2, 3, 4, 5, 6, 7, 8, 9 };
  std::vector <int> res_parallel(10);
  tmp[0] = 9;
  tmp[1] = 8;
  tmp[2] = 7;
  tmp[3] = 3;
  tmp[4] = 5;
  tmp[5] = 6;
  tmp[6] = 4;
  tmp[7] = 2;
  tmp[8] = 0;
  tmp[9] = 1;
  res_parallel = ShellSimpleMerge(tmp, 10);
  if (rank == 0) {
    EXPECT_NE(res_parallel, res);
  }
}

TEST(Shell_simple_merge, test_on_const_vector_with_negative_numbers_not_equal) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector <int> tmp(10);
  std::vector <int> res = { -4, -5, -3, -2, -1, 0, 1, 2, 3, 4 };
  std::vector <int> res_parallel(10);
  tmp[0] = 4;
  tmp[1] = 3;
  tmp[2] = 2;
  tmp[3] = -2;
  tmp[4] = 0;
  tmp[5] = 1;
  tmp[6] = -1;
  tmp[7] = -3;
  tmp[8] = -5;
  tmp[9] = -4;
  res_parallel = ShellSimpleMerge(tmp, 10);
  if (rank == 0) {
    EXPECT_NE(res_parallel, res);
  }
}

TEST(Shell_simple_merge, test_on_random_vector_with_size_100_qeual) {
  int size = 100;
  std::vector <int> tmp = GetRandomVector(size);
  std::vector <int> res_parallel(size);
  std::vector <int> res_seq(size);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  /*double start_parallel = MPI_Wtime();*/
  res_parallel = ShellSimpleMerge(tmp, size);
  /*double end_parallel = MPI_Wtime();*/

  if (rank == 0) {
    /*double start_seq = MPI_Wtime();*/
    res_seq = ShellSort(tmp, size);
    /*double end_seq = MPI_Wtime();*/
    EXPECT_EQ(res_parallel, res_seq);
    /*std::cout << "Sequential: " << end_seq - start_seq << "\n";
    std::cout << "Parallel: " << end_parallel - start_parallel << "\n";*/
  }
}

TEST(Shell_simple_merge, test_on_random_vector_with_size_100_qeual_not_equal) {
  int size = 100;
  std::vector <int> tmp = GetRandomVector(size);
  std::vector <int> res_vector_multy(size);
  std::vector <int> res_seq(size);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  res_vector_multy = ShellSimpleMerge(tmp, size);

  if (rank == 0) {
    res_seq = ShellSort(tmp, size);
    res_seq[size - 1] = -1;
    EXPECT_NE(res_vector_multy, res_seq);
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

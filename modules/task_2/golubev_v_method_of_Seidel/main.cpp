// Copyright 2019 Golubev Vladislav
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <numeric>
#include <vector>
#include "./method_of_Seidel.h"

TEST(method_of_Seidel, test_of_evkl_norm) {
  const int size = 7;
  std::vector<double> test(size);
  std::iota(test.begin(), test.end(), 0);
  ASSERT_NEAR(EvklNorm(test), sqrt(91), 1e-5);
}

TEST(method_of_Seidel, sequental_method) {
  const int size = 3;
  std::vector<std::vector<double> > A(size);
  for (int i = 0; i < size; ++i) {
    A[i] = std::vector<double>(size);
  }
  std::vector<double> b(size);
  std::vector<double> x(size);
  std::vector<double> tmp(size, 1);
  A[0][0] = 10;
  A[0][1] = 1;
  A[0][2] = 1;
  A[1][0] = 2;
  A[1][1] = 10;
  A[1][2] = 1;
  A[2][0] = 2;
  A[2][1] = 2;
  A[2][2] = 10;
  b[0] = 12;
  b[1] = 13;
  b[2] = 14;
  x = SequentialZeidel(A, b, size, 1e-5);
  ASSERT_NEAR(EvklNorm(x), EvklNorm(tmp), 1e-5);
}

TEST(method_of_Seidel, parallel_method_1) {
  const int size = 3;
  std::vector<std::vector<double> > A(size);
  for (int i = 0; i < size; ++i) {
    A[i] = std::vector<double>(size);
  }
  std::vector<double> b(size);
  std::vector<double> x(size);
  std::vector<double> tmp(size, 1);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    A[0][0] = 10;
    A[0][1] = 1;
    A[0][2] = 1;
    A[1][0] = 2;
    A[1][1] = 10;
    A[1][2] = 1;
    A[2][0] = 2;
    A[2][1] = 2;
    A[2][2] = 10;
    b[0] = 12;
    b[1] = 13;
    b[2] = 14;
  }
  x = ParallelZeidel(A, b, size, 1e-5);
  if (rank == 0) {
    ASSERT_NEAR(EvklNorm(x), EvklNorm(tmp), 1e-5);
  }
}

TEST(method_of_Seidel, parallel_method_2) {
  const int size = 3;
  std::vector<std::vector<double> > A(size);
  for (int i = 0; i < size; ++i) {
    A[i] = std::vector<double>(size);
  }
  std::vector<double> b(size);
  std::vector<double> x(size);
  std::vector<double> tmp(size, 1);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    A[0][0] = 7.6;
    A[0][1] = 0.5;
    A[0][2] = 2.4;
    A[1][0] = 2.2;
    A[1][1] = 9.1;
    A[1][2] = 4.4;
    A[2][0] = -1.3;
    A[2][1] = 0.2;
    A[2][2] = 5.8;
    b[0] = 1.9;
    b[1] = 9.7;
    b[2] = 1.4;
  }
  x = ParallelZeidel(A, b, size, 1e-5);
  if (rank == 0) {
    tmp[0] = 0.11489;
    tmp[1] = 0.924408;
    tmp[2] = 0.235256;
    ASSERT_NEAR(EvklNorm(x), EvklNorm(tmp), 1e-5);
  }
}

TEST(method_of_Seidel, comparison_of_parallel_and_sequental_methods) {
  const int size = 20;
  const int low = 0;
  const int high = 100;
  std::vector<std::vector<double> > A(size);
  for (int i = 0; i < size; ++i) {
    A[i] = std::vector<double>(size);
  }
  std::vector<double> b(size);
  std::vector<double> sequential(size);
  std::vector<double> parallel(size);
  std::vector<double> tmp(size, 1);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    std::random_device rd;
    std::mt19937 k(rd());
    double sum = 0;
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        A[i][j] = static_cast<double>(low) + k() % (high - low);
        sum += A[i][j];
      }
      A[i][i] = sum + low + k() % (high - low);;
      b[i] = static_cast<double>(low) + k() % (high - low);
      sum = 0;
    }
    sequential = SequentialZeidel(A, b, size, 1e-5);
  }
  parallel = ParallelZeidel(A, b, size, 1e-5);
  if (rank == 0) {
    ASSERT_NEAR(EvklNorm(sequential), EvklNorm(parallel), 1e-5);
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

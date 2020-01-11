// Copyright 2019 Pauzin Leonid
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include "./pauzin_l_batcher_shell.h"


TEST(Batcher_Sort, Can_Throw_Assert) {
  std::vector<int> testVec(0);
  const int sizeVec = testVec.size();
  ASSERT_ANY_THROW(batcherParallel(testVec, sizeVec));
}

TEST(Batcher_Sort, Can_Not_Throw_Assert) {
  std::vector<int> testVec(10);
  const int sizeVec = testVec.size();
  ASSERT_NO_THROW(batcherParallel(testVec, sizeVec));
}


TEST(Batcher_Sort, Test_On_Big_Data) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> testVec;
  const int vecSize = 1000000;
  if (rank == 0) {
    testVec = getRandomVector(vecSize);
  }
  std::vector<int> batchVec = batcherParallel(testVec, vecSize);
  if (rank == 0) {
    testVec = ShellSort(testVec);
    ASSERT_EQ(batchVec, testVec);
  }
}

TEST(Batcher_Sort, Test_On_Simple_Vec) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int vecSize = 10;
  std::vector<int> testVec(vecSize);
  if (rank == 0) {
    testVec = { 55, 36, 37, 28, 49, 90, 3, 12, 19, 14 };
  }
  std::vector<int> batchVec = batcherParallel(testVec, vecSize);
  if (rank == 0) {
    testVec = ShellSort(testVec);
    ASSERT_EQ(batchVec, testVec);
  }
}

TEST(Batcher_Sort, Test_On_Not_Equal) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int vecSize = 10;
  std::vector<int> testVec(vecSize);
  if (rank == 0) {
    testVec = { 55, 36, 37, 28, 49, 90, 3, 12, 19, 14 };
  }
  std::vector<int> batchVec = batcherParallel(testVec, vecSize);
  if (rank == 0) {
    testVec = ShellSort(testVec);
    testVec[0] = -1;
    ASSERT_NE(batchVec, testVec);
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

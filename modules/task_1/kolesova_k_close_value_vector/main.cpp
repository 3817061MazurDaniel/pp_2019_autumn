// Copyright 2019 Kolesova Kristina
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <random>
#include "../../../modules/task_1/kolesova_k_close_value_vector/close_value_vector.h"

TEST(Test_close_value_vector, Test_close_value_one) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> Vec = { 0, 6, 8, 16, 20, 30 };
  if (rank == 0) {
    int dif = getMinDifferenceOneProc(Vec);
    ASSERT_EQ(dif, 2);
  }
}

TEST(Test_close_value_vector, Test_close_value_one_oneElem) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> Vec = { 3 };
  if (rank == 0) {
    int dif = getMinDifferenceOneProc(Vec);
    ASSERT_EQ(dif, 0);
  }
}

TEST(Test_close_value_vector, Test_close_value_oneElem) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> Vec;
  Vec = getRandomVector(1);
  int dif = getMinDifference(Vec);
  if (rank == 0) {
    ASSERT_EQ(dif, getMinDifferenceOneProc(Vec));
  }
}

TEST(Test_close_value_vector, Test_close_value_10Elem) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> Vec;
  Vec = getRandomVector(10);
  int dif = getMinDifference(Vec);
  if (rank == 0) {
    ASSERT_EQ(dif, getMinDifferenceOneProc(Vec));
  }
}

TEST(Test_close_value_vector, Test_close_value_50Elem) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> Vec;
  Vec = getRandomVector(50);
  int dif = getMinDifference(Vec);
  if (rank == 0) {
    ASSERT_EQ(dif, getMinDifferenceOneProc(Vec));
  }
}

TEST(Test_close_value_vector, Test_close_value_100Elem) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> Vec;
  Vec = getRandomVector(100);
  int dif = getMinDifference(Vec);
  if (rank == 0) {
    ASSERT_EQ(dif, getMinDifferenceOneProc(Vec));
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

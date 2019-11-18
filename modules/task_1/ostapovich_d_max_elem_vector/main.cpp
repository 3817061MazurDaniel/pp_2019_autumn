// Copyright 2019 Ostapovich Denis

#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <vector>
#include "./max_elem_vector.h"

TEST(Vector_Max_MPI, Test_GetVector_Bad_Mode) {
  ASSERT_ANY_THROW(GetVector(10, 10));
}

TEST(Vector_Max_MPI, Test_GetVector_VMAX_POSITIVE) {
  ASSERT_NO_THROW(GetVector(10, VMAX_POSITIVE));
}

TEST(Vector_Max_MPI, Test_GetVector_VMAX_NEGATIVE) {
  ASSERT_NO_THROW(GetVector(10, VMAX_NEGATIVE));
}

TEST(Vector_Max_MPI, Test_GetVector_VMAX_MIXED) {
  ASSERT_NO_THROW(GetVector(10, VMAX_MIXED));
}

TEST(Vector_Max_MPI, Test_Manual_Input_Max_EQ) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vector = { 1, 2, 3, 4, 5 };
  int max = ParallelMaxValue(vector);
  if (rank == 0) {
    ASSERT_EQ(5, max);
  }
}

TEST(Vector_Max_MPI, Test_Max_EQ) {
  int worldRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

  int size = 250;
  std::vector<int> vector = GetVector(size, VMAX_MIXED);
  int calculatedMax = ParallelMaxValue(vector);
  int expectedMax = SequentialMaxValue(vector);

  if (worldRank == 0) {
    ASSERT_EQ(expectedMax, calculatedMax);
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

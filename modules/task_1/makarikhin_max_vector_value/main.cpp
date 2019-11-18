// Copyright 2019 Makarikhin Semen
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "./max_vector_value.h"

TEST(Parallel_max_vector_value, Test_Max_Value) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int size_vector = 100;

  if (rank == 0) {
    global_vec = getRandomVector(size_vector);
  }

  int global_max = GetMaxVectorValueParallel(global_vec, size_vector);

  if (rank == 0) {
    int seqential_max = *std::max_element(global_vec.begin(), global_vec.end());
    ASSERT_EQ(seqential_max, global_max);
  }
}

TEST(Parallel_max_vector_value, Test_Vector_With_Given_Values) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> global_vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  int global_max = GetMaxVectorValueParallel(global_vec, global_vec.size());

  if (rank == 0) {
    ASSERT_EQ(global_max, 9);
  }
}

TEST(Parallel_max_vector_value, Test_Vector_With_Same_Values) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> global_vec = { 2, 2, 2, 2, 2, 2, 2, 2, 2 };

  int global_max = GetMaxVectorValueParallel(global_vec, global_vec.size());

  if (rank == 0) {
    ASSERT_EQ(global_max, 2);
  }
}

TEST(Parallel_max_vector_value, Test_Odd_Size_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int size_vector = 225;

  if (rank == 0) {
    global_vec = getRandomVector(size_vector);
  }

  int global_max = GetMaxVectorValueParallel(global_vec, size_vector);

  if (rank == 0) {
    int seqential_max = *std::max_element(global_vec.begin(), global_vec.end());
    ASSERT_EQ(seqential_max, global_max);
  }
}

TEST(Parallel_max_vector_value, Test_Empty_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;

  if (rank == 0) {
    EXPECT_THROW(GetMaxVectorValueParallel(global_vec, global_vec.size()), std::runtime_error);
  }
}

TEST(Parallel_max_vector_value, Test_Big_Size_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int size_vector = 1000000;

  if (rank == 0) {
    global_vec = getRandomVector(size_vector);
  }

  int global_max = GetMaxVectorValueParallel(global_vec, size_vector);

  if (rank == 0) {
    int seqential_max = *std::max_element(global_vec.begin(), global_vec.end());
    ASSERT_EQ(seqential_max, global_max);
  }
}

int main(int argc, char **argv) {
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

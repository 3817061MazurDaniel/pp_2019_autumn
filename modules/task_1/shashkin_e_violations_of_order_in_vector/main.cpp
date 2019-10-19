// Copyright 2019 Shashkin Evgeny
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "../../../modules/task_1/shashkin_e_violations_of_order_in_vector/violations_of_order_in_vector.h"

TEST(Count_of_violations_of_order_in_vec, Test_on_empty_vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec = GetRandomVector(0);

  int parallel_count = GetCountOfViolationsOfOrderInVectorParallel(vec, 0);

  if (rank == 0) {
    ASSERT_EQ(parallel_count, 0);
  }
}

TEST(Count_of_violations_of_order_in_vec, Test_on_const_ordered_vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec(9);

  for (size_t i = 0; i < vec.size(); ++i) {
    vec[i] = i;
  }

  int parallel_count = GetCountOfViolationsOfOrderInVectorParallel(vec, 9);

  if (rank == 0) {
    ASSERT_EQ(parallel_count, 0);
  }
}

TEST(Count_of_violations_of_order_in_vec, Test_on_const_unordered_vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec = { 5, 3, 3, 4, 6, 9, 7, 1, 9 };

  int parallel_count = GetCountOfViolationsOfOrderInVectorParallel(vec, 9);

  if (rank == 0) {
    ASSERT_EQ(parallel_count, 3);
  }
}

TEST(Count_of_violations_of_order_in_vec, Test_on_random_vector_with_2_elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec;

  size_t size = 2;

  if (rank == 0) {
    vec = GetRandomVector(size);
  }

  int parallel_count = GetCountOfViolationsOfOrderInVectorParallel(vec, size);

  if (rank == 0) {
    int seqential_count = GetCountOfViolationsOfOrderInVector(vec);
    ASSERT_EQ(parallel_count, seqential_count);
  }
}

TEST(Count_of_violations_of_order_in_vec, Test_on_random_vector_with_50_elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec;

  size_t size = 50;

  if (rank == 0) {
    vec = GetRandomVector(size);
  }

  int parallel_count = GetCountOfViolationsOfOrderInVectorParallel(vec, size);

  if (rank == 0) {
    int seqential_count = GetCountOfViolationsOfOrderInVector(vec);
    ASSERT_EQ(parallel_count, seqential_count);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}

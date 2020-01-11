// Copyright 2019 Zhbanova Nadezhda

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./most_diff_vectors_elems.h"

TEST(most_diff_vectors_elems, min_vector) {
  int rank;
  int num_of_elem = 2;
  std::vector<int> v(num_of_elem);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    v = randomInput(num_of_elem);
  }

  double res = maxDiffMpi(v);

  if (rank == 0) {
    ASSERT_EQ(maxDiff(v), res);
  }
}

TEST(most_diff_vectors_elems, small_vector) {
  int rank;
  int num_of_elem = 7;
  std::vector<int> v(num_of_elem);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    v = randomInput(num_of_elem);
  }

  double res = maxDiffMpi(v);

  if (rank == 0) {
    ASSERT_EQ(maxDiff(v), res);
  }
}

TEST(most_diff_vectors_elems, medium_vector) {
  int rank;
  int num_of_elem = 100;
  std::vector<int> v(num_of_elem);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    v = randomInput(num_of_elem);
  }

  double res = maxDiffMpi(v);

  if (rank == 0) {
    ASSERT_EQ(maxDiff(v), res);
  }
}

TEST(most_diff_vectors_elems, big_vector) {
  int rank;
  int num_of_elem = 500;
  std::vector<int> v(num_of_elem);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    v = randomInput(num_of_elem);
  }

  double res = maxDiffMpi(v);

  if (rank == 0) {
    ASSERT_EQ(maxDiff(v), res);
  }
}

TEST(most_diff_vectors_elems, not_allowed_vector) {
  int rank;
  int num_of_elem = 1;
  std::vector<int> v(num_of_elem);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  // vector filled only for 0 process
  if (rank == 0) {
    v = randomInput(num_of_elem);
  }

  ASSERT_ANY_THROW(maxDiffMpi(v));
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



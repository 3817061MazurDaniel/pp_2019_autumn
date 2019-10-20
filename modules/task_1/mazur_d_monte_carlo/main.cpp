// Copyright 2019 Mazur Daniil
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./monte_carlo.h"

double example(double x) {
  return 0.01 * x * x + sin(2 * x) + 3;
}

TEST(int_monte_carlo, throws_when_n_points_is_zero) {
  ASSERT_ANY_THROW(getIntegral(0, example, 2.0, 5.0));
}

TEST(int_monte_carlo, integral_of_example_with_10000_points) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double res = getIntegral(10000, example, 0.0, 6.0);
  if (rank == 0) {
    ASSERT_NEAR(res, 18.07980730208778, 1);
  }
}

TEST(int_monte_carlo, integral_of_example_with_10_points) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double res = getIntegral(10000, example, 0.0, 6.0);
  if (rank == 0) {
    ASSERT_NEAR(res, 18.07980730208778, 1);
  }
}

TEST(int_monte_carlo, integral_of_example_with_0_interval) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double res = getIntegral(10000, example, 5.3, 5.3);
  if (rank == 0) {
  ASSERT_NEAR(res, 0, 1);
  }
}

TEST(int_monte_carlo, integral_of_example_with_large_interval) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double res = getIntegral(100000, example, -50.0, 50.0);
  if (rank == 0) {
  ASSERT_NEAR(res, 1133.333333333333, 300);
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

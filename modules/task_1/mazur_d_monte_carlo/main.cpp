// Copyright 2019 Mazur Daniil
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cmath>
#include "./monte_carlo.h"

double example1(double x) {
  return sin(x);
}

double example2(double x) {
  return x*x;
}

double example3(double x) {
  return x*x*x + 2*x;
}

double example4(double x) {
  return 0.01 * x * x + sin(x);
}

TEST(int_monte_carlo, throws_when_n_points_is_zero) {
  ASSERT_ANY_THROW(soloIntegral(0, example1, 2.0, 5.0));
}

TEST(int_monte_carlo, integral_of_example1_with_100000_points) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double parResult = parIntegral(100000, example1, 0, 3.141592653589/2);
  if (rank == 0) {
    double soloResult = soloIntegral(100000, example1, 0, 3.141592653589 / 2);
    std::cout << soloResult << '\n';
    std::cout << parResult << '\n';
    ASSERT_NEAR(parResult, soloResult, 0.05);
  }
}

TEST(int_monte_carlo, integral_of_example2_with_100000_points) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double parResult = parIntegral(100000, example2, 0, 3.141592653589 / 2);
  if (rank == 0) {
    double soloResult = soloIntegral(100000, example2, 0, 3.141592653589 / 2);
    std::cout << soloResult << '\n';
    std::cout << parResult << '\n';
    ASSERT_NEAR(parResult, soloResult, 0.05);
  }
}

TEST(int_monte_carlo, integral_of_example3_with_100000_points) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double parResult = parIntegral(100000, example3, 0, 3.141592653589 / 2);
  if (rank == 0) {
    double soloResult = soloIntegral(100000, example3, 0, 3.141592653589 / 2);
    std::cout << soloResult << '\n';
    std::cout << parResult << '\n';
    ASSERT_NEAR(parResult, soloResult, 0.05);
  }
}

TEST(int_monte_carlo, integral_of_example4_with_100000_points) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double parResult = parIntegral(100000, example4, 0, 3.141592653589 / 2);
  if (rank == 0) {
    double soloResult = soloIntegral(100000, example4, 0, 3.141592653589 / 2);
    std::cout << soloResult << '\n';
    std::cout << parResult << '\n';
    ASSERT_NEAR(parResult, soloResult, 0.05);
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

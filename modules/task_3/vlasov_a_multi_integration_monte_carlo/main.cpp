// Copyright 2019 Vlasov Andrey
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <mpi.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <iostream>
#include "./multi_integration_monte_carlo.h"

double f1(std::vector<double> x) {
  return x[0] * x[0];
}

double f2(std::vector<double> x) {
  return 3 * x[0] * x[0] * x[0] + 2 * x[1] * x[1];
}

double f3(std::vector<double> x) {
  return sin(x[0]) + 2 * x[1] + x[2] * x[2];
}

double f4(std::vector<double> x) {
  return x[0] * x[0] + 2 * x[1] - cos(x[2]) + 2 * x[3] * x[3] * x[3] - 3 * x[4];
}

TEST(multi_integration_monte_carlo, test1_n_is_negative) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double a = 0.0, b = 3.0;
  if (rank == 0) {
    ASSERT_ANY_THROW(getIntegralMonteCarloSequential(f1, { a }, { b }, -1000));
  }
}

TEST(multi_integration_monte_carlo, test2_multiplicity_is_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double a = 0.0, b = 1.0;
  double res = getIntegralMonteCarloParallel(f1, { a }, { b }, 1000000);
  if (rank == 0) {
    double res1 = getIntegralMonteCarloSequential(f1, { a }, { b }, 1000000);
    ASSERT_NEAR(res, res1, 0.05);
  }
}

TEST(multi_integration_monte_carlo, test3_multiplicity_is_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double>a = { 0.0, 2.5 };
  std::vector<double>b = { 1.534, 3.12 };
  double res = getIntegralMonteCarloParallel(f2, a, b, 1000000);
  if (rank == 0) {
    double res1 = getIntegralMonteCarloSequential(f2, a, b, 1000000);
    ASSERT_NEAR(res, res1, 0.05);
  }
}

TEST(multi_integration_monte_carlo, test4_multiplicity_is_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double>a = { 0.0, 2.5, 1.234 };
  std::vector<double>b = { 1.534, 3.12, 1.555 };
  double res = getIntegralMonteCarloParallel(f3, a, b, 1000000);
  if (rank == 0) {
    double res1 = getIntegralMonteCarloSequential(f3, a, b, 1000000);
    ASSERT_NEAR(res, res1, 0.05);
  }
}

TEST(multi_integration_monte_carlo, test5_multiplicity_is_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double>a = { 0.0, -2.5, 1.5, -5.0 };
  std::vector<double>b = { 1.0, -1.0, 2.5, -3.0 };
  double res = getIntegralMonteCarloParallel(f3, a, b, 1000000);
  if (rank == 0) {
    double res1 = getIntegralMonteCarloSequential(f3, a, b, 1000000);
    ASSERT_NEAR(res, res1, 0.05);
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

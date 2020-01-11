// Copyright 2019 Golubev Vladislav
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "./global_optimization_search_area.h"

double f1(double* x) {
  return exp(-0.5 * *x) * sin(6 * *x - 1.5);
}
double f(double* x) {
  return *x * *x;
}
double testF(double* _x) {
  double x = *_x;
  return cos(18 * x - 3) * sin(10 * x - 7) + 1.5;
}

double f_xy(double* x) {
  return x[0] + x[1];
}

double testF_xy(double* y) {
  return -1.5 * pow(y[0], 2) * exp(1 - y[0] * y[0] - 20.25 * pow((y[0] - y[1]), 2))
    - pow((0.5 * (y[0] - 1) * (y[1] - 1)), 4) *
    exp(2 - pow((0.5 * (y[0] - 1)), 4) - pow((y[1] - 1), 4));
}

double testFxy(double* x) {
  return x[0] * x[0] + x[1] * x[1];
}

TEST(global_optimization, sequential_global_min_at_xy) {
  const int maxIterations = 1000;
  Optimization opt(1, 0.6, 2.2, testF, 1e-5);
  double res = opt.SequentialGlobalSearchXY(maxIterations);
  ASSERT_NEAR(res, 2.093, 1e-3);
}

TEST(global_optimization, parallel_global_min_at_xy) {
  const int maxIterations = 1000;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Optimization opt(1, 0.6, 2.2, testF, 1e-5);
  double res = opt.ParallelGlobalSearchXY(maxIterations);
  if (rank == 0) {
    ASSERT_NEAR(res, 2.093, 1e-2);
  }
}

TEST(global_optimization, parallel_global_min_with_easy_function) {
  const int maxIterations = 1000;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Optimization opt(1, -6, 10, f, 1e-5);
  double res = opt.ParallelGlobalSearchXY(maxIterations);
  if (rank == 0) {
    ASSERT_NEAR(res, 0, 1e-2);
  }
}

TEST(global_optimization, sequential_global_min_at_xyz) {
  const int maxIterations = 1000;
  double* x = new double[2];
  Optimization opt(2, -5, 5, testFxy, 1e-5, -5, 5);
  x = opt.SequentialGlobalSearchXYZ(maxIterations);
  ASSERT_NEAR(testFxy(x), 0, 1e-3);
}

TEST(global_optimization, parallel_global_min_at_xyz) {
  const int maxIterations = 1000;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double* x = new double[2];
  Optimization opt(2, -1, 2, testFxy, 1e-3, -4, 4);
  x = opt.ParallelGlobalSearchXYZ(maxIterations);
  if (rank == 0) {
    ASSERT_NEAR(testFxy(x), 0, 0.01);
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

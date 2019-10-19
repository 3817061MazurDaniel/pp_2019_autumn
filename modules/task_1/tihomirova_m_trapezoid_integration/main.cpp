// Copyright 2019 Tihomirova Mariya
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "../../../modules/task_1/tihomirova_m_trapezoid_integration/trapezoid_integration.h"

double polynom(double x) {
  return x * x + 1;
}

double polynom1(double x) {
  return x * x - 2;
}

TEST(trapezoid_integration, test1_throws_when_n_is_negative) {
  ASSERT_ANY_THROW(Parallelization(cos, 0.0, 10.0, -100));
}

TEST(trapezoid_integration, test2_integral_of_trigonometric_function1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double rez = Parallelization(sin, 0.0, 3.141, 1000);
  if (rank == 0) {
    ASSERT_NEAR(rez, 2.0, 1e-4);
  }
}

TEST(trapezoid_integration, test3_integral_of_trigonometric_function2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double rez = Parallelization(cos, 0, 3.14159, 1000);
  if (rank == 0) {
    ASSERT_NEAR(rez, 0.0, 1e-4);
  }
}

TEST(trapezoid_integration, test4_integral_of_polynom) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double rez = Parallelization(polynom, 0, 2.0, 1000);
  if (rank == 0) {
    ASSERT_NEAR(rez, 4.66667, 1e-4);
  }
}

TEST(trapezoid_integration, test5_integral_of_polynom_is_negative) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double rez = Parallelization(polynom1, 0.0, 1.0, 1000);
  if (rank == 0) {
    ASSERT_NEAR(rez, -1.66667, 1e-4);
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

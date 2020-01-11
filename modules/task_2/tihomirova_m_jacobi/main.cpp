// Copyright 2019 Tihomirova Mariya
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <mpi.h>
#include <vector>
#include <iostream>
#include "./jacobi.h"

TEST(jacobi_MPI, test1_A_Size_is_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector <double> A(9);
  std::vector <double> b(3);
  double eps = 0.0001;
  if (rank == 0) {
    A = { 50, -7, 9, -3, 47, -13, 6, 1, 37 };
    b = { 17, 14, 23 };
  }
  std::vector<double> result = Parallel(A, b, 3, eps);
  if (rank == 0) {
    double error = getNorm(A, result, b);
    ASSERT_NEAR(error, 0, 0.001);
  }
}

TEST(jacobi_MPI, test2_A_Size_is_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector <double> A(9);
  std::vector <double> b(3);
  double eps = 0.0001;
  if (rank == 0) {
    A = { 47, -9, -12, 15, 50, -9, 2, -3, 39 };
    b = { 15, 19, 17 };
  }
  std::vector<double> result = Parallel(A, b, 3, eps);
  if (rank == 0) {
    double error = getNorm(A, result, b);
    ASSERT_NEAR(error, 0, 0.001);
  }
}

TEST(jacobi_MPI, test3_A_Size_is_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector <double> A(9);
  std::vector <double> b(3);
  double eps = 0.0001;
  if (rank == 0) {
    A = { 5.5, 0.7, -0.67, 1.6, 7.2, 1.3, -2, -3, 6.3 };
    b = { 3.1, 2.4, 1.7 };
  }
  std::vector<double> result = Parallel(A, b, 3, eps);
  if (rank == 0) {
    double error = getNorm(A, result, b);
    ASSERT_NEAR(error, 0, 0.001);
  }
}

TEST(jacobi_MPI, test4_A_Size_is_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector <double> A(9);
  std::vector <double> b(3);
  double eps = 0.00001;
  if (rank == 0) {
    A = { 337, 70, -50, 90, 555, -85, -20, 37, 450 };
    b = { 127, 32, 128 };
  }
  std::vector<double> result = Parallel(A, b, 3, eps);
  if (rank == 0) {
    double error = getNorm(A, result, b);
    ASSERT_NEAR(error, 0, 0.001);
  }
}

TEST(jacobi_MPI, test5_A_Size_is_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector <double> A(16);
  std::vector <double> b(4);
  double eps = 0.0001;
  if (rank == 0) {
    A = { 50, -7, 9, -3, 7 , 47, -13, 6, 1, 3, 77, 8, 2, 3, 5, 33 };
    b = { 17, 14, 23, 5 };
  }
  std::vector<double> result = Parallel(A, b, 4, eps);
  if (rank == 0) {
    double error = getNorm(A, result, b);
    ASSERT_NEAR(error, 0, 0.001);
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

// Copyright 2019 Karin Timofey
#include <stdlib.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cmath>
#include "./rec_int.h"

double f1(double x) {
  return 3;
}

double f2(double x) {
  return x;
}

double f3(double x) {
  return 2*x+(3/std::sqrt(x));
}

double f4(double x) {
  return (std::sin(2/x));
}

double f5(double x) {
  return 1 * x * x + 4 * x - 5;
}

TEST(Rec_int, negative_rectangles_number) {
  ASSERT_ANY_THROW(ParIntegration(f1, 1, 2, -3));
}

TEST(Rect_int, can_change_a_and_b) {
  double res1;
  double res2;
  double a = 1;
  double b = 5;
  int n = 2000;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  res1 = ParIntegration(f1, a, b, n);
  res2 = ParIntegration(f1, b, a, n);
  if (rank == 0) {
    ASSERT_NEAR(res1, -res2, 1e-6);
  }
}

TEST(Rect_int, correct_with_a_a) {
  double res;
  double a = 1;
  int n = 2000;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  res = ParIntegration(f1, a, a, n);
  if (rank == 0) {
    ASSERT_NEAR(res, 0, 1e-6);
  }
}

TEST(Rect_int, Int_and_ParInt_return_eq_val) {
  double res1;
  double res2;
  double a = 1;
  double b = 5;
  int n = 2000;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  res1 = ParIntegration(f4, a, b, n);
  res2 = Integration(f4, a, b, n);
  if (rank == 0) {
    ASSERT_NEAR(res1, res2, 1e-6);
  }
}


TEST(Rec_int, test1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    double res = Integration(f2, 1, 5, 2000);
    ASSERT_NEAR(12.0, res, 1e-6);
  }
}

TEST(Rec_int, test2) {
  double res;
  double correct = 12;
  double a = 1;
  double b = 5;
  int n = 2000;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  res = ParIntegration(f1, a, b, n);
  if (rank == 0) {
    ASSERT_NEAR(correct, res, 1e-6);
  }
}

TEST(Rec_int, test3) {
  double res;
  double correct = 12.0;
  double a = 1.0;
  double b = 5.0;
  int n = 2000;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  res = ParIntegration(f2, a, b, n);
  if (rank == 0) {
    ASSERT_NEAR(correct, res, 1e-6);
  }
}

TEST(Rec_int, test4) {
  double res;
  double correct = 21.0;
  double a = 1.0;
  double b = 4.0;
  int n = 2000;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  res = ParIntegration(f3, a, b, n);
  if (rank == 0) {
    ASSERT_NEAR(correct, res, 1e-6);
  }
}

TEST(Rec_int, test5) {
  double res;
  double correct = 21.0;
  double a = 1.0;
  double b = 4.0;
  int n = 2000;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  res = ParIntegration(f3, a, b, n);
  if (rank == 0) {
    ASSERT_NEAR(correct, res, 1e-6);
  }
}

  TEST(Rec_int, test6) {
  double res;
  double a = 1.0;
  double b = 4.0;
  double c = -5.0;
  double k1 = GetRand();
  double k2 = GetRand();
  double correct = QuadrFunInt(a, b, c, k1, k2);
  int n = 200000;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  res = ParIntegration(f5, k1, k2, n);
  if (rank == 0) {
    ASSERT_NEAR(correct, res, 1);
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

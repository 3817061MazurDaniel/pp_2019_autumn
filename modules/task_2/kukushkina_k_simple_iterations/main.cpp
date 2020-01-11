// Copyright 2019 Kukushkina Ksenia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <numeric>
#include <vector>
#include "./simple_iterations.h"

TEST(Simple_Iterations, norm_testing) {
  std::vector<double> test = { 0, -7, 4, 6 };
  ASSERT_EQ(Norm(test), 7);
}

TEST(Simple_Iterations, non_equal_dimensions_non_parallel) {
  std::vector<double> A{ 10, 1, 1, 2, 10, 1, 2, 2 };
  std::vector<double> b{ 12, 13, 14 };
  ASSERT_ANY_THROW(Simple_Iterations(A, b, 1e-5));
}

TEST(Simple_Iterations, non_equal_dimensions_parallel) {
  std::vector<double> A{ 10, 1, 1, 2, 10, 1, 2, 2 };
  std::vector<double> b{ 12, 13, 14 };
  ASSERT_ANY_THROW(Simple_Iterations_MPI(A, b, 1e-5));
}

TEST(Simple_Iterations, zero_element_non_parallel) {
  std::vector<double> A{ 10, 1, 1, 2, 10, 1, 2, 2, 0 };
  std::vector<double> b{ 12, 13, 14 };
  ASSERT_ANY_THROW(Simple_Iterations(A, b, 1e-5));
}

TEST(Simple_Iterations, zero_element_parallel) {
  std::vector<double> A{ 10, 1, 1, 2, 10, 1, 2, 2, 0 };
  std::vector<double> b{ 12, 13, 14 };
  ASSERT_ANY_THROW(Simple_Iterations_MPI(A, b, 1e-5));
}

TEST(Simple_Iterations, no_diag_prevalence_non_parallel) {
  std::vector<double> A{ 10, 1, 1, 2, 10, 1, 2, 2, 1 };
  std::vector<double> b{ 12, 13, 14 };
  ASSERT_ANY_THROW(Simple_Iterations(A, b, 1e-5));
}

TEST(Simple_Iterations, no_diag_prevalence_parallel) {
  std::vector<double> A{ 10, 1, 1, 2, 10, 1, 2, 2, 1 };
  std::vector<double> b{ 12, 13, 14 };
  ASSERT_ANY_THROW(Simple_Iterations_MPI(A, b, 1e-5));
}

TEST(Simple_Iterations, non_parallel_1) {
  std::vector<double> A{ 10, 1, 2, 3, 1, 10, 2, 3, 1, 0, 10, 2, 2, 0, 0, 10 };
  std::vector<double> b{ 12, 3, 11, 2 };
  std::vector<double> x(4);
  std::vector<double> tmp{ 1, 0, 1, 0 };
  x = Simple_Iterations(A, b, 1e-12);
  ASSERT_NEAR(Norm(x), Norm(tmp), 1e-12);
}

TEST(Simple_Iterations, non_parallel_2) {
  std::vector<double> A{ 3, 1, 1, 2, 4, 1, 5, 7, 10 };
  std::vector<double> b{ 10, 11, 47 };
  std::vector<double> x(3);
  std::vector<double> tmp{ 2, 1, 3 };
  x = Simple_Iterations(A, b, 1e-7);
  ASSERT_NEAR(Norm(x), Norm(tmp), 1e-7);
}

TEST(Simple_Iterations, non_parallel_3) {
  std::vector<double> A{ 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4 };
  std::vector<double> b{ 10, 13, 16, 10 };
  std::vector<double> x(4);
  std::vector<double> tmp{ 1, 2, 3, 1 };
  x = Simple_Iterations(A, b, 1e-7);
  ASSERT_NEAR(Norm(x), Norm(tmp), 1e-7);
}

TEST(Simple_Iterations, parallel_1) {
  std::vector<double> A{ 10, 1, 1, 2, 10, 1, 2, 2, 10 };
  std::vector<double> b{ 12, 13, 14 };
  std::vector<double> x(3);
  std::vector<double> tmp{ 1, 1, 1 };
  x = Simple_Iterations_MPI(A, b, 1e-12);
  ASSERT_NEAR(Norm(x), Norm(tmp), 1e-12);
}

TEST(Simple_Iterations, parallel_2) {
  std::vector<double> A{ 3, 1, 1, 2, 4, 1, 5, 7, 10 };
  std::vector<double> b{ 10, 11, 47 };
  std::vector<double> x(3);
  std::vector<double> tmp{ 2, 1, 3 };
  x = Simple_Iterations_MPI(A, b, 1e-7);
  ASSERT_NEAR(Norm(x), Norm(tmp), 1e-7);
}

TEST(Simple_Iterations, parallel_3) {
  std::vector<double> A{ 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4 };
  std::vector<double> b{ 10, 13, 16, 10 };
  std::vector<double> x(4);
  std::vector<double> tmp{ 1, 2, 3, 1 };
  x = Simple_Iterations_MPI(A, b, 1e-7);
  ASSERT_NEAR(Norm(x), Norm(tmp), 1e-7);
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

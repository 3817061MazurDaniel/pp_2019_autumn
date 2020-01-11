// Copyright 2019 Babushkin Aleksey

#include <gtest/gtest.h>
#include <mpi.h>

#include <gtest-mpi-listener.hpp>
#include <iostream>
#include <vector>

#include "./sparse_matrix.h"

void test(const int& t_cols, const int& t_rows, const int& t_b_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int cols = t_cols;
  const int rows = t_rows;
  const int b_cols = t_b_cols;

  SparseMatrix A(cols, rows), B(b_cols, cols);
  std::vector<double> result;

  double elapsed_time;

  if (rank == 0) {
    std::vector<double> sparse_matrix_a(randomSparseMatrix(cols, rows, 1, 50));
    std::vector<double> sparse_matrix_b(
        randomSparseMatrix(b_cols, cols, 1, 50));

    A = SparseMatrix(sparse_matrix_a, cols, rows);
    B = SparseMatrix(sparse_matrix_b, b_cols, cols);

    sparse_matrix_a.clear();
    sparse_matrix_b.clear();

    elapsed_time = MPI_Wtime();
  }

  result = multiply(&A, &B);
  if (rank == 0) {
    elapsed_time = MPI_Wtime() - elapsed_time;
    std::cout << "Elapsed time: " << elapsed_time << "s" << std::endl;

    elapsed_time = MPI_Wtime();
    std::vector<double> expected_result = A * B;
    elapsed_time = MPI_Wtime() - elapsed_time;
    std::cout << "Elapsed time_s: " << elapsed_time << "s" << std::endl;

    ASSERT_EQ(result, expected_result);
  }
}

TEST(SPARSE_MATRIX_MULT, SEQUENTIAL) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    SparseMatrix A(std::vector<double>{1.0, 2.0, 3.0},
                   std::vector<int>{2, 1, 2}, std::vector<int>{0, 1, 3, 3}, 3,
                   3),
        B(std::vector<double>{5, 3}, std::vector<int>{0, 1},
          std::vector<int>{0, 1, 2, 2}, 2, 3);

    std::vector<double> result = A * B;
    std::vector<double> exp_result{0, 0, 0, 6, 0, 0};

    ASSERT_EQ(result, exp_result);
  }
}

TEST(SPARSE_MATRIX_MULT, PARALLEL) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  SparseMatrix A(6, 6), B(2, 6);
  std::vector<double> result;

  if (rank == 0) {
    A = SparseMatrix(std::vector<double>{0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 3,
                                         0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0,
                                         0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                     6, 6);
    B = SparseMatrix(std::vector<double>{0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 3}, 2,
                     6);
  }

  result = multiply(&A, &B);

  if (rank == 0) {
    std::vector<double> exp_result{5, 0, 10, 9, 0, 0, 20, 0, 0, 0, 0, 0};
    ASSERT_EQ(result, exp_result);
  }
}

TEST(SPARSE_MATRIX_MULT, PARLL_SIZE_20) { test(20, 10, 20); }
TEST(SPARSE_MATRIX_MULT, PARLL_SIZE_50) { test(50, 50, 50); }
TEST(SPARSE_MATRIX_MULT, PARLL_SIZE_100) { test(100, 100, 100); }

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

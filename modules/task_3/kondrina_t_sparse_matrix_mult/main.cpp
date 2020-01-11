// Copyright 2019 Kondrina Tatyana

#include <gtest/gtest.h>
#include <mpi.h>

#include <gtest-mpi-listener.hpp>
#include <vector>

#include "./sparse_matrix_mult.h"

TEST(CCS_Matrix_mult, seq) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    SparseMatrix A, B;
    A = CCS(std::vector<double>{0, 0, 1, 0, 2, 0, 0, 0, 3, 0, 0, 0}, 3, 4);
    B = CCS(std::vector<double>{3, 0, 0, 0, 4, 0}, 2, 3);

    std::vector<double> result = A * B;
    std::vector<double> exp_result{4, 0, 0, 0, 12, 0, 0, 0};

    ASSERT_EQ(result, exp_result);
  }
}

TEST(CCS_Matrix_mult, parall) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  SparseMatrix A, B;
  if (rank == 0) {
    A = CCS(std::vector<double>{0, 0, 1, 0, 2, 0, 0, 0, 3, 0, 0, 0}, 3, 4);
    B = CCS(std::vector<double>{3, 0, 0, 0, 4, 0}, 2, 3);
  }

  std::vector<double> result = multiply(A, B);

  if (rank == 0) {
    std::vector<double> exp_result{4, 0, 0, 0, 12, 0, 0, 0};

    ASSERT_EQ(result, exp_result);
  }
}

TEST(CCS_Matrix_mult, random_20x20) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  SparseMatrix A, B;
  if (rank == 0) {
    A = CCS(randMatrix(20, 20), 20, 20);
    B = CCS(randMatrix(20, 20), 20, 20);
  }

  std::vector<double> result = multiply(A, B);

  if (rank == 0) {
    std::vector<double> exp_result = A * B;

    ASSERT_EQ(result, exp_result);
  }
}

TEST(CCS_Matrix_mult, random_50x50) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  SparseMatrix A, B;
  if (rank == 0) {
    A = CCS(randMatrix(50, 50), 50, 50);
    B = CCS(randMatrix(50, 50), 50, 50);
  }

  std::vector<double> result = multiply(A, B);

  if (rank == 0) {
    std::vector<double> exp_result = A * B;

    ASSERT_EQ(result, exp_result);
  }
}

TEST(CCS_Matrix_mult, random_75x75) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  SparseMatrix A, B;
  if (rank == 0) {
    A = CCS(randMatrix(75, 75), 75, 75);
    B = CCS(randMatrix(75, 75), 75, 75);
  }

  std::vector<double> result = multiply(A, B);

  if (rank == 0) {
    std::vector<double> exp_result = A * B;

    ASSERT_EQ(result, exp_result);
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

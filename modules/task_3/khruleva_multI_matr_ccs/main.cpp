// Copyright 2019 Khruleva Anastasia

#include <gtest/gtest.h>
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <vector>

#include "../../../modules/task_3/khruleva_multI_matr_ccs/multi_ccs.h"

TEST(Matrix_milt_CCS, seq) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    MatrixCCS X, Y;
    X = CCS(std::vector<double>{0, 0, 1, 0, 2, 0, 0, 0, 3, 0, 0, 0}, 3, 4);
    Y = CCS(std::vector<double>{3, 0, 0, 0, 4, 0}, 2, 3);

    std::vector<double> All = X * Y;
    std::vector<double> res{ 4, 0, 0, 0, 12, 0, 0, 0 };

    ASSERT_EQ(All, res);
  }
}

TEST(Matrix_milt_CCS, random_100x100) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MatrixCCS X, Y;
  if (rank == 0) {
    X = CCS(randomMatrix(100, 100), 100, 100);
    Y = CCS(randomMatrix(100, 100), 100, 100);
  }

  std::vector<double> All = multiplication(X, Y);

  if (rank == 0) {
    std::vector<double> res = X * Y;

    ASSERT_EQ(All, res);
  }
}

TEST(Matrix_milt_CCS, rand_256x256) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MatrixCCS X, Y;
  if (rank == 0) {
    X = CCS(randomMatrix(256, 256), 256, 256);
    Y = CCS(randomMatrix(256, 256), 256, 256);
  }

  std::vector<double> All = multiplication(X, Y);

  if (rank == 0) {
    std::vector<double> res = X * Y;

    ASSERT_EQ(All, res);
  }
}

TEST(Matrix_milt_CCS, rand_75x75) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MatrixCCS X, Y;
  if (rank == 0) {
    X = CCS(randomMatrix(75, 75), 75, 75);
    Y = CCS(randomMatrix(75, 75), 75, 75);
  }

  std::vector<double> All = multiplication(X, Y);

  if (rank == 0) {
    std::vector<double> res = X * Y;

    ASSERT_EQ(All, res);
  }
}

TEST(Matrix_milt_CCS, parall) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MatrixCCS X, Y;
  if (rank == 0) {
    X = CCS(std::vector<double>{0, 0, 1, 0, 2, 0, 0, 0, 3, 0, 0, 0}, 3, 4);
    Y = CCS(std::vector<double>{3, 0, 0, 0, 4, 0}, 2, 3);
  }

  std::vector<double> All = multiplication(X, Y);

  if (rank == 0) {
    std::vector<double> res{ 4, 0, 0, 0, 12, 0, 0, 0 };

    ASSERT_EQ(All, res);
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

// Copyright 2019 Koltyushkina Yanina
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_2/koltyushkina_ya_gauss_vert/gauss_vert.h"

TEST(Gauss_vert, correct_size) {
  std::vector<double> mtr;
  ASSERT_ANY_THROW(mtr = RandomMatrix(-1));
}

TEST(Gauss_vert, create_matrix) {
  std::vector<double> mtr;
  ASSERT_NO_THROW(mtr = RandomMatrix(7));
}

TEST(Gauss_vert, correct_gausspr) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> mtr = { 1, 2, 2, 2, 0, 1 };
  std::vector<double> res = PrGauss(mtr, 2);
  std::vector<double> prov = { 2, 0, 2, 1, 1, -0.5 };

  if (rank == 0) {
    ASSERT_EQ(res, prov);
  }
}

TEST(Gauss_vert, correct_all) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> mtr = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 2, 2, 2 };
  std::vector<double> dres(3);
  dres = All(mtr, 3);
  std::vector<double> prov = { 2, 2, 2 };

  if (rank == 0) {
    ASSERT_EQ(dres, prov);
  }
}

TEST(Gauss_vert, correct_gausspr3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> mtr = { 2, 0, 1, 1, 2, 0, 0, 1, 2, 1, 0, 1 };
  std::vector<double> res = PrGauss(mtr, 3);
  std::vector<double> prov = { 2, 0, 0, 1, 2, 0, 0, 1, 2.25, 1, 0, 0.5 };

  if (rank == 0) {
    ASSERT_EQ(res, prov);
  }
}

TEST(Gauss_vert, correct_gausspr4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> mtr = { 2, 0, 0, 0, 0, 1, 0, 0.5, 1, 0, 2, 1, 1, 1, 1, 3, 2, 2, 2, 1 };
  std::vector<double> res = PrGauss(mtr, 4);
  std::vector<double> prov = { 2, 0, 0, 0, 0, 1, 0, 0, 1, 0, 2, 0, 1, 1, 1, 2, 2, 2, 2, -1 };

  if (rank == 0) {
    ASSERT_EQ(res, prov);
  }
}

TEST(Gauss_vert, correct_all1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> mtr = { 2, 0, 0, 0, 1, 1, 0, 0, 1, 2, 2, 0, 2, 2, 1, 1, 3, 1, 2, 1 };
  std::vector<double> dres(4);
  dres = All(mtr, 4);
  std::vector<double> prov = { 1.25, -2, 0.5, 1 };

  if (rank == 0) {
    ASSERT_EQ(dres, prov);
  }
}
TEST(Gauss_vert, correct_gaussobr) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> mtr = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 2, 2, 2 };
  std::vector<double> dres(3);

  dres = ObrGauss(mtr, 3);

  std::vector<double> prov = { 2, 2, 2 };

  if (rank == 0) {
    ASSERT_EQ(dres, prov);
  }
}
TEST(Gauss_vert, correct_gaussobr1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> mtr = { 1, 0, 0, 0, 0, 2, 0, 0, 0, 1, 4, 0, 1, 1, 2, 1, 2, 2, 2, 2 };
  std::vector<double> dres(4);

  dres = ObrGauss(mtr, 4);

  std::vector<double> prov = { 0, 0.25, -0.5, 2 };

  if (rank == 0) {
    ASSERT_EQ(dres, prov);
  }
}

TEST(Gauss_vert, correct_gaussobr2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> mtr = { 2, 0, 0, 0, 1, 1, 0, 0, 1, 2, 2, 0, 2, 2, 1, 1, 3, 1, 2, 1 };
  std::vector<double> dres(4);
  dres = ObrGauss(mtr, 4);

  std::vector<double> prov = { 1.25, -2, 0.5, 1 };

  if (rank == 0) {
    ASSERT_EQ(dres, prov);
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

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  return RUN_ALL_TESTS();
}

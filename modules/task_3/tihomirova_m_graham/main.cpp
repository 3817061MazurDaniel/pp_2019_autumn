// Copyright 2019 Tihomirova Mariya
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <mpi.h>
#include <iostream>
#include <vector>
#include <ctime>
#include "./graham.h"

TEST(graham, test1_GenRand_n_is_negative) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    ASSERT_ANY_THROW(GenPoints(-100));
  }
}

TEST(graham, test2_self_in) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<Point> Points{ {1, 4}, {0, 0}, {0, 2}, {1, 2}, {1, 0.5}, {1, 3}, {-2, 1}, {2, 0}, {2, 2}, {1, 5} };
  std::vector<Point> P = Sort(Points);
  std::vector<Point> res = GrahamPar(P);
  std::vector<Point> check{ {0, 0}, {2, 0}, {2, 2}, {1, 5}, {-2, 1} };
  bool a = 1;
  if (rank == 0) {
    for (size_t i = 0; i < res.size(); i++)
      if (res[i] != check[i])
        a = 0;
    EXPECT_EQ(a, 1);
  }
}

TEST(graham, test3_n_is_100) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<Point> P = GenPoints(100);
  std::vector<Point> res = GrahamPar(P);
  std::vector<Point> check = GrahamSeq(P);
  bool a = 1;
  if (rank == 0) {
    for (size_t i = 0; i < res.size(); i++)
      if (res[i] != check[i])
        a = 0;
    EXPECT_EQ(a, 1);
  }
}

TEST(graham, test4_n_is_200) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<Point> P = GenPoints(200);
  std::vector<Point> res = GrahamPar(P);
  std::vector<Point> check = GrahamSeq(P);
  bool a = 1;
  if (rank == 0) {
    for (size_t i = 0; i < res.size(); i++)
      if (res[i] != check[i])
        a = 0;
    EXPECT_EQ(a, 1);
  }
}

TEST(graham, test5_n_is_300) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<Point> P = GenPoints(300);
  std::vector<Point> res = GrahamPar(P);
  std::vector<Point> check = GrahamSeq(P);
  bool a = 1;
  if (rank == 0) {
    for (size_t i = 0; i < res.size(); i++)
      if (res[i] != check[i])
        a = 0;
    EXPECT_EQ(a, 1);
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

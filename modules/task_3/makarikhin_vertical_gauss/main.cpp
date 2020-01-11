// Copyright 2019 Makarikhin Semen
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <array>
#include "./vertical_gauss.h"

TEST(Vertical_Gauss, test_img_3_rows_3_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rows = 3;
  int cols = 3;

  std::vector<unsigned int> src = Rand_Img(cols, rows);;
  std::vector<unsigned int> result_Parall;
  std::vector<unsigned int> result_Sequential;

  result_Parall = fil_img_parall(src, rows, cols);

  if (rank == 0) {
    result_Sequential = fil_img_sequential(src, rows, cols);
    ASSERT_EQ(result_Parall, result_Sequential);
  }
}

TEST(Vertical_Gauss, test_img_5_rows_5_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rows = 5;
  int cols = 5;

  std::vector<unsigned int> src = Rand_Img(cols, rows);;
  std::vector<unsigned int> result_Parall;
  std::vector<unsigned int> result_Sequential;

  result_Parall = fil_img_parall(src, rows, cols);

  if (rank == 0) {
    result_Sequential = fil_img_sequential(src, rows, cols);
    ASSERT_EQ(result_Parall, result_Sequential);
  }
}

TEST(Vertical_Gauss, test_img_9_rows_9_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rows = 9;
  int cols = 9;

  std::vector<unsigned int> src = Rand_Img(cols, rows);;
  std::vector<unsigned int> result_Parall;
  std::vector<unsigned int> result_Sequential;

  result_Parall = fil_img_parall(src, rows, cols);

  if (rank == 0) {
    result_Sequential = fil_img_sequential(src, rows, cols);
    ASSERT_EQ(result_Parall, result_Sequential);
  }
}

TEST(Vertical_Gauss, test_img_20_rows_20_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rows = 20;
  int cols = 20;

  std::vector<unsigned int> src = Rand_Img(cols, rows);;
  std::vector<unsigned int> result_Parall;
  std::vector<unsigned int> result_Sequential;

  result_Parall = fil_img_parall(src, rows, cols);

  if (rank == 0) {
    result_Sequential = fil_img_sequential(src, rows, cols);
    ASSERT_EQ(result_Parall, result_Sequential);
  }
}

TEST(Vertical_Gauss, test_img_100_rows_100_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rows = 100;
  int cols = 100;

  std::vector<unsigned int> src = Rand_Img(cols, rows);;
  std::vector<unsigned int> result_Parall;
  std::vector<unsigned int> result_Sequential;

  result_Parall = fil_img_parall(src, rows, cols);

  if (rank == 0) {
    result_Sequential = fil_img_sequential(src, rows, cols);
    ASSERT_EQ(result_Parall, result_Sequential);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}

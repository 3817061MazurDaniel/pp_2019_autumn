// Copyright 2019 Poletueva Anastasia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./poletueva_a_contrast_enhancement.h"

TEST(Random_Contrast_MPI, TEST1) {
  int rank;
  int correction = 20;
  std::vector<int > global_res;
  std::vector<int> a;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
  a = getRandomM(10, 15);
  }
  global_res = ContrastFilterParallel(a, 10, 15, correction);

  std::vector<int> reference_res;

  if (rank == 0) {
    reference_res = ContrastFilter(a, 10, 15, correction);
    // for (int i = 0; i < 10 * 15; i++) {
      // EXPECT_EQ(reference_res[i], global_res[i]);
     // std::cout << i << " "<< reference_res[i] << " " << global_res[i] << std::endl;
    //}
    ASSERT_EQ(reference_res, global_res);
  }
  }

TEST(Contrast_Sequential_MPI, TEST2) {
  int rows = 3, cols = 3, contrast = 10;
  std::vector <int> a2(rows * cols), res_a2(rows * cols);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    a2[0] = 150;
    a2[1] = 120;
    a2[2] = 100;
    a2[3] = 123;
    a2[4] = 175;
    a2[5] = 170;
    a2[6] = 50;
    a2[7] = 250;
    a2[8] = 70;
    res_a2[0] = 151;
    res_a2[1] = 118;
    res_a2[2] = 96;
    res_a2[3] = 121;
    res_a2[4] = 179;
    res_a2[5] = 173;
    res_a2[6] = 41;
    res_a2[7] = 255;
    res_a2[8] = 63;
  }
  std::vector <int> res(rows * cols);
  res = ContrastFilter(a2, rows, cols, contrast);
  if (rank == 0) {
    ASSERT_EQ(res_a2, res);
  }
}

TEST(Contrast_Parallel_MPI, TEST3) {
  int rows = 3, cols = 3, contrast = 60;
  std::vector <int> a3(rows * cols), res_a3(rows * cols);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    a3[0] = 150;
    a3[1] = 118;
    a3[2] = 50;
    a3[3] = 120;
    a3[4] = 75;
    a3[5] = 200;
    a3[6] = 157;
    a3[7] = 200;
    a3[8] = 250;
    res_a3[0] = 152;
    res_a3[1] = 101;
    res_a3[2] = 0;
    res_a3[3] = 104;
    res_a3[4] = 32;
    res_a3[5] = 232;
    res_a3[6] = 163;
    res_a3[7] = 232;
    res_a3[8] = 255;
  }
  std::vector <int> res(rows * cols);
  res = ContrastFilterParallel(a3, rows, cols, contrast);
  if (rank == 0) {
    ASSERT_EQ(res_a3, res);
  }
}

TEST(Contrast_throw_MPI, TEST4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    ASSERT_ANY_THROW(getRandomM(-4, 2));
  }
}

TEST(Contrast_Parallel_throw_MPI, TEST5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    ASSERT_ANY_THROW(getRandomM(2, -3));
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

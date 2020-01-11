// Copyright 2019 Rezantsev Sergey
#include <gtest/gtest.h>
#include <math.h>
#include <gtest-mpi-listener.hpp>
#include <vector>
#include "../../../modules/task_2/rezantsev_s_hor_gauss/hor_gauss.h"

TEST(Hor_Gauss_MPI, gauss_test_on_matrix_3x4) {
  std::vector<double> a(12);
  int rank;
  a[0] = 3;
  a[1] = 4;
  a[2] = 8;
  a[3] = 1;
  a[4] = 2;
  a[5] = 5;
  a[6] = 6;
  a[7] = 3;
  a[8] = 1;
  a[9] = 3;
  a[10] = 7;
  a[11] = 9;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> res(3);
  res = getGauss(a, 3);
  bool check = isItTrueAnswer(a, 3, res);
  if (rank == 0) {
    EXPECT_EQ(check, true);
  }
}

TEST(Hor_Gauss_MPI, pargauss_test_on_matrix_2x3) {
  std::vector<double> a(6);
  a = {3, 4, 8, 1, 3, 7};
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> res(2);
  res = getParGauss(a, 2);
  if (rank == 0) {
    bool check = isItTrueAnswer(a, 2, res);
    EXPECT_EQ(check, true);
  }
}

TEST(Hor_Gauss_MPI, pargauss_test_on_matrix_3x4) {
  std::vector<double> a(12);
  int rank;
  a[0] = 3;
  a[1] = 4;
  a[2] = 8;
  a[3] = 1;
  a[4] = 2;
  a[5] = 5;
  a[6] = 6;
  a[7] = 3;
  a[8] = 1;
  a[9] = 3;
  a[10] = 7;
  a[11] = 9;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> res = getParGauss(a, 3);
  if (rank == 0) {
    bool c = isItTrueAnswer(a, 3, res);
    EXPECT_EQ(c, true);
  }
}

TEST(Hor_Gauss_MPI, pargauss_test_on_matrix_4x5) {
  std::vector<double> a(20);
  a = {
    1, 12, 4, 5, 7,
    5, 8, 3, 11, 45,
    19, 3, 4, 8, 3,
    6, 11, 5, 7, 0
  };
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> res(4);
  res = getParGauss(a, 4);
  if (rank == 0) {
    bool check = isItTrueAnswer(a, 4, res);
    EXPECT_EQ(check, true);
  }
}

TEST(Hor_Gauss_MPI, pargauss_test_on_matrix_5x6) {
  std::vector<double> a(30);
  a = {
    1, 12, 4, 5, 7, 5,
    8, 3, 11, 45, 19, 3,
    4, 8, 3, 6, 11, 5,
    7, 0, 4, 13, 44, 18,
    10, 17, 88, 3, 9, 2
  };
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> res(5);
  res = getParGauss(a, 5);
  if (rank == 0) {
    bool check = isItTrueAnswer(a, 5, res);
    EXPECT_EQ(check, true);
  }
}

TEST(Hor_Gauss_MPI, pargauss_test_on_matrix_6x7) {
  std::vector<double> a(42);
  a = {
    1, 42, 4, 7, 5, 0, 3,
    8, 6, 3, 9, 7, 17, 11,
    15, 4, 67, 8, 9, 9, 1,
    23, 6, 4, 2, 1, 6, 4,
    7, 8, 5, 45, 79, 55, 2,
    0, 12, 7, 58, 4, 90, 1
  };
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> res(6);
  res = getParGauss(a, 6);
  if (rank == 0) {
    bool check = isItTrueAnswer(a, 6, res);
    EXPECT_EQ(check, true);
  }
}

/*TEST(Hor_Gauss_MPI, effective_test_on_matrix_100x101) {
  std::vector<double> a = getRandMatrix(100);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double startPar = MPI_Wtime();
  std::vector<double> a1 = getParGauss(a, 100);
  double endPar = MPI_Wtime();
  if (rank == 0) {
    double startSeq = MPI_Wtime();
    std::vector<double> a2 = getGauss(a, 100);
    double endSeq = MPI_Wtime();
    bool c = isItTrueAnswer(a, 100, a1);
    EXPECT_EQ(c, true);
    std::cout << "Time seq: " << endSeq - startSeq << std::endl;
    std::cout << "Time par: " << endPar - startPar << std::endl;
  }
}*/
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

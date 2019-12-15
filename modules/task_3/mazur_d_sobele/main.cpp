// Copyright 2019 Mazur Daniil
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "../../../modules/task_3/mazur_d_sobele/sobele.h"

TEST(Sobele, Test_Image_15_rows_15_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rows = 15;
  int cols = 15;

  std::vector<int> src = randomMatrix(rows, cols);
  std::vector<int> resPar;
  std::vector<int> resSeq;
  if (rank == 0)
    resSeq = soloSobele(src, rows, cols);
  resPar = parSobele(src, rows, cols);
  /*if (rank == 0) {
    std::cout << "StartMatrix" << '\n';
    for (int i = 0; i < cols * rows; ++i) {
      if (i % (cols) == 0)
        std::cout << '\n';
      std::cout << src[i] << ' ';
    }

    std::cout << "soloMatrix" << '\n';
    for (int i = 0; i < cols * rows; ++i) {
      if (i % (cols) == 0)
        std::cout << '\n';
      std::cout << resSeq[i] << ' ';
    }
    std::cout << "parMartix" << '\n';
    for (int i = 0; i < cols * rows; ++i) {
      if (i % (cols) == 0)
        std::cout << '\n';
      std::cout << resPar[i] << ' ';
    }
  } */
  if (rank == 0) {
    ASSERT_EQ(resPar, resSeq);
  }
}



TEST(Sobele, Test_Image_100_rows_102_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rows = 100;
  int cols = 102;

  std::vector<int> src = randomMatrix(rows, cols);
  std::vector<int> resPar;
  std::vector<int> resSeq;
  if (rank == 0)
    resSeq = soloSobele(src, rows, cols);
  resPar = parSobele(src, rows, cols);

  if (rank == 0) {
    ASSERT_EQ(resPar, resSeq);
  }
}
TEST(Sobele, Test_Image_42_rows_83_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rows = 42;
  int cols = 83;

  std::vector<int> src = randomMatrix(rows, cols);
  std::vector<int> resPar;
  std::vector<int> resSeq;
  if (rank == 0)
    resSeq = soloSobele(src, rows, cols);
  resPar = parSobele(src, rows, cols);

  if (rank == 0) {
    ASSERT_EQ(resPar, resSeq);
  }
}


TEST(Sobele, Test_Image_3_rows_3_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rows = 3;
  int cols = 3;

  std::vector<int> src = randomMatrix(rows, cols);
  std::vector<int> resPar;
  std::vector<int> resSeq;
  if (rank == 0)
    resSeq = soloSobele(src, rows, cols);
  resPar = parSobele(src, rows, cols);

  if (rank == 0) {
    ASSERT_EQ(resPar, resSeq);
  }
}

TEST(Sobele, Test_Image_2_rows_6_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rows = 2;
  int cols = 6;

  std::vector<int> src = randomMatrix(rows, cols);
  std::vector<int> resPar;
  std::vector<int> resSeq;
  if (rank == 0)
    resSeq = soloSobele(src, rows, cols);
  resPar = parSobele(src, rows, cols);

  if (rank == 0) {
    ASSERT_EQ(resPar, resSeq);
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

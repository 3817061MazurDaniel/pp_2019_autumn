// Copyright 2019 Mazur Daniil
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "../../../modules/task_3/mazur_d_sobele/sobele.h"

TEST(Sobele, Test_Image_15_rows_15_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  constexpr int rows{ 15 };
  constexpr int cols{ 15 };

  std::vector<unsigned int> src = randomMatrix(rows, cols);
  std::vector<unsigned int> resPar;
  std::vector<unsigned int> resSeq;

  if (rank == 0)
    resSeq = soloSobele(src, rows, cols);
  resPar = parSobele(src, rows, cols);

  if (rank == 0)
    ASSERT_EQ(resPar, resSeq);
}

TEST(Sobele, Test_Image_invalid_rows_invalid_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  constexpr int rows{ 2 };
  constexpr int cols{ 15 };

  std::vector<unsigned int> src = randomMatrix(rows, cols);
  std::vector<unsigned int> resPar;
  std::vector<unsigned int> resSeq;

  if (rank == 0)
    resSeq = soloSobele(src, rows, cols);
  resPar = parSobele(src, rows, cols);

  if (rank == 0)
    ASSERT_EQ(resPar, resSeq);
}

TEST(Sobele, Test_Image_3_rows_3_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  constexpr int rows{ 3 };
  constexpr int cols{ 3 };

  std::vector<unsigned int> src = randomMatrix(rows, cols);
  std::vector<unsigned int> resPar;
  std::vector<unsigned int> resSeq;

  if (rank == 0)
    resSeq = soloSobele(src, rows, cols);
  resPar = parSobele(src, rows, cols);

  if (rank == 0)
    ASSERT_EQ(resPar, resSeq);
}

TEST(Sobele, Test_Image_42_rows_14_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  constexpr int rows{ 42 };
  constexpr int cols{ 14 };

  std::vector<unsigned int> src = randomMatrix(rows, cols);
  std::vector<unsigned int> resPar;
  std::vector<unsigned int> resSeq;

  if (rank == 0)
    resSeq = soloSobele(src, rows, cols);
  resPar = parSobele(src, rows, cols);

  if (rank == 0)
    ASSERT_EQ(resPar, resSeq);
}

TEST(Sobele, Test_Image_100_rows_102_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  constexpr int rows{ 100 };
  constexpr int cols{ 102 };

  std::vector<unsigned int> src = randomMatrix(rows, cols);
  std::vector<unsigned int> resPar;
  std::vector<unsigned int> resSeq;

  if (rank == 0)
    resSeq = soloSobele(src, rows, cols);
  resPar = parSobele(src, rows, cols);

  if (rank == 0)
    ASSERT_EQ(resPar, resSeq);
}

TEST(Sobele, Test_Image_44_rows_65_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  constexpr int rows{ 44 };
  constexpr int cols{ 65 };

  std::vector<unsigned int> src = randomMatrix(rows, cols);
  std::vector<unsigned int> resPar;
  std::vector<unsigned int> resSeq;

  if (rank == 0)
    resSeq = soloSobele(src, rows, cols);
  resPar = parSobele(src, rows, cols);

  if (rank == 0)
    ASSERT_EQ(resPar, resSeq);
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

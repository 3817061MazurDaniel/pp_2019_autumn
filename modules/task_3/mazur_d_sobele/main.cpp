// Copyright 2019 Mazur Daniil
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "../../../modules/task_3/mazur_d_sobele/sobele.h"

TEST(Sobele, Test_Image_9_rows_9_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  constexpr int rows{ 9 };
  constexpr int cols{ 9 };

  std::vector<unsigned char> src = randomMatrix(cols, rows);;
  std::vector<unsigned char> resPar;
  std::vector<unsigned char> resSeq;

  resPar = parSobele(src, cols, rows);

  if (rank == 0) {
    resSeq = soloSobele(src, cols, rows);
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
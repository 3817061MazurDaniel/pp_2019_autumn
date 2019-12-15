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

  std::vector<unsigned short> src = randomMatrix(rows, cols);;
  std::vector<unsigned short> resPar;
  std::vector<unsigned short> resSeq;

  resPar = parSobele(src, rows, cols);

  if (rank == 0) {
    resSeq = soloSobele(src, rows, cols);
#ifdef DEBUG
    std::cout << "Start Matrix:" << std::endl;
    for (int i = 0; i < cols * rows; ++i) {
      std::cout << (unsigned int)src[i] << " ";
      if ((i + 1) % cols == 0)
        std::cout << '\n';
    }

    std::cout << "Parallel result:" << std::endl;
    for (int i = 0; i < cols * rows; ++i) {
      std::cout << (unsigned int)resPar[i] << " ";
      if ((i + 1) % cols == 0)
        std::cout << '\n';
    }
    std::cout << std::endl;
    std::cout << "Sequential result:" << std::endl;
    for (int i = 0; i < cols * rows; ++i) {
      std::cout << (unsigned int)resSeq[i] << " ";
      if ((i + 1) % cols == 0)
        std::cout << '\n';
    }
#endif //DEBUG
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
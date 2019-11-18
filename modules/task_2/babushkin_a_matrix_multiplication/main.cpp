// Copyright 2019 Babushkin Aleksey

#include <gtest/gtest.h>
#include <mpi.h>

#include <gtest-mpi-listener.hpp>

#include "./matrix_multiplication.h"


void multTest(const int, const int, const int);

TEST(Matrix_Sample_Tests, Sequential_Muliplication) {
  int __rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &__rank);

  if (__rank == 0) {
    matrix __testMatrixLeft{1, 2, 3, 4, 5, 6, 7, 8, 9};

    matrix __testMatrixRight{1, 4, 2, 5, 3, 6};

    matrix __result(6);
    matrix __expectedResult{14, 32, 32, 77, 50, 122};

    __result = multiply_seq(__testMatrixLeft, __testMatrixRight, 3, 3, 2);

    ASSERT_EQ(__result, __expectedResult);
  }
}

TEST(Matrix_Sample_Tests, Parll_Muliplication) {
  int __rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &__rank);

  double __elapsedTime;
  if (__rank == 0) {
    __elapsedTime = MPI_Wtime();
  }

  matrix __testMatrixLeft{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  matrix __testMatrixRight(24, 2);

  matrix __result = multiply(__testMatrixLeft, __testMatrixRight, 4, 3, 8);

  if (__rank == 0) {
    matrix __expectedResult =
        multiply_seq(__testMatrixLeft, __testMatrixRight, 4, 3, 8);

    __elapsedTime = MPI_Wtime() - __elapsedTime;
    std::cout << "Elapsed time: " << __elapsedTime << "s" << std::endl;

    ASSERT_EQ(__result, __expectedResult);
  }
}

TEST(Matrix_Sample_Tests, Rand_Matrix_20) { multTest(4, 5, 8); }

TEST(Matrix_Sample_Tests, Rand_Matrix_200) { multTest(40, 5, 16); }

TEST(Matrix_Sample_Tests, Rand_Matrix_1000) { multTest(25, 40, 64); }

TEST(Matrix_Sample_Tests, Rand_Matrix_10000) { multTest(100, 100, 100); }

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

void multTest(const int __rows, const int __cols, const int __rCols) {
  int __rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &__rank);

  double __elapsedTime;
  if (__rank == 0) {
    __elapsedTime = MPI_Wtime();
  }

  matrix __testMatrixLeft = getRandomMatrix(__rows, __cols);
  matrix __testMatrixRight = getRandomMatrix(__cols, __rCols);

  matrix __result =
      multiply(__testMatrixLeft, __testMatrixRight, __rows, __cols, __rCols);

  if (__rank == 0) {
    matrix __expectedResult = multiply_seq(__testMatrixLeft, __testMatrixRight,
                                           __rows, __cols, __rCols);

    __elapsedTime = MPI_Wtime() - __elapsedTime;
    std::cout << "Elapsed time: " << __elapsedTime << "s" << std::endl;

    ASSERT_EQ(__result, __expectedResult);
  }
}

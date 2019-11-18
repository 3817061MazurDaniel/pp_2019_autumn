// Copyright 2019 Myshkin Andrew
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "./min_matrix.h"


TEST(Parallel_Min_Matrix_MPI, Test_On_Min_Matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_matrix;
  const int rows_matrix_size = 50;
  const int cols_matrix_size = 50;

  if (rank == 0) {
    global_matrix = getRandomMatrix(rows_matrix_size, cols_matrix_size);
  }

  int global_min;
  global_min = getParallelMinMatrix(global_matrix, rows_matrix_size, cols_matrix_size);

  if (rank == 0) {
    int reference_min = *std::min_element(global_matrix.begin(), global_matrix.end());
    ASSERT_EQ(global_min, reference_min);
  }
}


TEST(Parallel_Min_Matrix_MPI, Test_With_Consistent_Matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> matrix;
  const int rows_matrix_size = 100;
  const int cols_matrix_size = 100;

  if (rank == 0) {
    matrix = getConsistentMatrix(rows_matrix_size, cols_matrix_size);
  }

  int min_matrix = getParallelMinMatrix(matrix, rows_matrix_size, cols_matrix_size);
  if (rank == 0) {
    ASSERT_EQ(0, min_matrix);
  }
}


TEST(Parallel_Min_Matrix_MPI, Test_With_Zero_Matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> matrix;
  const int rows_matrix_size = 10;
  const int cols_matrix_size = 0;

  if (rank == 0) {
    ASSERT_ANY_THROW(getRandomMatrix(rows_matrix_size, cols_matrix_size));
  }
}


TEST(Parallel_Min_Matrix_MPI, Test_With_Small_Matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> matrix;
  const int rows_matrix_size = 6;
  const int cols_matrix_size = 6;

  if (rank == 0) {
    matrix = getConsistentMatrix(rows_matrix_size, cols_matrix_size);
  }

  int min_matrix = getParallelMinMatrix(matrix, rows_matrix_size, cols_matrix_size);
  if (rank == 0) {
    ASSERT_EQ(0, min_matrix);
  }
}


TEST(Parallel_Min_Matrix_MPI, Test_With_Given_Matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int rows_matrix_size = 3;
  const int cols_matrix_size = 3;
  std::vector<int> matrix(rows_matrix_size * cols_matrix_size);

  matrix[0] = 4;
  matrix[1] = 3;
  matrix[2] = 7;
  matrix[3] = 9;
  matrix[4] = 5;
  matrix[5] = 7;
  matrix[6] = 10;
  matrix[7] = 8;
  matrix[8] = 12;

  int min_matrix = getParallelMinMatrix(matrix, rows_matrix_size, cols_matrix_size);
  if (rank == 0) {
    ASSERT_EQ(3, min_matrix);
  }
}


TEST(Parallel_Min_Matrix_MPI, Test_With_Different_rows_n_cols) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> matrix;
  const int rows_matrix_size = 8;
  const int cols_matrix_size = 22;

  if (rank == 0) {
    matrix = getConsistentMatrix(rows_matrix_size, cols_matrix_size);
  }

  int min_matrix = getParallelMinMatrix(matrix, rows_matrix_size, cols_matrix_size);
  if (rank == 0) {
    ASSERT_EQ(0, min_matrix);
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

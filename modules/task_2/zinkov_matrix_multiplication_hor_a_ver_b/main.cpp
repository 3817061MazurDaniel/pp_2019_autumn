//  Copyright 2019 Zinkov Artem
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_multiplication_hor_a_ver_b.h"

TEST(Matrix_Multiplication_Horizontal_A_Vertical_B, Test_Multiplication) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int matrix_side_size = 10;
  std::vector<int> matrix_a, matrix_b;
  std::vector<int> resulting_matrix_parallel, resulting_matrix_seqential;

  if (rank == 0) {
    matrix_a = getRandomMatrix(matrix_side_size);
    matrix_b = getRandomMatrix(matrix_side_size);
  }
  resulting_matrix_parallel = getMatrixMultiplicationParellel(matrix_a, matrix_b, matrix_side_size);

  if (rank == 0) {
    resulting_matrix_seqential = getMatrixMultiplication(matrix_a, matrix_b, matrix_side_size);

    ASSERT_EQ(resulting_matrix_parallel, resulting_matrix_seqential);
  }
}

TEST(Matrix_Multiplication_Horizontal_A_Vertical_B, Test_Small_Matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int matrix_side_size = 4;
  std::vector<int> matrix_a, matrix_b;
  std::vector<int> resulting_matrix_parallel, resulting_matrix_seqential;

  if (rank == 0) {
    matrix_a = getRandomMatrix(matrix_side_size);
    matrix_b = getRandomMatrix(matrix_side_size);
  }
  resulting_matrix_parallel = getMatrixMultiplicationParellel(matrix_a, matrix_b, matrix_side_size);

  if (rank == 0) {
    resulting_matrix_seqential = getMatrixMultiplication(matrix_a, matrix_b, matrix_side_size);

    ASSERT_EQ(resulting_matrix_parallel, resulting_matrix_seqential);
}
}

TEST(Matrix_Multiplication_Horizontal_A_Vertical_B, Test_Big_Matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int matrix_side_size = 200;
  std::vector<int> matrix_a, matrix_b;
  std::vector<int> resulting_matrix_parallel, resulting_matrix_seqential;

  if (rank == 0) {
    matrix_a = getRandomMatrix(matrix_side_size);
    matrix_b = getRandomMatrix(matrix_side_size);
  }
  resulting_matrix_parallel = getMatrixMultiplicationParellel(matrix_a, matrix_b, matrix_side_size);

  if (rank == 0) {
    resulting_matrix_seqential = getMatrixMultiplication(matrix_a, matrix_b, matrix_side_size);

    ASSERT_EQ(resulting_matrix_parallel, resulting_matrix_seqential);
  }
}

TEST(Matrix_Multiplication_Horizontal_A_Vertical_B, Test_Odd_Side_Size_Matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int matrix_side_size = 25;
  std::vector<int> matrix_a, matrix_b;
  std::vector<int> resulting_matrix_parallel, resulting_matrix_seqential;

  if (rank == 0) {
    matrix_a = getRandomMatrix(matrix_side_size);
    matrix_b = getRandomMatrix(matrix_side_size);
  }
  resulting_matrix_parallel = getMatrixMultiplicationParellel(matrix_a, matrix_b, matrix_side_size);

  if (rank == 0) {
    resulting_matrix_seqential = getMatrixMultiplication(matrix_a, matrix_b, matrix_side_size);

    ASSERT_EQ(resulting_matrix_parallel, resulting_matrix_seqential);
  }
}

TEST(Matrix_Multiplication_Horizontal_A_Vertical_B, Test_Matrix_With_Identical_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int matrix_side_size = 10;
  std::vector<int> matrix_a(matrix_side_size * matrix_side_size, 1);
  std::vector<int> matrix_b(matrix_side_size * matrix_side_size, 1);

  std::vector<int> resulting_matrix(matrix_side_size * matrix_side_size, matrix_side_size);
  std::vector<int> resulting_matrix_parallel;

  resulting_matrix_parallel = getMatrixMultiplicationParellel(matrix_a, matrix_b, matrix_side_size);

  if (rank == 0) {
    ASSERT_EQ(resulting_matrix_parallel, resulting_matrix);
  }
}

TEST(Matrix_Multiplication_Horizontal_A_Vertical_B, Test_Matrix_With_Null_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int matrix_side_size = 10;
  std::vector<int> matrix_a(matrix_side_size * matrix_side_size, 0);
  std::vector<int> matrix_b(matrix_side_size * matrix_side_size, 0);
  std::vector<int> resulting_matrix(matrix_side_size * matrix_side_size, 0);
  std::vector<int> resulting_matrix_parallel;

  resulting_matrix_parallel = getMatrixMultiplicationParellel(matrix_a, matrix_b, matrix_side_size);

  if (rank == 0) {
    ASSERT_EQ(resulting_matrix_parallel, resulting_matrix);
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

// Copyright 2019 Mityagina Daria

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <random>
#include "../../../modules/task_2/mityagina_d_reduce/reduce.h"

void test_add(int *a, int* b, int n) {
    for (int i = 0; i < n; i++)
      b[i] += a[i];
}

void test_multiplication(int *a, int* b, int n) {
    for (int i = 0; i < n; i++)
     b[i] *= a[i];
}

void test_i(int *a, int* b, int n) {
    for (int i = 0; i < n; i++)
        b[i] &= a[i];
}

int SumOfMatrixElementsPartly(std::vector<int> matrix) {
  int sum = 0;
  int size = matrix.size();
  for (int i = 0; i < size; i++)
    sum += matrix[i];
  return sum;
}

int MultiplicationOfMatrixElementsPartly(std::vector<int> matrix) {
  int sum = 0;
  int size = matrix.size();
  for (int i = 0; i < size; i++)
    sum *= matrix[i];
  return sum;
}

int iOfMatrixElementsPartly(std::vector<int> matrix) {
  int sum = 0;
  int size = matrix.size();
  for (int i = 0; i < size; i++)
    sum &= matrix[i];
  return sum;
}

std::vector<int> getMatrix(int size) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> new_matrix(size);
  for (int i = 0; i < size; ++i) {
    new_matrix[i] = gen() % 100;
  }
  return new_matrix;
}

int Work(int size, std::vector<int> matrix, int choice) {
  int sum_res = 0;
  int rank, p_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p_size);
  const int calculate_part = size / p_size;
  const int dop = size % p_size;
  int error;
  MPI_Status status;
  MPI_Datatype ctype;

  MPI_Type_contiguous(2, MPI_DOUBLE, &ctype);
  MPI_Type_commit(&ctype);

  if (rank == 0) {
    if (size <= 0) {
      error = 1;
    } else {
      error = 0;
    }
    for (int proc = 1; proc < p_size; ++proc)
      MPI_Send(&error, 1, MPI_INT, proc, 5, MPI_COMM_WORLD);
  } else {
    MPI_Recv(&error, 1, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);
  }
  switch (error) {
  case 0:
    break;
  case 1:
    throw std::runtime_error("size <= 0");
  }
  std::vector<int> recieved = std::vector<int>(calculate_part, 0);

  if (rank == 0) {
    if (calculate_part != 0) {
      for (int proc = 1; proc < p_size; proc++)
        MPI_Send(&matrix[dop] + proc * calculate_part, calculate_part, MPI_INT, proc, 0, MPI_COMM_WORLD);
    }
  }
  if (rank == 0) {
    recieved.resize(calculate_part + dop);
    recieved = std::vector<int>(matrix.begin(), matrix.begin() + calculate_part + dop);
  } else if (calculate_part != 0) {
      MPI_Recv(&recieved[0], calculate_part, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }
  int part_sum = SumOfMatrixElementsPartly(recieved);
  if (calculate_part != 0) {
    switch (choice) {
      case 0 :
      MPI_Reduce(&part_sum, &sum_res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);  // Сумма
      break;
      case 1 :
      Reduce(&part_sum, &sum_res, 1, MPI_INT, (func)test_add, 0, MPI_COMM_WORLD);
      break;
      case 2 :
      MPI_Reduce(&part_sum, &sum_res, 1, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);  // Произведение
      break;
      case 3 :
      Reduce(&part_sum, &sum_res, 1, MPI_INT, (func)test_multiplication, 0, MPI_COMM_WORLD);
      break;
      case 4 :
      MPI_Reduce(&part_sum, &sum_res, 1, MPI_INT, MPI_BAND, 0, MPI_COMM_WORLD);  // Побитовая операция И
      break;
      case 5 :
      Reduce(&part_sum, &sum_res, 1, MPI_INT, (func)test_i, 0, MPI_COMM_WORLD);
      break;
      default:
      break;
    }
  } else {
    sum_res = part_sum;
  }

  return sum_res;
}

void testing_lab_1(int size) {
    int answer0, answer1;
    int rank;
    std::vector<int> matrix(size, 0);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matrix = getMatrix(size);
    }
    answer0 = Work(size, matrix, 0);
    answer1 = Work(size, matrix, 1);
    if (rank == 0) {
      ASSERT_EQ(answer0, answer1);
    }
}

void testing_lab_2(int size) {
    int answer0, answer1;
    int rank;
    std::vector<int> matrix(size, 0);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matrix = getMatrix(size);
    }
    answer0 = Work(size, matrix, 2);
    answer1 = Work(size, matrix, 3);
    if (rank == 0) {
      ASSERT_EQ(answer0, answer1);
    }
}

void testing_lab_3(int size) {
    int answer0, answer1;
    int rank;
    std::vector<int> matrix(size, 0);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matrix = getMatrix(size);
    }
    answer0 = Work(size, matrix, 4);
    answer1 = Work(size, matrix, 5);
    if (rank == 0) {
      ASSERT_EQ(answer0, answer1);
    }
}

TEST(Reduce_MPI, Test1_On_Size_1) {
    int size = 1;
    testing_lab_1(size);
}

TEST(Reduce_MPI, Test1_On_Size_3) {
    int size = 3;
    testing_lab_1(size);
}

TEST(Reduce_MPI, Test1_On_Size_100) {
    int size = 100;
    testing_lab_1(size);
}

TEST(Reduce_MPI, Test1_On_Size_1000) {
    int size = 1000;
    testing_lab_1(size);
}

TEST(Reduce_MPI, Test1_On_Size_10000) {
    int size = 10000;
    testing_lab_1(size);
}

TEST(Reduce_MPI, Test2_On_Size_1) {
    int size = 1;
    testing_lab_2(size);
}

TEST(Reduce_MPI, Test2_On_Size_3) {
    int size = 3;
    testing_lab_2(size);
}

TEST(Reduce_MPI, Test2_On_Size_100) {
    int size = 100;
    testing_lab_2(size);
}

TEST(Reduce_MPI, Test2_On_Size_1000) {
    int size = 1000;
    testing_lab_2(size);
}

TEST(Reduce_MPI, Test2_On_Size_10000) {
    int size = 10000;
    testing_lab_2(size);
}

TEST(Reduce_MPI, Test3_On_Size_1) {
    int size = 1;
    testing_lab_3(size);
}

TEST(Reduce_MPI, Test3_On_Size_3) {
    int size = 3;
    testing_lab_3(size);
}

TEST(Reduce_MPI, Test3_On_Size_100) {
    int size = 100;
    testing_lab_3(size);
}

TEST(Reduce_MPI, Test3_On_Size_1000) {
    int size = 1000;
    testing_lab_3(size);
}

TEST(Reduce_MPI, Test3_On_Size_10000) {
    int size = 10000;
    testing_lab_3(size);
}

int main(int argc, char **argv) {
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

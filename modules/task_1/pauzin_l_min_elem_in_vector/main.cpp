// Copyright 2019 Pauzin Leonid
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include "./pauzin_l_min_elem_in_vector.h"

TEST(Parallel_Min_In_Vector_MPI, Test_On_Default_Vec) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vec;
  const int count_size_vector = 100;
  if (rank == 0) {
    vec = getVec(count_size_vector);
  }
  int minElem = minElemInVec(vec, count_size_vector);
  if (rank == 0) {
    ASSERT_EQ(0, minElem);
  }
}

TEST(Parallel_Min_In_Vector_MPI, Test_With_Random) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int count_size_vector = 100;
  std::vector<int> vec;
  if (rank == 0) {
    vec = getRandomVector(count_size_vector);
  }
  int minElem = minElemInVec(vec, count_size_vector);
  if (rank == 0) {
    std::vector<int>::iterator minElemSeq = std::min_element(vec.begin(), vec.end());
    ASSERT_EQ(minElem, *minElemSeq);
  }
}


TEST(Parallel_Min_In_Vector_MPI, Test_With_Const) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int count_size_vector = 10;
  std::vector<int> vec(count_size_vector);
  if (rank == 0) {
    for (int i = 0; i < count_size_vector; i++) {
      vec[i] = 7;
    }
  }
  int minElem = minElemInVec(vec, count_size_vector);
  if (rank == 0) {
    ASSERT_EQ(7, minElem);
  }
}

TEST(Parallel_Min_In_Vector_MPI, Test_With_Negative_Elems) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int count_size_vector = 10;
  std::vector<int> vec(count_size_vector);
  if (rank == 0) {
    for (int i = 0; i < count_size_vector; i++) {
      vec[i] = -i - 1;
    }
  }
  int minElem = minElemInVec(vec, count_size_vector);
  if (rank == 0) {
    ASSERT_EQ(-10, minElem);
  }
}

TEST(Parallel_Min_In_Vector_MPI, Test_Vec_With_One_Elem) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vec;
  const int count_size_vector = 1;
  if (rank == 0) {
    vec = getVec(count_size_vector);
  }
  int minElem = minElemInVec(vec, count_size_vector);
  if (rank == 0) {
    ASSERT_EQ(0, minElem);
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

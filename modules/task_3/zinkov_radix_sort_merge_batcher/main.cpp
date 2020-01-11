  //  Copyright 2019 Zinkov Artem
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <numeric>
#include <algorithm>
#include "./radix_sort_merge_batcher.h"

TEST(Radix_Sort_Merge_Batcher, Test_Merge) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int size_vector = 500;
  if (rank == 0) {
    global_vec = getRandomVector(size_vector);
  }

  std::vector<int> parralel = merge_batcher(global_vec, size_vector);
  if (rank == 0) {
    global_vec = radix_sort(global_vec);
    ASSERT_EQ(parralel, global_vec);
  }
}

TEST(Radix_Sort_Merge_Batcher, Test_Disordered_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec(50), res(50);
  if (rank == 0) {
    for (size_t i = 0; i < global_vec.size(); i++) {
      global_vec[i] = global_vec.size() - i;
    }
    std::iota(res.begin(), res.end(), 1);
  }

  std::vector<int> parralel = merge_batcher(global_vec, global_vec.size());
  if (rank == 0) {
    ASSERT_EQ(parralel, res);
  }
}

TEST(Radix_Sort_Merge_Batcher, Test_Odd_Size_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int size_vector = 225;
  if (rank == 0) {
    global_vec = getRandomVector(size_vector);
  }

  std::vector<int> parralel = merge_batcher(global_vec, size_vector);
  if (rank == 0) {
    global_vec = radix_sort(global_vec);
    ASSERT_EQ(parralel, global_vec);
  }
}

TEST(Radix_Sort_Merge_Batcher, Test_Identical_Elements_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec(200, 5);

  std::vector<int> parralel = merge_batcher(global_vec, global_vec.size());
  if (rank == 0) {
    global_vec = radix_sort(global_vec);
    ASSERT_EQ(parralel, global_vec);
  }
}

TEST(Radix_Sort_Merge_Batcher, Test_Ordered_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec(200);
  if (rank == 0) {
    std::iota(global_vec.begin(), global_vec.end(), 1);
  }

  std::vector<int> parralel = merge_batcher(global_vec, global_vec.size());
  if (rank == 0) {
    ASSERT_EQ(parralel, global_vec);
  }
}

TEST(Radix_Sort_Merge_Batcher, Test_Big_Size_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int size_vector = 10000;
  if (rank == 0) {
    global_vec = getRandomVector(size_vector);
  }

  std::vector<int> parralel = merge_batcher(global_vec, size_vector);
  if (rank == 0) {
    global_vec = radix_sort(global_vec);
    ASSERT_EQ(parralel, global_vec);
  }
}

TEST(Radix_Sort_Merge_Batcher, Test_Const_Vect) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec = {57, 39, 26, 163, 4, 273, 14, 2, 356, 37, 93, 3, 678, 256, 83, 17, 26};
  std::vector<int> res = {2, 3, 4, 14, 17, 26, 26, 37, 39, 57, 83, 93, 163, 256, 273, 356, 678};

  std::vector<int> parralel = merge_batcher(global_vec, global_vec.size());
  if (rank == 0) {
    ASSERT_EQ(parralel, res);
  }
}

TEST(Radix_Sort_Merge_Batcher, Test_One_Elements_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int size_vector = 1;
  if (rank == 0) {
    global_vec = getRandomVector(size_vector);
  }

  std::vector<int> parralel = merge_batcher(global_vec, size_vector);
  if (rank == 0) {
    global_vec = radix_sort(global_vec);
    ASSERT_EQ(parralel, global_vec);
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

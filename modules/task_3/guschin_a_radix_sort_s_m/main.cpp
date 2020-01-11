// Copyright 2019 Guschin Alexander
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <string>
#include <vector>
#include "../../../modules/task_3/guschin_a_radix_sort_s_m/radix_sort_s_m.h"

TEST(radix_sort, radix_sort) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<std::int64_t> vec;

  if (rank == 0) {
    vec.resize(1000);
    Fill_random(&vec[0], vec.size());
    std::vector<std::int64_t> res(Radix_sort(vec));
    bool is_sort = true;
    int length = vec.size();
    for (int i = 1; i < length; ++i)
      if (res[i] < res[i - 1]) {
        is_sort = false;
        break;
      }
    EXPECT_EQ(is_sort, 1);
  }
}

TEST(radix_sort, can_sort_int64) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<std::int64_t> vec(1000);
  Fill_random(&vec[0], vec.size());
  std::vector<std::int64_t> res(P_radix_sort(vec));
  if (rank == 0) {
    bool is_sort = true;
    int length = vec.size();
    for (int i = 1; i < length; ++i)
      if (res[i] < res[i - 1]) {
        is_sort = false;
        break;
      }

    EXPECT_EQ(is_sort, 1);
  }
}

TEST(radix_sort, can_sort_uint64) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<std::uint64_t> vec(1000);
  Fill_random(&vec[0], vec.size());
  std::vector<std::uint64_t> res(P_radix_sort(vec));
  if (rank == 0) {
    bool is_sort = true;
    int length = vec.size();
    for (int i = 1; i < length; ++i)
      if (res[i] < res[i - 1]) {
        is_sort = false;
        break;
      }

    EXPECT_EQ(is_sort, 1);
  }
}

TEST(radix_sort, can_sort_int32) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<std::int32_t> vec(1000);
  Fill_random(&vec[0], vec.size());
  std::vector<std::int32_t> res(P_radix_sort(vec));
  if (rank == 0) {
    bool is_sort = true;
    int length = vec.size();
    for (int i = 1; i < length; ++i)
      if (res[i] < res[i - 1]) {
        is_sort = false;
        break;
      }

    EXPECT_EQ(is_sort, 1);
  }
}

TEST(radix_sort, can_sort_uint32) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<std::uint32_t> vec(1000);
  Fill_random(&vec[0], vec.size());
  std::vector<std::uint32_t> res(P_radix_sort(vec));
  if (rank == 0) {
    bool is_sort = true;
    int length = vec.size();
    for (int i = 1; i < length; ++i)
      if (res[i] < res[i - 1]) {
        is_sort = false;
        break;
      }

    EXPECT_EQ(is_sort, 1);
  }
}

TEST(radix_sort, can_sort_int16) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<std::int16_t> vec(1000);
  Fill_random(&vec[0], vec.size());
  std::vector<std::int16_t> res(P_radix_sort(vec));
  if (rank == 0) {
    bool is_sort = true;
    int length = vec.size();
    for (int i = 1; i < length; ++i)
      if (res[i] < res[i - 1]) {
        is_sort = false;
        break;
      }

    EXPECT_EQ(is_sort, 1);
  }
}

TEST(radix_sort, can_sort_uint16) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<std::uint16_t> vec(1000);
  Fill_random(&vec[0], vec.size());
  std::vector<std::uint16_t> res(P_radix_sort(vec));
  if (rank == 0) {
    bool is_sort = true;
    int length = vec.size();
    for (int i = 1; i < length; ++i)
      if (res[i] < res[i - 1]) {
        is_sort = false;
        break;
      }

    EXPECT_EQ(is_sort, 1);
  }
}

TEST(radix_sort, can_sort_defined_array_int32) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<std::int32_t> vec{-32, 11, 332, 42, 1 << 28, -8, 1 << 26, -(1 << 26), 55, -24, -(1 << 27), 1111};
  std::vector<std::int32_t> expected{-(1 << 27), -(1 << 26), -32, -24, -8, 11, 42, 55, 332, 1111, 1 << 26, 1 << 28};
  std::vector<std::int32_t> res(P_radix_sort(vec));
  if (rank == 0) {
    EXPECT_EQ(res, expected);
  }
}

TEST(radix_sort, can_sort_defined_array_uint16) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<std::uint16_t> vec{1, 4, 2, 16, 4, 64, 8, 256, 16, 1024};
  std::vector<std::uint16_t> expected{1, 2, 4, 4, 8, 16, 16, 64, 256, 1024};
  std::vector<std::uint16_t> res(P_radix_sort(vec));
  if (rank == 0) {
    EXPECT_EQ(res, expected);
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

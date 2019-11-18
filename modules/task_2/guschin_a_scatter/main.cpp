// Copyright 2019 Guschin Alexander
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <string>
#include <vector>
#include "../../../modules/task_2/guschin_a_scatter/scatter.h"

TEST(scatter, throw_when_different_size) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<int> p(1);
  int dest[3];
  EXPECT_EQ(MPI_Scatter_custom(&p[0], 1, MPI_INT, &dest[0], 99, MPI_INT, 0, MPI_COMM_WORLD),
            MPI_ERR_COUNT);
}

TEST(scatter, can_scatter_and_gather_double) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<double> p(3 * size);
  std::vector<double> d(3 * size);
  double dest[3];
  if (rank == 0) {
    for (int i = 0; i < 3 * size; ++i) p[i] = i + 1.0/i;
  }
  MPI_Scatter_custom(&p[0], 3, MPI_DOUBLE, &dest[0], 3, MPI_DOUBLE, 0,
                     MPI_COMM_WORLD);
  MPI_Gather(&dest[0], 3, MPI_DOUBLE, &d[0], 3, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    EXPECT_EQ(p, d);
  }
}

TEST(scatter, can_scatter_and_gather_int) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<int> p(3 * size);
  std::vector<int> d(3 * size);
  int dest[3];
  if (rank == 0) {
    for (int i = 0; i < 3 * size; ++i) p[i] = i;
  }
  MPI_Scatter_custom(&p[0], 3, MPI_INT, &dest[0], 3, MPI_INT, 0,
                     MPI_COMM_WORLD);
  MPI_Gather(&dest[0], 3, MPI_INT, &d[0], 3, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    EXPECT_EQ(p, d);
  }
}

TEST(scatter, can_compare_custom_scatter_with_MPI_Scatter) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<int> p(3 * size);
  std::vector<int> d1(3 * size);
  std::vector<int> d2(3 * size);
  int dest[3];
  if (rank == 0) {
    for (int i = 0; i < 3 * size; ++i) p[i] = i;
  }
  MPI_Scatter_custom(&p[0], 3, MPI_INT, &dest[0], 3, MPI_INT, 0,
                     MPI_COMM_WORLD);
  MPI_Gather(&dest[0], 3, MPI_INT, &d1[0], 3, MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Scatter(&p[0], 3, MPI_INT, &dest[0], 3, MPI_INT, 0,
                     MPI_COMM_WORLD);
  MPI_Gather(&dest[0], 3, MPI_INT, &d2[0], 3, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    EXPECT_EQ(d1, d2);
  }
}

TEST(scatter, can_scatter_and_gather_float) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<float> p(3 * size);
  std::vector<float> d(3 * size);
  float dest[3];
  if (rank == 0) {
    for (int i = 0; i < 3 * size; ++i) p[i] = i + 1.0 / i;
  }
  MPI_Scatter_custom(&p[0], 3, MPI_FLOAT, &dest[0], 3, MPI_FLOAT, 0,
                     MPI_COMM_WORLD);
  MPI_Gather(&dest[0], 3, MPI_FLOAT, &d[0], 3, MPI_FLOAT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    EXPECT_EQ(p, d);
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

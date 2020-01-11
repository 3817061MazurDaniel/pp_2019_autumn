// Copyright 2019 Kondrina Tatyana

#include <mpi.h>
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>

#include <numeric>

#include "../../../modules/task_2/kondrina_t_broadcast/broadcast.h"

TEST(Bcast_MPI, Test_INT) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int data;
  int sum = 0;

  if (rank == 0) {
    data = 7;
  }
  bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
  data++;

  MPI_Reduce(&data, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    ASSERT_EQ(sum, data * size);
  }
}

TEST(Bcast_MPI, Test_FLOAT) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  float data;
  float sum = 0;

  if (rank == 0) {
    data = 7.35434;
  }
  bcast(&data, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  data++;

  MPI_Reduce(&data, &sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    ASSERT_EQ(sum, data * size);
  }
}

TEST(Bcast_MPI, Test_DOUBLE) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double data;
  double sum = 0;

  if (rank == 0) {
    data = 34.2363421;
  }
  bcast(&data, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  data++;

  MPI_Reduce(&data, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    ASSERT_EQ(sum, data * size);
  }
}

TEST(Bcast_MPI, Test_INT_arr) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  const int arrSize = 5;
  int data[arrSize]{};
  int sum = 0;

  if (rank == 0) {
    for (int index = 0; index < arrSize; index++) {
      data[index] = (index + 1) * 2;
    }
  }

  bcast(&data, arrSize, MPI_INT, 0, MPI_COMM_WORLD);
  for (int elem : data) {
    elem++;
  }

  int localSum = std::accumulate(&data[0], &data[arrSize], 0);

  MPI_Reduce(&localSum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    int expSum = std::accumulate(&data[0], &data[arrSize], 0) * size;
    ASSERT_EQ(sum, expSum);
  }
}

TEST(Bcast_MPI, Test_FLOAT_arr) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  const int arrSize = 5;
  float data[arrSize]{};
  float sum = 0;

  if (rank == 0) {
    for (int index = 0; index < arrSize; index++) {
      data[index] = index + size * size + .08678;
    }
  }

  bcast(&data, arrSize, MPI_FLOAT, 0, MPI_COMM_WORLD);
  for (float elem : data) {
    elem++;
  }

  float localSum = std::accumulate(&data[0], &data[arrSize], 0.0f);

  MPI_Reduce(&localSum, &sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    float expSum = std::accumulate(&data[0], &data[arrSize], 0.0f) * size;
    ASSERT_EQ(sum, expSum);
  }
}

TEST(Bcast_MPI, Test_DOUBLE_arr) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  const int arrSize = 5;
  double data[arrSize]{};
  double sum = 0;

  if (rank == 0) {
    for (int index = 0; index < arrSize; index++) {
      data[index] = index + size * size + .8798612;
    }
  }

  bcast(&data, arrSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  for (double elem : data) {
    elem++;
  }

  double localSum = std::accumulate(&data[0], &data[arrSize], 0.0);

  MPI_Reduce(&localSum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    double expSum = std::accumulate(&data[0], &data[arrSize], 0.0) * size;
    ASSERT_EQ(sum, expSum);
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

// Copyright 2019 Mazur Daniil
#include <mpi.h>
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <random>
#include "../../../modules/task_2/mazur_d_reader_writer/reader_writer.h"

#define GTEST_COUT std::cerr << "[ INFO ] "

TEST(Writers_Readers_Problem, writersCount_is_not_valid_is_negative) {
  int size, writersCount, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  writersCount = -1;
  if (rank == 0)
    GTEST_COUT << "writersCount is wrong. Cannot be < 0\n";
  ASSERT_ANY_THROW(startTask(writersCount));
}

TEST(Writers_Readers_Problem, writersCount_is_not_valid_is_bad) {
  int size, writersCount, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  writersCount = size;
  if (rank == 0)
    GTEST_COUT << "writersCount is wrong. Cannot be > then (size - 2)\n";
  ASSERT_ANY_THROW(startTask(writersCount));
}

TEST(Writers_Readers_Problem, writersCount_is_fine) {
  int size, writersCount, rankM;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  writersCount = 2;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankM);
  if (writersCount < size - 2 && writersCount > 0) {
    ASSERT_EQ(startTask(writersCount), 0);
  } else {
    ASSERT_ANY_THROW(startTask(writersCount));
  }
}

TEST(Writers_Readers_Problem, writersCount_is_fine2) {
  int size, writersCount, rankM;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  writersCount = 3;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankM);
  if (writersCount < size - 2 && writersCount > 0) {
    ASSERT_EQ(startTask(writersCount), 0);
  } else {
    ASSERT_ANY_THROW(startTask(writersCount));
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

// Copyright 2019 Shashkin Evgeny
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "../../../modules/task_2/shashkin_e_gather/gather.h"

TEST(Gather, same_types_int) {
  int root = 0;
  int sendcount = 500;
  int rank, size;
  int* sendbuf = new int[sendcount];
  int* recvbuf = nullptr;
  int* expectedbuf = nullptr;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int k = rank * sendcount;

  for (int i = 0; i < sendcount; ++i) {
    sendbuf[i] = ++k;
  }

  if (rank == 0) {
    recvbuf = new int[size * sendcount];
    expectedbuf = new int[size * sendcount];
  }

  Gather(sendbuf, sendcount, MPI_INT,
    recvbuf, sendcount, MPI_INT, root, MPI_COMM_WORLD);
  MPI_Gather(sendbuf, sendcount, MPI_INT,
    expectedbuf, sendcount, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == 0) {
    for (int i = 0; i < size * sendcount; ++i) {
      ASSERT_EQ(expectedbuf[i], recvbuf[i]);
    }
  }
}

TEST(Gather, same_types_double) {
  int root = 0;
  int sendcount = 500;
  int rank, size;
  double* sendbuf = new double[sendcount];
  double* recvbuf = nullptr;
  double* expectedbuf = nullptr;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double k = rank * sendcount;

  for (int i = 0; i < sendcount; ++i) {
    sendbuf[i] = ++k;
  }

  if (rank == 0) {
    recvbuf = new double[size * sendcount];
    expectedbuf = new double[size * sendcount];
  }

  Gather(sendbuf, sendcount, MPI_DOUBLE, recvbuf, sendcount, MPI_DOUBLE, root, MPI_COMM_WORLD);
  MPI_Gather(sendbuf, sendcount, MPI_DOUBLE, expectedbuf, sendcount, MPI_DOUBLE, root, MPI_COMM_WORLD);

  if (rank == 0) {
    for (int i = 0; i < size * sendcount; ++i) {
      ASSERT_EQ(expectedbuf[i], recvbuf[i]);
    }
  }
}

TEST(Gather, same_types_char) {
  int root = 0;
  int sendcount = 500;
  int rank, size;
  char* sendbuf = new char[sendcount];
  char* recvbuf = nullptr;
  char* expectedbuf = nullptr;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  char k = rank * sendcount;

  for (int i = 0; i < sendcount; ++i) {
    sendbuf[i] = ++k;
  }

  if (rank == 0) {
    recvbuf = new char[size * sendcount];
    expectedbuf = new char[size * sendcount];
  }

  Gather(sendbuf, sendcount, MPI_CHAR, recvbuf, sendcount, MPI_CHAR, root, MPI_COMM_WORLD);
  MPI_Gather(sendbuf, sendcount, MPI_CHAR, expectedbuf, sendcount, MPI_CHAR, root, MPI_COMM_WORLD);

  if (rank == 0) {
    for (int i = 0; i < size * sendcount; ++i) {
      ASSERT_EQ(expectedbuf[i], recvbuf[i]);
    }
  }
}

TEST(Gather, throw_when_root_less_then_zero) {
  int root = -1;
  int sendcount = 500;
  int rank, size;
  int* sendbuf = new int[sendcount];
  int* recvbuf = nullptr;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0)
    recvbuf = new int[size*sendcount];

  ASSERT_ANY_THROW(
    Gather(sendbuf, sendcount, MPI_INT, recvbuf, sendcount,
      MPI_INT, root, MPI_COMM_WORLD));
}

TEST(Gather, throw_when_incorrect_sendcount) {
  int root = 0;
  int sendcount = 0;
  int rank, size;
  int* sendbuf = new int[sendcount];
  int* recvbuf = nullptr;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  ASSERT_ANY_THROW(
    Gather(sendbuf, sendcount, MPI_INT, recvbuf, sendcount,
      MPI_INT, root, MPI_COMM_WORLD));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}


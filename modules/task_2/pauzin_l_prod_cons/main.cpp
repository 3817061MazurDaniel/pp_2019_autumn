// Copyright 2019 Pauzin Leonid
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <stdio.h>
#include <mpi.h>
#include "./pauzin_l_prod_cons.h"



TEST(Parallel_ProdCons, test1) {
  int data = 6;
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  if (world_rank == 0) {
    ParallelFunc(1, &data);
  } else {
    ParallelFunc(1, &data);
    EXPECT_EQ(data, 6);
  }
}

TEST(Parallel_ProdCons, SimpleTest) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  const int n = 100;
  const int delta = 2 * n / size;
  int* mas = new int[delta];
  if (rank % 2 == 0) {
    for (int i = 0; i < delta; i++) {
      mas[i] = i;
    }
    ParallelFunc(delta, mas);
  } else {
    ParallelFunc(delta, mas);
    for (int i = 0; i < delta; i++) {
      ASSERT_EQ(mas[i], i);
    }
  }
}

TEST(Parallel_ProdCons, sum) {
  int rank, size, n = 100;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int* a = new int[n];
  int sum = 0;

  if (rank % 2) {
    for (int i = 0; i < n; i++) {
      a[i] = 0;
    }
  }
  ParallelFunc(n, a);
  a[rank] = 1;
  MPI_Reduce(&a[rank], &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    ASSERT_EQ(sum, size);
  }
}


TEST(Parallel_ProdCons, throw_with_wrong_count_of_elems) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  const int n = 0;
  int data = 6;
  if (rank == 0) {
    ASSERT_ANY_THROW(ParallelFunc(n, &data));
  }
}

TEST(Parallel_ProdCons, Test_with_remainder) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  const int n = 183;
  const int delta = 2 * n / size;
  int* mas = new int[delta];
  if (size > 1) {
    if (rank % 2 == 0) {
      for (int i = 0; i < delta; i++) {
        mas[i] = i;
      }
      if (rank == 0) {
        int remaining = n % (size / 2);
        for (int i = 0; i < delta + remaining; i++) {
          mas[i] = i;
        }
        ParallelFunc(delta + remaining, mas);
      } else {
        ParallelFunc(delta, mas);
      }
    } else {
      if (rank != 1) {
        ParallelFunc(delta, mas);
      } else {
        int remaining = n % (size / 2);
        ParallelFunc(delta + remaining, mas);
        for (int i = 0; i < delta + remaining; i++) {
          ASSERT_EQ(mas[i], i);
        }
      }
    }
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

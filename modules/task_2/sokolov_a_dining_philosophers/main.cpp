// Copyright 2019 Sokolov Andrey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <array>
#include "../../../modules/task_2/sokolov_a_dining_philosophers/dining_philosophers.h"

TEST(Dining_Philosophers_MPI, Sum_Mode_with_3_elements) {
  int mode          {-1};
  int size          {3};
  int rank          {0};
  int comm_size     {0};
  int gold_result   {0};
  int local_result  {0};
  int global_result {0};

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  if (rank == 0) {
    startTable(comm_size, size);

    for (int proc = 1; proc < comm_size; proc++) {
      gold_result += proc + (proc + 1) + (proc + 2);
    }
  } else {
    std::array<int, 3U> mas{ { rank, rank + 1, rank + 2 } };

    startPilosopher(&local_result, rank, mas.data(), comm_size, mode, size);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Reduce(&local_result, &global_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    ASSERT_EQ(global_result, gold_result);
  }
}

TEST(Dining_Philosophers_MPI, Sum_Mode_with_10_elements) {
  int mode          {-1};
  int size          {10};
  int rank          {0};
  int comm_size     {0};
  int gold_result   {0};
  int local_result  {0};
  int global_result {0};

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  if (rank == 0) {
    startTable(comm_size, size);

    for (int proc = 1; proc < comm_size; proc++) {
      gold_result += proc + (proc + 1) + (proc + 2) + (proc + 3) + (proc + 4) +\
                     (proc + 5) + (proc + 6) + (proc + 7) + (proc + 8) + (proc + 9);
    }
  } else {
    std::array<int, 10U> mas{ { rank, rank + 1, rank + 2, rank + 3, rank + 4,
                                rank + 5, rank + 6, rank + 7, rank + 8, rank + 9 } };

    startPilosopher(&local_result, rank, mas.data(), comm_size, mode, size);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Reduce(&local_result, &global_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    ASSERT_EQ(global_result, gold_result);
  }
}

TEST(Dining_Philosophers_MPI, Multiplication_Mode_with_3_elements) {
  int mode          {-2};
  int size          {3};
  int rank          {0};
  int comm_size     {0};
  int gold_result   {0};
  int local_result  {1};
  int global_result {0};

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  if (rank == 0) {
    startTable(comm_size, size);

    for (int proc = 1; proc < comm_size; proc++) {
      gold_result += proc * (proc + 1) * (proc + 2);
    }
  } else {
    std::array<int, 3U> mas{ { rank, rank + 1, rank + 2 } };

    startPilosopher(&local_result, rank, mas.data(), comm_size, mode, size);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Reduce(&local_result, &global_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    global_result--;
    ASSERT_EQ(global_result, gold_result);
  }
}

TEST(Dining_Philosophers_MPI, Multiplication_Mode_with_10_elements) {
  int mode          {-2};
  int size          {10};
  int rank          {0};
  int comm_size     {0};
  int gold_result   {0};
  int local_result  {1};
  int global_result {0};

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  if (rank == 0) {
    startTable(comm_size, size);

    for (int proc = 1; proc < comm_size; proc++) {
      gold_result += proc * (proc + 1) * (proc + 2) * (proc + 3) * (proc + 4) * \
                     (proc + 5) * (proc + 6) * (proc + 7) * (proc + 8)  * (proc + 9);
    }
  } else {
    std::array<int, 10U> mas{ { rank, rank + 1, rank + 2, rank + 3, rank + 4,
                                rank + 5, rank + 6, rank + 7, rank + 8, rank + 9 } };

    startPilosopher(&local_result, rank, mas.data(), comm_size, mode, size);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Reduce(&local_result, &global_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    global_result--;
    ASSERT_EQ(global_result, gold_result);
  }
}

TEST(Dining_Philosophers_MPI, Default_Mode_with_5_elements) {
  int mode          {0};
  int size          {5};
  int rank          {0};
  int comm_size     {0};
  int gold_result   {0};
  int local_result  {0};
  int global_result {0};

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  if (rank == 0) {
    startTable(comm_size, size);

    for (int proc = 1; proc < comm_size; proc++) {
      gold_result += proc + 4;
    }
  } else {
    std::array<int, 5U> mas{ { rank, rank + 1, rank + 2, rank + 3, rank + 4 } };

    startPilosopher(&local_result, rank, mas.data(), comm_size, mode, size);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Reduce(&local_result, &global_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    ASSERT_EQ(global_result, gold_result);
  }
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

// Copyright 2019 Golovanova Elena
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "../../../modules/task_2/golovanova_e_sleeping_barber/sleeping_barber.h"


TEST(Sleeping_Barber, test_1) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size < 2) {
    ASSERT_ANY_THROW(SleepingBarber());
  }
}

TEST(Sleeping_Barber, test_2) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size > 1) {
    ASSERT_NO_THROW(SleepingBarber());
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

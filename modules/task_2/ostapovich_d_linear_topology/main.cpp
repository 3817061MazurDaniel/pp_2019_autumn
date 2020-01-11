// Copyright 2019 Ostapovich Denis

#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <vector>
#include "./linear.h"

TEST(Bus_topology, Test_isBus_returns_false_on_non_cortesian_comm) {
  EXPECT_FALSE(isBus(MPI_COMM_WORLD));
}

TEST(Bus_topology, Test_isBus_returns_false_on_cortesian_comm_with_ndims_greater_than_1) {
  MPI_Comm cartesian;
  int size;
  int nDims = 2;
  int dims[] = { 0, 0 };
  int periodic[] = { 0, 0 };
  int reorder = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Dims_create(size, nDims, dims);
  MPI_Cart_create(MPI_COMM_WORLD, nDims, dims, periodic, reorder, &cartesian);
  EXPECT_FALSE(isBus(cartesian));
  MPI_Comm_free(&cartesian);
}

TEST(Bus_topology, Test_isBus_returns_false_on_periodic_cartesian) {
  MPI_Comm cartesian;
  int size;
  int nDims = 1;
  int dims[] = { 0 };
  int periodic[] = { 1 };
  int reorder = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Dims_create(size, nDims, dims);
  MPI_Cart_create(MPI_COMM_WORLD, nDims, dims, periodic, reorder, &cartesian);
  EXPECT_FALSE(isBus(cartesian));
  MPI_Comm_free(&cartesian);
}

TEST(Bus_topology, Test_isBus_returns_true_on_bus_topology) {
  MPI_Comm cartesian;
  int size;
  int dims[] = { 0 };
  int periodic[] = { 0 };
  int reorder = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Dims_create(size, NUM_OF_DIMENSIONS, dims);
  MPI_Cart_create(MPI_COMM_WORLD, NUM_OF_DIMENSIONS, dims, periodic, reorder, &cartesian);
  EXPECT_TRUE(isBus(cartesian));
  MPI_Comm_free(&cartesian);
}

TEST(Bus_toplogy, Test_createBus_returns_bus_toplogy) {
  MPI_Comm comm = createBus(MPI_COMM_WORLD);
  EXPECT_TRUE(isBus(comm));
  MPI_Comm_free(&comm);
}

TEST(Bus_toplogy, Test_pingBus_throws_exception_on_non_bus_toplogy) {
  ASSERT_ANY_THROW(pingBus(MPI_COMM_WORLD));
}

TEST(Bus_toplogy, Test_pingBus_no_throws_on_bus_topology) {
  MPI_Comm comm = createBus(MPI_COMM_WORLD);
  ASSERT_NO_THROW(pingBus(comm));
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

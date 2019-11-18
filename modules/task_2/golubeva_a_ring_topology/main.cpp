  // Copyright 2019 Golubeva Anna
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cstdlib>
#include <cstring>
#include "../../../modules/task_2/golubeva_a_ring_topology/ring_topology.h"

TEST(Ring_Topology, Can_Create_Ring_Topology) {
  MPI_Comm commRing = createRingComm(MPI_COMM_WORLD);
  ASSERT_TRUE(testRingTopology(commRing));
}

TEST(Ring_Topology, Comm_Is_Ring) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm commRing = createRingComm(MPI_COMM_WORLD);
  if (rank == 0) {
    ASSERT_TRUE(isRingTopology(commRing));
  }
}

TEST(Ring_Topology, Not_Cart_Comm_Has_No_Ring_Topology) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    ASSERT_FALSE(isRingTopology(MPI_COMM_WORLD));
  }
}

TEST(Ring_Topology, False_If_Not_Ring_Topology) {
  ASSERT_FALSE(testRingTopology(MPI_COMM_WORLD));
}

TEST(Ring_Topology, False_If_Ring_Comm_Has_Not_One_Dim) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm commRing;
  int dims[3]{ 0 };
  int periods[3]{ 1 };
  MPI_Dims_create(size, 3, dims);
  MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, 0, &commRing);
  if (rank == 0) {
    ASSERT_FALSE(isRingTopology(commRing));
  }
}


TEST(Ring_Topology, Create_Comm_With_Different_Size) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm commRing = createRingComm(MPI_COMM_WORLD, 4);
  if (rank == 0) {
    ASSERT_TRUE(isRingTopology(commRing));
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

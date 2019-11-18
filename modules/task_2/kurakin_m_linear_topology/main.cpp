// Copyright 2019 Kurakin Mikhail
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./linear_topology.h"
TEST(Linear_Topology_MPI, Linear_Comm_Can_Be_Only_One_Dim) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm commLinear;
    int dims[2]{0};
    int periods[2]{0};
    MPI_Dims_create(size, 2, dims);
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &commLinear);
    if (rank == 0) {
        EXPECT_FALSE(isLinearTopology(commLinear));
    }
}

TEST(Linear_Topology_MPI, Linear_Comm_Can_Not_Be_Periodic) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm commLinear;
    int dims[1]{0};
    int periods[1]{1};
    MPI_Dims_create(size, 1, dims);
    MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periods, 0, &commLinear);
    if (rank == 0) {
        EXPECT_FALSE(isLinearTopology(commLinear));
    }
}

TEST(Linear_Topology_MPI, Linear_Comm_Can_Be_Only_Cart) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        EXPECT_FALSE(isLinearTopology(MPI_COMM_WORLD));
    }
}

TEST(Linear_Topology_MPI, Created_Commincator_Is_Linear) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm commLinear = createLinearComm(MPI_COMM_WORLD);
    if (rank == 0) {
        EXPECT_TRUE(isLinearTopology(commLinear));
    }
}

TEST(Linear_Topology_MPI, Created_Communicator_With_Diff_Size_Is_Linear) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm commLinear = createLinearComm(MPI_COMM_WORLD, 2);
    if (rank == 0) {
        EXPECT_TRUE(isLinearTopology(commLinear));
        MPI_Comm_free(&commLinear);
    }
}

TEST(Linear_Topology_MPI, Test_Communication_Linear_Topology) {
    MPI_Comm commLinear = createLinearComm(MPI_COMM_WORLD);
    EXPECT_TRUE(testLinearTopology(commLinear));
}

TEST(Linear_Topology_MPI, Can_Not_Communicate_With_Not_Linear_Topology) {
    EXPECT_FALSE(testLinearTopology(MPI_COMM_WORLD));
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

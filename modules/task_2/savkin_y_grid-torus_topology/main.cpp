// Copyright 2019 Savkin Yuriy
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include "./grid-torus_topology.h"

TEST(Grid_Torus_Topology, can_create_topology) {
    MPI_Comm torus_comm = getTorusComm(MPI_COMM_WORLD);

    int r;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);

    if (r == 0) {
        ASSERT_TRUE(testTorusTopology(torus_comm));
    }
}

TEST(Grid_Torus_Topology, not_cart_comm_has_no_torus_topology) {
    int r;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);

    if (r == 0) {
        ASSERT_FALSE(testTorusTopology(MPI_COMM_WORLD));
    }
}

TEST(Grid_Torus_Topology, non_periodic_comm_has_no_torus_topology) {
    MPI_Comm my_comm;

    int r;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int dims[] = { 0, 0 };
    int periods[] = { 1, 0 };
    MPI_Dims_create(size, 2, dims);
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &my_comm);

    if (r == 0) {
        ASSERT_FALSE(testTorusTopology(my_comm));
    }
}

TEST(Grid_Torus_Topology, not_2_dims_comm_has_no_torus_topology) {
    MPI_Comm my_comm;

    int r;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int dims[] = { 0, 0, 0 };
    int periods[] = { 1, 1, 1 };
    MPI_Dims_create(size, 3, dims);
    MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, 1, &my_comm);

    if (r == 0) {
        ASSERT_FALSE(testTorusTopology(my_comm));
    }
}

TEST(Grid_Torus_Topology, can_create_with_specified_size) {
    MPI_Comm torus_comm = getTorusComm(MPI_COMM_WORLD, 1);

    int r;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);

    if (r == 0) {
        ASSERT_TRUE(testTorusTopology(torus_comm));
    }
}

TEST(Grid_Torus_Topology, cant_create_with_wrong_size) {
    int r;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm my_comm = getTorusComm(MPI_COMM_WORLD, size + 1);

    if (r == 0) {
        ASSERT_EQ(my_comm, MPI_COMM_NULL);
    }
}

TEST(Grid_Torus_Topology, shift_works_correctly) {
    MPI_Comm torus_comm = getTorusComm(MPI_COMM_WORLD);

    int r;
    MPI_Comm_rank(torus_comm, &r);

    bool f = testCommunications(torus_comm);

    if (r == 0) {
        ASSERT_TRUE(f);
    }
}

TEST(Grid_Torus_Topology, shift_dont_works_on_incorrect_comm) {
    MPI_Comm my_comm;

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int dims[] = { 0, 0 };
    int periods[] = { 1, 0 };
    MPI_Dims_create(size, 2, dims);
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &my_comm);

    int r;
    MPI_Comm_rank(my_comm, &r);

    bool f = testCommunications(my_comm);

    if (r == 0) {
        ASSERT_FALSE(f);
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

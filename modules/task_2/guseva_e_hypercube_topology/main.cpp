// Copyright 2019 Guseva Catherine
#include <gtest/gtest.h>
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include "./Hypercube_topology.h"

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

TEST(Hypercube_Topology_MPI, Can_Creat_Hypercube_Comm) {
    int rank, proc;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc);

    if (proc == 8) {
        MPI_Comm temp_comm = createHypercube(3, 2);

        if (rank == 0) {
            ASSERT_TRUE(isHypercubeTop(temp_comm, 3, 2));
        }
    }
}

TEST(Hypercube_Topology_MPI, Can_Transfer_Data_Right) {
    int rank, proc;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc);

    if (proc == 8) {
        MPI_Comm temp_comm = createHypercube(3, 2);
        bool temp_result = testHypercubeTopDATA(temp_comm, 3, 2);

        if (rank == 0) {
            ASSERT_TRUE(temp_result);
        }
    }
}

TEST(Hypercube_Topology_MPI, Wrong_Size_tempDims) {
    ASSERT_ANY_THROW(createHypercube(-6, 4));
}

TEST(Hypercube_topology_MPI, Wrong_Size_sizeHypercube) {
    ASSERT_ANY_THROW(createHypercube(4, -6));
}

TEST(Hypercube_Topology_MPI, Wrong_Arguments_In_isHypercubeTop) {
    int rank, proc;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc);

    if (proc == 8) {
        MPI_Comm temp_comm = createHypercube(3, 2);

        if (rank == 0) {
            ASSERT_TRUE(isHypercubeTop(temp_comm, 3, 4));
        }
    }
}

TEST(Hypercube_Topology_MPI, Does_Not_Have_Periods) {
    int rank, proc;
    MPI_Comm temp_comm;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc);

    if (proc == 8) {
        int dims[] = { 2, 2, 2 };
        int periods[] = { 0, 0, 0 };

        MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, 1, &temp_comm);

        if (rank == 0) {
            ASSERT_FALSE(isHypercubeTop(temp_comm, 3, 2));
        }
    }
}

TEST(Hypercube_topology_MPI, Transfer_Data_Wrong) {
    int rank, proc;
    MPI_Comm temp_comm;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc);

    if (proc == 8) {
        int dims[] = { 2, 2, 2 };
        int periods[] = { 1, 0, 1 };

        MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, 1, &temp_comm);
        bool temp_result = testHypercubeTopDATA(temp_comm, 3, 2);

        if (rank == 0) {
            ASSERT_FALSE(temp_result);
        }
    }
}

// Copyright 2019 Shemetov Philipp
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include "./network_top_hypercube.h"

TEST(Network_Top_Hypercube, Test_Wrong_Size_Dimension_Of_Hypercube) {
    ASSERT_ANY_THROW(getHypercube(-1, 2));
}

TEST(Network_Top_Hypercube, Test_Wrong_Size_Node_Of_Hypercube) {
    ASSERT_ANY_THROW(getHypercube(2, -1));
}


TEST(Network_Top_Hypercube, Test_This_Hypercube_Has_Not_Right_Dimension) {
    int rankProc, sizeProc;

    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    if (sizeProc == 8) {
        MPI_Comm actual_comm = getHypercube(3, 2);

        if (rankProc == 0) {
            ASSERT_FALSE(thisIsHypercube(actual_comm, 4, 2));
        }
    }
}

TEST(Network_Top_Hypercube, Test_This_Hypercube_Has_Right_Arguments) {
    int rankProc, sizeProc;

    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    if (sizeProc == 8) {
        MPI_Comm actual_comm = getHypercube(3, 2);
        if (rankProc == 0) {
            ASSERT_TRUE(thisIsHypercube(actual_comm, 3, 2));
        }
    }
}

TEST(Network_Top_Hypercube, Test_Can_Create_Hypercube_Comm) {
    int rankProc, sizeProc;

    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    if (sizeProc == 8) {
        MPI_Comm actual_comm = getHypercube(3, 2);
        if (rankProc == 0) {
            ASSERT_TRUE(thisIsHypercube(actual_comm, 3, 2));
        }
    }
}

TEST(Network_Top_Hypercube, TEST_Topology_Hypercube_Does_Not_Have_Periods) {
    MPI_Comm test_comm;
    int rankProc, sizeProc;

    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    if (sizeProc == 8) {
        int dims[] = { 2, 2, 2 };
        int periods[] = { 0, 0, 0 };

        MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, 1, &test_comm);

        if (rankProc == 0) {
            ASSERT_FALSE(thisIsHypercube(test_comm, 3, 2));
        }
    }
}

TEST(Network_Top_Hypercube, TEST_Transfer_Data_Work_Correctly) {
    int rankProc, sizeProc;

    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    if (sizeProc == 8) {
        MPI_Comm actual_comm = getHypercube(3, 2);
        bool condition = testHypercubeDataTransfer(actual_comm, 3, 2);

        if (rankProc == 0) {
            ASSERT_TRUE(condition);
        }
    }
}

TEST(Network_Top_Hypercube, TEST_Transfer_Data_Not_Work_Correctly) {
    MPI_Comm test_comm;
    int rankProc, sizeProc;

    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    if (sizeProc == 8) {
        int dims[] { 2, 2, 2 };
        int periods[] { 1, 1, 0 };

        MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, 1, &test_comm);
        bool condition = testHypercubeDataTransfer(test_comm, 3, 2);

        if (rankProc == 0) {
            ASSERT_FALSE(condition);
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

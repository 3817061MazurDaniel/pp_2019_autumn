// Copyright 2019 Okmyanskiy Andrey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./topology_line.h"

TEST(Parallel_Topology_Line_MPI, Test_Topolgy_Line_Works_Size_Number_x_100) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm LineComm = getLineComm(MPI_COMM_WORLD, ProcNum);
    int sizeVector = 100;

    // double startL = MPI_Wtime();
    std::vector<int> line(ProcNum*sizeVector);
    line = getMessage(ProcRank, ProcNum, LineComm, sizeVector);
    // double endL = MPI_Wtime() - startL;

    // double startW = MPI_Wtime();
    std::vector<int> world(ProcNum*sizeVector);
    world = getMessage(ProcRank, ProcNum, MPI_COMM_WORLD, sizeVector);
    // double endW = MPI_Wtime() - startW;

    if (ProcRank == 0) {
        // printf("TotalTimeL: %f\n", endL);
        // printf("TotalTimeW: %f\n", endW);
        ASSERT_EQ(world, line);
    }
    if (ProcNum != 1) {
        MPI_Comm_free(&LineComm);
    }
}

TEST(Parallel_Topology_Line_MPI, Test_Topolgy_Line_Works_Size_Number_x_1000) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm LineComm = getLineComm(MPI_COMM_WORLD, ProcNum);
    int sizeVector = 1000;

    // double startL = MPI_Wtime();
    std::vector<int> line(ProcNum*sizeVector);
    line = getMessage(ProcRank, ProcNum, LineComm, sizeVector);
    // double endL = MPI_Wtime() - startL;

    // double startW = MPI_Wtime();
    std::vector<int> world(ProcNum*sizeVector);
    world = getMessage(ProcRank, ProcNum, MPI_COMM_WORLD, sizeVector);
    // double endW = MPI_Wtime() - startW;

    if (ProcRank == 0) {
        // printf("TotalTimeL: %f\n", endL);
        // printf("TotalTimeW: %f\n", endW);
        ASSERT_EQ(world, line);
    }
    if (ProcNum != 1) {
        MPI_Comm_free(&LineComm);
    }
}

TEST(Parallel_Topology_Line_MPI, Test_Topolgy_Line_Works_Size_Number_x_10000) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm LineComm = getLineComm(MPI_COMM_WORLD, ProcNum);
    int sizeVector = 10000;

    // double startL = MPI_Wtime();
    std::vector<int> line(ProcNum*sizeVector);
    line = getMessage(ProcRank, ProcNum, LineComm, sizeVector);
    // double endL = MPI_Wtime() - startL;

    // double startW = MPI_Wtime();
    std::vector<int> world(ProcNum*sizeVector);
    world = getMessage(ProcRank, ProcNum, MPI_COMM_WORLD, sizeVector);
    // double endW = MPI_Wtime() - startW;

    if (ProcRank == 0) {
        // printf("TotalTimeL: %f\n", endL);
        // printf("TotalTimeW: %f\n", endW);
        ASSERT_EQ(world, line);
    }
    if (ProcNum != 1) {
        MPI_Comm_free(&LineComm);
    }
}

TEST(Parallel_Topology_Line_MPI, Test_Topology_Is_Graph) {
    int ProcRank, ProcNum, Status;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    MPI_Comm LineComm = getLineComm(MPI_COMM_WORLD, ProcNum);
    if (ProcRank == 0) {
        if (ProcNum == 1) {
            MPI_Topo_test(LineComm, &Status);
            ASSERT_EQ(Status, MPI_UNDEFINED);
        } else {
            MPI_Topo_test(LineComm, &Status);
            ASSERT_EQ(Status, MPI_GRAPH);
            MPI_Comm_free(&LineComm);
        }
    }
}

TEST(Parallel_Topology_Line_MPI, Test_Topology_Is_Line) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    MPI_Comm LineComm = getLineComm(MPI_COMM_WORLD, ProcNum);
    if (ProcNum == 1) {
        EXPECT_FALSE(isLineTopology(LineComm, ProcRank));
    } else {
        EXPECT_TRUE(isLineTopology(LineComm, ProcRank));
        MPI_Comm_free(&LineComm);
    }
}

TEST(Parallel_Topology_Line_MPI, Test_Topology_Is_Not_Line) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        EXPECT_FALSE(isLineTopology(MPI_COMM_WORLD, ProcRank));
    }
}

TEST(Parallel_Topology_Line_MPI, Test_Comm_Of_Topology_Is_Not_MPI_COMM_WORLD) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    MPI_Comm LineComm = getLineComm(MPI_COMM_WORLD, ProcNum);
    if (ProcNum == 1) {
        ASSERT_EQ(MPI_COMM_WORLD, LineComm);
    } else {
        ASSERT_NE(MPI_COMM_WORLD, LineComm);
        MPI_Comm_free(&LineComm);
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

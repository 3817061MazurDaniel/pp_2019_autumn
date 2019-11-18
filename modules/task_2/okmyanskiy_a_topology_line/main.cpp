// Copyright 2019 Okmyanskiy Andrey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./topology_line.h"

TEST(Parallel_Topology_Line_MPI, Test_Topolgy_Line_Works) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    int world = getMessage(ProcRank, ProcNum, MPI_COMM_WORLD);
    MPI_Comm LineComm = getLineComm(MPI_COMM_WORLD, ProcNum);
    int line = getMessage(ProcRank, ProcNum, LineComm);
    if (ProcRank == 0) {
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

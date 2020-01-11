// Copyright 2019 Kurakin Mikhail
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <vector>
#include "./star_topology.h"

TEST(Star_Topology_MPI, isStarTopology_Return_Fals_With_Not_Graph_Topo) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        EXPECT_FALSE(isStarTopology(MPI_COMM_WORLD));
    }
}

TEST(Star_Topology_MPI, isStarTopology_Return_Fals_With_Not_Star_Topo) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int* index = new int[size];
    int* edges = new int[size];
    for (int i = 0; i < size; i++) {
        index[i] = i + 1;
        edges[i] = i + 1;
    }

    int reorder = 0;
    MPI_Comm newComm;
    MPI_Graph_create(MPI_COMM_WORLD, size, index, edges, reorder, &newComm);

    if (rank == 0) {
        EXPECT_FALSE(isStarTopology(newComm));
    }
}

TEST(Star_Topology_MPI, isStarTopology_Return_True_With_Star_Topo) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int nnodes = size;

    int* index = new int[nnodes];
    for (int i = 0, j = nnodes - 1; i < nnodes; i++, j++)
        index[i] = j;

    int* edges = new int[(nnodes - 1) * 2];
    for (int i = 0; i < nnodes - 1; i++)
        edges[i] = i + 1;
    for (int i = nnodes - 1; i < (nnodes - 1) * 2; i++)
        edges[i] = 0;

    int reorder = 0;
    MPI_Comm newComm;
    MPI_Graph_create(MPI_COMM_WORLD, nnodes, index, edges, reorder, &newComm);

    if (rank == 0) {
        EXPECT_TRUE(isStarTopology(newComm));
    }
}

TEST(Star_Topology_MPI, Created_Commincator_Is_Star) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm commStar = createStarComm(MPI_COMM_WORLD);
    if (rank == 0) {
        EXPECT_TRUE(isStarTopology(commStar));
    }
}

TEST(Star_Topology_MPI, Created_Communicator_With_Diff_Size_Is_Star) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm commStar = createStarComm(MPI_COMM_WORLD);
    if (rank == 0) {
        EXPECT_TRUE(isStarTopology(commStar));
        MPI_Comm_free(&commStar);
    }
}

TEST(Star_Topology_MPI, Created_Star_Topology_Can_Find_Sum_Of_Vector_Values) {
    int len = 16;
    std::vector<int> vec(len);
    int answer = 0;
    int right_answer = 0;

    for (int i = 0; i < len; i++) {
        vec[i] = i;
        right_answer += i;
    }

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm commStar = createStarComm(MPI_COMM_WORLD);

    if (size > len) {
        for (int i = 0; i < len; i++) {
            answer += vec[i];
        }
    } else {
        int delta = len / size;
        int delta_step = len % size;

        if (rank == 0) {
            for (int proc = 1; proc < size; proc++) {
                MPI_Send(&vec[0] + proc * delta + delta_step, delta, MPI_INT,
                         proc, 0, commStar);
            }
        }

        std::vector<int> local_vec(delta);

        if (rank == 0) {
            local_vec = std::vector<int>(vec.begin(),
                                         vec.begin() + delta + delta_step);
        } else {
            MPI_Status st;
            MPI_Recv(&local_vec[0], delta, MPI_INT, 0, 0,
                     commStar, &st);
        }

        int result = 0;
        if (rank != 0) {
            for (int i = 0; i < delta; i++) {
                result += local_vec[i];
            }
        } else {
            for (int i = 0; i < delta + delta_step; i++) {
                result += local_vec[i];
            }
        }

        MPI_Barrier(commStar);
        MPI_Reduce(&result, &answer, 1, MPI_INT, MPI_SUM,
                    0, commStar);
    }

    if (rank == 0) {
        EXPECT_EQ(answer, right_answer);
        MPI_Comm_free(&commStar);
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

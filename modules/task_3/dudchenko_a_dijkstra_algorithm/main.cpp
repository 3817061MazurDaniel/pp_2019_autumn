// Copyright 2019 Dudchenko Anton

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "../../../modules/task_3/dudchenko_a_dijkstra_algorithm/dijkstra_algorithm.h"


TEST(Test_Dijsktra, Create_Graph) {
    int rank;
    size_t n = 5;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        EXPECT_EQ(n * n, generateGraph(n).size());
    }
}


TEST(Test_Dijsktra, Sequence_Dijkstra) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> graph = { 0, 7, 5, 3,
                               7, 0, 3, 1,
                               5, 3, 0, 4,
                               3, 1, 4, 0 };
    if (rank == 0) {
        std::vector<int> result = sequenceDijkstra(graph, 1);
        std::vector<int> expectResult = { 4, 0, 3, 1 };
        EXPECT_EQ(expectResult, result);
    }
}


TEST(Test_Dijsktra, Parallel_Dijkstra) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> graph = { 0, 7, 5, 3,
                               7, 0, 3, 1,
                               5, 3, 0, 4,
                               3, 1, 4, 0 };
    std::vector<int> result = parallelDijkstra(graph, 1);
    if (rank == 0) {
        std::vector<int> expectResult = { 4, 0, 3, 1 };
        EXPECT_EQ(expectResult, result);
    }
}

TEST(Test_Dijsktra, Compare_Little_Size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 4;
    std::vector<int> graph = generateGraph(n);
    std::vector<int> parallelResult = parallelDijkstra(graph, n / 2);
    if (rank == 0) {
        std::vector<int> sequenceResult = sequenceDijkstra(graph, n / 2);
        EXPECT_EQ(sequenceResult, parallelResult);
    }
}

TEST(Test_Dijsktra, Compare_Biiiig_Size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 1200;
    std::vector<int> graph = generateGraph(n);
    std::vector<int> parallelResult = parallelDijkstra(graph, n / 2);
    if (rank == 0) {
        std::vector<int> sequenceResult = sequenceDijkstra(graph, n / 2);
        EXPECT_EQ(sequenceResult, parallelResult);
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

// Copyright 2019 Isaev Ilya
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "./dijkstra_alg.h"

TEST(Dijkstra_Alg, CAN_CREATE_GRAPH) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        ASSERT_NO_THROW(Graph(5));
    }
}

TEST(Dijkstra_Alg, THROWS_ON_NEGATIVE_V_SIZE) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        ASSERT_ANY_THROW(Graph(-5));
    }
}

TEST(Dijkstra_Alg, SEQUENTIAL_AND_PARALLEL_HAVE_SAME_ANSWER_UNDIRECTED) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int nv = 500;
    Graph graph(nv);
    std::vector<int> seqres;
    std::vector<int> parres;
    if (rank == 0) {
        graph.setAdjacency(createRandomGraph(nv));
        seqres = dijkstraSequential(graph, 0);
    }
    parres = dijkstraParallel(graph, 0);
    if (rank == 0) {
        ASSERT_EQ(seqres, parres);
    }
}

TEST(Dijkstra_Alg, SEQUENTIAL_AND_PARALLEL_HAVE_SAME_ANSWER_DIRECTED) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int nv = 500;
    Graph graph(nv);
    std::vector<int> seqres;
    std::vector<int> parres;
    if (rank == 0) {
        graph.setAdjacency(createRandomGraph(nv, DIRECTED));
        seqres = dijkstraSequential(graph, 0);
    }
    parres = dijkstraParallel(graph, 0);
    if (rank == 0) {
        ASSERT_EQ(seqres, parres);
    }
}

TEST(Dijkstra_Alg, THROWS_ON_WRONG_SOURCE) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        Graph graph(5);
        ASSERT_ANY_THROW(dijkstraParallel(graph, 7));
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

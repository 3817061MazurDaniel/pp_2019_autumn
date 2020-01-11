// Copyright 2019 Lebedev Alexander
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <exception>

#include "./sparse_matrix_mult.h"

TEST(sparse_matrix_mult, transpanent_test) {
    int rank;
    int root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == root) {
        ccsMatrix a;
        ccsMatrix b;
        ccsMatrix expected;
        expected.init(3, {2, 4, 1, 3}, {1, 2, 0, 2}, {0, 2, 3, 4});
        a.init(3, {1, 2, 4, 3}, {1, 0, 0, 2}, {0, 1, 2, 4});
        a.transpanent();
        ASSERT_EQ(a, expected);
    }
}

TEST(sparse_matrix_mult, simple_sequential_mult) {
    int rank;
    int root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == root) {
        ccsMatrix a;
        ccsMatrix b;
        ccsMatrix expected;
        a.init(3, {1, 2, 4, 3}, {1, 0, 0, 2}, {0, 1, 2, 4});
        b.init(3, {1, 3, 2}, {0, 2, 1}, {0, 2, 2, 3});
        expected.init(3, {12, 4, 1, 9}, {0, 2, 0, 0}, {0, 2, 3, 4});
        ccsMatrix result = sparseMatrixMultSequential(a, b);
        ASSERT_EQ(result, expected);
    }
}

TEST(sparse_matrix_mult, simple_parallel_mult) {
    int rank;
    int root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    ccsMatrix a;
    ccsMatrix b;
    ccsMatrix expected;

    if (rank == root) {
        a.init(3, {1, 2, 4, 3}, {1, 0, 0, 2}, {0, 1, 2, 4});
        b.init(3, {1, 3, 2}, {0, 2, 1}, {0, 2, 2, 3});
        expected.init(3, {12, 4, 1, 9}, {0, 2, 0, 0}, {0, 2, 3, 4});
    }

    ccsMatrix result = sparseMatrixMultParallel(a, b);

    if (rank == root) {
        ASSERT_EQ(result, expected);
    }
}

TEST(sparse_matrix_mult, zero_test_1) {
    int rank;
    int root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    ccsMatrix a;
    ccsMatrix b;
    ccsMatrix expected;

    if (rank == root) {
        a.init(3, {}, {}, {0, 0, 0, 0});
        b.init(3, {}, {}, {0, 0, 0, 0});
        ccsMatrix cpA(3, {}, {}, {0, 0, 0, 0});
        expected.init(sparseMatrixMultSequential(cpA, b));
    }

    ccsMatrix result = sparseMatrixMultParallel(a, b);

    if (rank == root) {
        ASSERT_EQ(result, expected);
    }
}

TEST(sparse_matrix_mult, zero_test_2) {
    int rank;
    int root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    ccsMatrix a;
    ccsMatrix b;
    ccsMatrix expected;

    if (rank == root) {
        a.init(3, {2, 3}, {0, 1}, {0, 0, 2, 2});
        b.init(3, {0}, {0}, {0, 0, 0, 0});
        ccsMatrix cpA(3, {2, 3}, {0, 1}, {0, 0, 2, 2});
        expected.init(sparseMatrixMultSequential(cpA, b));
    }

    ccsMatrix result = sparseMatrixMultParallel(a, b);

    if (rank == root) {
        ASSERT_EQ(result, expected);
    }
}

TEST(sparse_matrix_mult, parallel_and_sequential_test_n_5) {
    int rank;
    int root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 5;

    ccsMatrix a;
    ccsMatrix b;
    ccsMatrix copyA;
    ccsMatrix expected;

    if (rank == root) {
        a.init(generateRandomMatrix(n));
        copyA.init(a);
        b.init(generateRandomMatrix(n));
        expected.init(sparseMatrixMultSequential(a, b));
    }

    ccsMatrix result = sparseMatrixMultParallel(copyA, b);

    if (rank == root) {
        ASSERT_EQ(result, expected);
    }
}

TEST(sparse_matrix_mult, parallel_and_sequential_test_n_10) {
    int rank;
    int root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 10;

    ccsMatrix a;
    ccsMatrix b;
    ccsMatrix copyA;
    ccsMatrix expected;

    if (rank == root) {
        a.init(generateRandomMatrix(n));
        copyA.init(a);
        b.init(generateRandomMatrix(n));
        expected.init(sparseMatrixMultSequential(a, b));
    }

    ccsMatrix result = sparseMatrixMultParallel(copyA, b);

    if (rank == root) {
        ASSERT_EQ(result, expected);
    }
}

TEST(sparse_matrix_mult, parallel_and_sequential_test_n_15) {
    int rank;
    int root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 15;

    ccsMatrix a;
    ccsMatrix b;
    ccsMatrix copyA;
    ccsMatrix expected;

    if (rank == root) {
        a.init(generateRandomMatrix(n));
        copyA.init(a);
        b.init(generateRandomMatrix(n));
        expected.init(sparseMatrixMultSequential(a, b));
    }

    ccsMatrix result = sparseMatrixMultParallel(copyA, b);

    if (rank == root) {
        ASSERT_EQ(result, expected);
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

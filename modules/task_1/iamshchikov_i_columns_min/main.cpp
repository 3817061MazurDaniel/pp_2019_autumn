// Copyright 2019 Iamshchikov Ivan
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./columns_min.h"

TEST(Parallel_Operations_MPI, cant_create_matrix_with_negative_scale) {
    ASSERT_ANY_THROW(std::vector <int> matrix(-1));
    size_t m = 3, n = -5;
    std::vector <int> matrix;
    std::vector <int>* pmatrix = &matrix;
    ASSERT_ANY_THROW(getRandomMatrix(pmatrix, n, m));
}

TEST(Parallel_Operations_MPI, can_create_matrix) {
    size_t m = 3, n = 5;
    std::vector <int> matrix;
    std::vector <int>* pmatrix = &matrix;
    getRandomMatrix(pmatrix, n, m);
    ASSERT_EQ(m*n, pmatrix->size());
}

TEST(Parallel_Operations_MPI, can_transpose_matrix_to_row) {
    int k = 0;
    const int m = 5, n = 6;
    std::vector <int> matrix;
    std::vector <int>* pmatrix = &matrix;
    std::vector<int> transposed_matrix(m*n);
    std::vector<int>* ptransposed_matrix = &transposed_matrix;
    getRandomMatrix(pmatrix, n, m);
    transposeMatrix(pmatrix, ptransposed_matrix, m, n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            ASSERT_EQ(pmatrix->at(j*n + i), ptransposed_matrix->at(k++));
}

TEST(Parallel_Operations_MPI, find_min_in_row) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int m = 1, n = 6;
    std::vector<int> res;
    std::vector <int> matrix;
    std::vector <int>* pmatrix = &matrix;
    if (rank == 0) {
        getRandomMatrix(pmatrix, n, m);
    }
    res = getMinElementsParallel(pmatrix, m, n);
    if (rank == 0) {
        ASSERT_EQ(getMinElementsSequential(pmatrix, m, n), res);
    }
}

TEST(Parallel_Operations_MPI, find_min_in_col) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int m = 6, n = 1;
    std::vector<int> res;
    std::vector <int> matrix;
    std::vector <int>* pmatrix = &matrix;
    if (rank == 0) {
        getRandomMatrix(pmatrix, n, m);
    }
    res = getMinElementsParallel(pmatrix, m, n);
    if (rank == 0) {
        ASSERT_EQ(getMinElementsSequential(pmatrix, m, n), res);
    }
}

TEST(Parallel_Operations_MPI, find_mins_in_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int m = 1000, n = 1000;
    std::vector<int> res;
    std::vector <int> matrix;
    std::vector <int>* pmatrix = &matrix;
    if (rank == 0) {
        getRandomMatrix(pmatrix, n, m);
    }
    res = getMinElementsParallel(pmatrix, m, n);
    if (rank == 0) {
        ASSERT_EQ(getMinElementsSequential(pmatrix, m, n), res);
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

// Copyright 2019 Iamshchikov Ivan
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./columns_min.h"

TEST(Parallel_Operations_MPI, cant_create_matrix_with_negative_scale) {
    ASSERT_ANY_THROW(std::vector <std::vector<int> > matrix(-1));
    std::vector <std::vector<int> > matrix(3);
    std::vector <std::vector<int> >* pmatrix = &matrix;
    ASSERT_ANY_THROW(getRandomMatrix(pmatrix, -5));
}

TEST(Parallel_Operations_MPI, can_create_matrix) {
    size_t m = 3, n = 5;
    std::vector <std::vector<int> > matrix(3);
    std::vector <std::vector<int> >* pmatrix = &matrix;
    getRandomMatrix(pmatrix, n);
    ASSERT_EQ(m, pmatrix->size());
    ASSERT_EQ(n, pmatrix->at(0).size());
}

TEST(Parallel_Operations_MPI, can_transform_matrix_to_row) {
    int k = 0;
    const int m = 5, n = 6;
    std::vector <std::vector<int> > matrix(m);
    std::vector <std::vector<int> >* pmatrix = &matrix;
    std::vector<int> matrix_in_row(m*n);
    std::vector<int>* pmatrix_in_row = &matrix_in_row;
    getRandomMatrix(pmatrix, n);
    matrixToRow(pmatrix, pmatrix_in_row, m, n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            ASSERT_EQ(pmatrix->at(j)[i], pmatrix_in_row->at(k++));
}

TEST(Parallel_Operations_MPI, find_min_in_row) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int m = 1, n = 6;
    std::vector<int> res;
    std::vector <std::vector<int> > matrix(m);
    std::vector <std::vector<int> >* pmatrix = &matrix;
    if (rank == 0) {
        getRandomMatrix(pmatrix, n);
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
    std::vector <std::vector<int> > matrix(m);
    std::vector <std::vector<int> >* pmatrix = &matrix;
    if (rank == 0) {
        getRandomMatrix(pmatrix, n);
    }
    res = getMinElementsParallel(pmatrix, m, n);
    if (rank == 0) {
        ASSERT_EQ(getMinElementsSequential(pmatrix, m, n), res);
    }
}

TEST(Parallel_Operations_MPI, find_mins_in_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int m = 5, n = 6;
    std::vector<int> res;
    std::vector <std::vector<int> > matrix(m);
    std::vector <std::vector<int> >* pmatrix = &matrix;
    if (rank == 0) {
        getRandomMatrix(pmatrix, n);
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

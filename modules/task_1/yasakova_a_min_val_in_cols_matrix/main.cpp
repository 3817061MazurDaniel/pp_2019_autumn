// Copyright 2019 Yasakova Anastasia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "../../../modules/task_1/yasakova_a_min_val_in_cols_matrix/min_val_in_cols_matrix.h"


TEST(Min_val_in_cols_matrix_MPI, test1_on_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_ANY_THROW(getRandomMatrix(-1, 3));
    }
}

TEST(Min_val_in_cols_matrix_MPI, test2_on_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_ANY_THROW(getRandomMatrix(2, 0));
    }
}

TEST(Min_val_in_cols_matrix_MPI, test3_on_matrix) {
    std::vector <int> a;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        a = getRandomMatrix(5, 7);
        ASSERT_ANY_THROW(ParallelMinInColsMatrix(a, 3, 2));
    }
}

TEST(Min_val_in_cols_matrix_MPI, test4_on_matrix) {
    int rows = 1, cols = 1;
    std::vector <int> a = getRandomMatrix(rows, cols);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector <int> ans(cols, rows * cols + 1);
    ans = ParallelMinInColsMatrix(a, rows, cols);
    std::vector <int> ans_seq(cols, rows * cols + 1);
    if (rank == 0) {
        ans_seq = SequentialMinInColsMatrix(a, rows, cols);
        ASSERT_EQ(ans_seq, ans);
    }
}

TEST(Min_val_in_cols_matrix_MPI, test5_on_matrix) {
    int rows = 1, cols = 3;
    std::vector <int> a = getRandomMatrix(rows, cols);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector <int> ans(cols, rows * cols + 1);
    ans = ParallelMinInColsMatrix(a, rows, cols);
    std::vector <int> ans_seq(cols, rows * cols + 1);
    if (rank == 0) {
        ans_seq = SequentialMinInColsMatrix(a, rows, cols);
        ASSERT_EQ(ans_seq, ans);
    }
}

TEST(Min_val_in_cols_matrix_MPI, test6_on_matrix) {
    int rows = 100, cols = 100;
    std::vector <int> a = getRandomMatrix(rows, cols);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector <int> ans(cols, rows * cols + 1);
    ans = ParallelMinInColsMatrix(a, rows, cols);
    std::vector <int> ans_seq(cols, rows * cols + 1);
    if (rank == 0) {
        ans_seq = SequentialMinInColsMatrix(a, rows, cols);
        ASSERT_EQ(ans_seq, ans);
    }
}

TEST(Min_val_in_cols_matrix_MPI, test7_on_matrix) {
    int rows = 100, cols = 50;
    std::vector <int> a = getRandomMatrix(rows, cols);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector <int> ans(cols, rows * cols + 1);
    ans = ParallelMinInColsMatrix(a, rows, cols);
    std::vector <int> ans_seq(cols, rows * cols + 1);
    if (rank == 0) {
        ans_seq = SequentialMinInColsMatrix(a, rows, cols);
        ASSERT_EQ(ans_seq, ans);
    }
}

TEST(Min_val_in_cols_matrix_MPI, test8_on_matrix) {
    int rows = 500, cols = 500;
    std::vector <int> a = getRandomMatrix(rows, cols);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector <int> ans(cols, rows * cols + 1);
    ans = ParallelMinInColsMatrix(a, rows, cols);
    std::vector <int> ans_seq(cols, rows * cols + 1);
    if (rank == 0) {
        ans_seq = SequentialMinInColsMatrix(a, rows, cols);
        ASSERT_EQ(ans_seq, ans);
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

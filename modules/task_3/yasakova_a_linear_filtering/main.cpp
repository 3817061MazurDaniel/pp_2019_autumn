// Copyright 2019 Yasakova Anastasia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "../../../modules/task_3/yasakova_a_linear_filtering/linear_filtering.h"

TEST(Linear_Filtering_MPI, test1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_ANY_THROW(getImage(-1, 3));
    }
}

TEST(Linear_Filtering_MPI, test2) {
    std::vector <int> a;
    int rank;
    std::vector <int> mask = { 0, 0, 0, 0, 1, 0, 0, 0, 0 };
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        a = getImage(5, 7);
        ASSERT_ANY_THROW(ParallelLinearFilter(mask, a, 3, 2));
    }
}

TEST(Linear_Filtering_MPI, test3) {
    int rows = 3, cols = 3;
    std::vector <int> mask = { 0, 1, 0, 1, -4, 1, 0, 1, 0 };
    std::vector <int> a(rows * cols), ans_seq(rows * cols);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        a[0] = 150;
        a[1] = 128;
        a[2] = 100;
        a[3] = 200;
        a[4] = 175;
        a[5] = 100;
        a[6] = 50;
        a[7] = 250;
        a[8] = 200;
        ans_seq[0] = 28;
        ans_seq[1] = 41;
        ans_seq[2] = 28;
        ans_seq[3] = -225;
        ans_seq[4] = -22;
        ans_seq[5] = 175;
        ans_seq[6] = 350;
        ans_seq[7] = -325;
        ans_seq[8] = -50;
    }
    std::vector <int> ans(rows * cols);
    ans = ParallelLinearFilter(mask, a, rows, cols);
    if (rank == 0) {
        ASSERT_EQ(ans_seq, ans);
    }
}

TEST(Linear_Filtering_MPI, test4) {
    int rows = 10, cols = 10;
    std::vector <int> mask = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
    std::vector <int> a(rows * cols);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        a = getImage(rows, cols);
    }
    std::vector <int> ans(rows * cols);
    ans = ParallelLinearFilter(mask, a, rows, cols);
    std::vector <int> ans_seq(rows * cols);
    if (rank == 0) {
        ans_seq = SequentialLinearFilter(mask, a, rows, cols);
        ASSERT_EQ(ans_seq, ans);
    }
}

TEST(Linear_Filtering_MPI, test5) {
    int rows = 20, cols = 40;
    std::vector <int> mask = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
    std::vector <int> a(rows * cols);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        a = getImage(rows, cols);
    }
    std::vector <int> ans(rows * cols);
    ans = ParallelLinearFilter(mask, a, rows, cols);
    std::vector <int> ans_seq(rows * cols);
    if (rank == 0) {
        ans_seq = SequentialLinearFilter(mask, a, rows, cols);
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

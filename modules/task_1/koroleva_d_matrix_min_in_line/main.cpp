// Copyright 2018 Koroleva Daria
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_min_in_line.h"

TEST(Sequential_Operations_MPI, Test_Random_Matrix_Size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) return;

    const int m = 190;
    const int n = 27;
    const size_t expected_size = m * n;

    std::vector<int> matr = getRandomMatrix(m, n);

    ASSERT_EQ(expected_size, matr.size());
}

TEST(Sequential_Operations_MPI, Test_Throw_When_M_Is_Zero) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) return;

    const int count_size_m = 0;
    const int count_size_n = 5;

    std::vector<int> matr = { 1, 1, 1, 1, 129723,
                              32, 0, 123, 5, 5,
                              7, 7, 7, 7, 7,
                              123, -10, 323, 75, 3 };

    ASSERT_ANY_THROW(getMinsInMatrixLines(matr, count_size_m, count_size_n););
}

TEST(Sequential_Operations_MPI, Test_Get_Correct_Mins_In_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) return;

    const int m = 4;
    const int n = 5;
    std::vector<int> matr = { 1, 1, 1, 1, 129723,
                              32, 0, 123, 5, 5,
                              7, 7, 7, 7, 7,
                              123, -10, 323, 75, 3 };
    std::vector<int> expected_res = { 1, 0, 7, -10 };

    std::vector<int> mins = getMinsInMatrixLines(matr, m, n);

    EXPECT_EQ(expected_res, mins);
}

TEST(Parallel_Operations_MPI, Test_Throw_When_Negaive_N) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int count_size_m = 4;
    const int count_size_n = -5;

    std::vector<int> matr = { 1, 1, 1, 1, 129723,
                              32, 0, 123, 5, 5,
                              7, 7, 7, 7, 7,
                              123, -10, 323, 75, 3 };

    if (rank == 0) {
        ASSERT_ANY_THROW(getMinsInMatrixLines(matr, count_size_m, count_size_n));
    }
}

TEST(Parallel_Operations_MPI, Test_Get_Mins_In_Small_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_m = 9;
    const int count_size_n = 5;

    if (rank == 0) {
        global_vec = getRandomMatrix(count_size_m, count_size_n);
    }

    std::vector<int> result = getParallelMinsInMatrixLines(global_vec, count_size_m, count_size_n);

    if (rank == 0) {
        std::vector<int> reference_vec = getMinsInMatrixLines(global_vec, count_size_m, count_size_n);
        ASSERT_EQ(reference_vec, result);
    }
}

TEST(Parallel_Operations_MPI, Test_Get_Mins_In_Big_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_m = 1034;
    const int count_size_n = 3323;

    if (rank == 0) {
        global_vec = getRandomMatrix(count_size_m, count_size_n);
    }

    std::vector<int> result = getParallelMinsInMatrixLines(global_vec, count_size_m, count_size_n);

    if (rank == 0) {
        std::vector<int> reference_vec = getMinsInMatrixLines(global_vec, count_size_m, count_size_n);
        ASSERT_EQ(reference_vec, result);
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

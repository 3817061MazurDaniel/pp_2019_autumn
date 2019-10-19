// Copyright 2019 Korobeinikov Aleksei
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./max_in_matrix.h"

TEST(Max_On_Columns, Throw_When_Rows_Or_Columns_Equal_Zero) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrix;

    if (rank == 0) {
        ASSERT_ANY_THROW(matrix = GetMatrix(0, 4));
    }
}

TEST(Max_On_Columns, Rows_Equal_Columns) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrix;

    if (rank == 0) {
        matrix = GetMatrix(4, 4);
    }

    std::vector<int> result = GetMaxValueInColumnOfMatrix(matrix, 4, 4);

    if (rank == 0) {
        std::vector <int> check = CheckResult(matrix, 4, 4);
        ASSERT_EQ(result, check);
    }
}
TEST(Max_On_Columns, Rows_And_Columns_Equal_One) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrix;

    if (rank == 0) {
        matrix = GetMatrix(1, 1);
    }

    std::vector<int> result = GetMaxValueInColumnOfMatrix(matrix, 1, 1);

    if (rank == 0) {
        std::vector <int> check = CheckResult(matrix, 1, 1);
        ASSERT_EQ(result, check);
    }
}

TEST(Max_On_Columns, Rows_And_Columns_Are_Diff_And_Rows_Less_Then_Columns) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrix;

    if (rank == 0) {
        matrix = GetMatrix(4, 7);
    }

    std::vector<int> result = GetMaxValueInColumnOfMatrix(matrix, 4, 7);

    if (rank == 0) {
        std::vector <int> check = CheckResult(matrix, 4, 7);
        ASSERT_EQ(result, check);
    }
}

TEST(Max_On_Columns, Rows_And_Columns_Are_Diff_And_Columns_Less_Then_Rows) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrix;

    if (rank == 0) {
        matrix = GetMatrix(9, 5);
    }

    std::vector<int> result = GetMaxValueInColumnOfMatrix(matrix, 9, 5);

    if (rank == 0) {
        std::vector <int> check = CheckResult(matrix, 9, 5);
        ASSERT_EQ(result, check);
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

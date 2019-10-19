// Copyright 2019 Kondrina Tatyana

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>

#include <valarray>
#include <iostream>

#include "../../../modules/task_1/kondrina_t_matrix_rows_sum/matrix_rows_sum.h"

TEST(Matrix_Rows_Sum_MPI, Test_Matrix_Seq) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int columns = 4, rows = 3;

    if (rank == 0) {
        std::valarray<int> testMatrix { 1,  2,   3,  4,
                                        5,  6,   7,  8,
                                        9,  10,  11, 12};

        std::valarray<int> expectedResult { 10, 26, 42 };
        std::valarray<int> result = rowsSumSeq(testMatrix, columns, rows);

        ASSERT_EQ(valarraysEquality(expectedResult, result), true);
    }
}

TEST(Matrix_Rows_Sum_MPI, Test_Matrix_10x9) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int columns  = 10, rows = 9;

    std::valarray<int> testMatrix = randomMatrix(columns, rows);
    std::valarray<int> result(rows), expectedResult(rows);

    result = rowsSum(testMatrix, columns, rows);

    if (rank == 0) {
        expectedResult = rowsSumSeq(testMatrix, columns, rows);

        ASSERT_EQ(valarraysEquality(expectedResult, result), true);
    }
}

TEST(Matrix_Rows_Sum_MPI, Test_Matrix_4x5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int columns  = 4, rows = 5;

    std::valarray<int> testMatrix = randomMatrix(columns, rows);
    std::valarray<int> result(rows), expectedResult(rows);

    result = rowsSum(testMatrix, columns, rows);

    if (rank == 0) {
        expectedResult = rowsSumSeq(testMatrix, columns, rows);

        ASSERT_EQ(valarraysEquality(expectedResult, result), true);
    }
}

TEST(Matrix_Rows_Sum_MPI, Test_Matrix_15x10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int columns  = 15, rows = 10;

    std::valarray<int> testMatrix = randomMatrix(columns, rows);
    std::valarray<int> result(rows), expectedResult(rows);

    result = rowsSum(testMatrix, columns, rows);

    if (rank == 0) {
        expectedResult = rowsSumSeq(testMatrix, columns, rows);

        ASSERT_EQ(valarraysEquality(expectedResult, result), true);
    }
}

TEST(Matrix_Rows_Sum_MPI, Test_Matrix_123x10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int columns  = 123, rows = 10;

    std::valarray<int> testMatrix = randomMatrix(columns, rows);
    std::valarray<int> result(rows), expectedResult(rows);

    result = rowsSum(testMatrix, columns, rows);

    if (rank == 0) {
        expectedResult = rowsSumSeq(testMatrix, columns, rows);

        ASSERT_EQ(valarraysEquality(expectedResult, result), true);
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

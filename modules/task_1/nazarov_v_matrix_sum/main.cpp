// Copyright 2019 Nazarov Vladislav
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_sum.h"

TEST(Parallel_Sum_MPI, Test_Matrix_Sum) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    const int matrix_size_n = 100;
    const int matrix_size_m = 100;

    if (rank == 0) {
        matrix = getRandomMatrix(matrix_size_m, matrix_size_n);
    }

    int global_sum = getParallelSum(matrix, matrix_size_n, matrix_size_m);

    if (rank == 0) {
        int reference_sum = getSequentialSum(matrix);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Sum_MPI, Test_Sum_Of_Big_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    const int matrix_size_n = 1000;
    const int matrix_size_m = 1000;

    if (rank == 0) {
        matrix = getRandomMatrix(matrix_size_m, matrix_size_n);
    }

    int global_sum = getParallelSum(matrix, matrix_size_n, matrix_size_m);

    if (rank == 0) {
        int reference_sum = getSequentialSum(matrix);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Sum_MPI, Test_Sum_Of_Matrix_2x2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    const int matrix_size_n = 2;
    const int matrix_size_m = 2;

    if (rank == 0) {
        matrix = getRandomMatrix(matrix_size_m, matrix_size_n);
    }

    int global_sum = getParallelSum(matrix, matrix_size_n, matrix_size_m);

    if (rank == 0) {
        int reference_sum = getSequentialSum(matrix);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Sum_MPI, Test_Matrix_Size_Not_Positive) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    const int matrix_size_n = -4;
    const int matrix_size_m = 10;

    if (rank == 0) {
        ASSERT_ANY_THROW(getRandomMatrix(matrix_size_m, matrix_size_n));
    }
}

TEST(Parallel_Sum_MPI, Test_Matrix_Size_Too_Big) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    const int matrix_size_n = 10000;
    const int matrix_size_m = 2500;

    if (rank == 0) {
        ASSERT_ANY_THROW(getRandomMatrix(matrix_size_m, matrix_size_n));
    }
}

TEST(Parallel_Sum_MPI, Test_Vector_and_Matrix_Size_Different) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    const int matrix_size_n = 5;
    const int matrix_size_m = 5;

    if (rank == 0) {
        matrix = getRandomMatrix(3, 4);
    }

    ASSERT_ANY_THROW(getParallelSum(matrix, matrix_size_n, matrix_size_m));
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

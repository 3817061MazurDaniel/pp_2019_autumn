// Copyright 2019 Konnov Sergey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include "./matrix_mult_only_hor_a.h"

TEST(Matrix_Mult_Only_Hor_A, Test_Sequential_Matrix_Multiplication) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        std::vector<int>a, b, c;
        for (int i = 0; i < 9; i++) {
            a.push_back(i+1);
            b.push_back(9-i);
        }
        c = matrix_mult_sequential(a, b, 3);
        std::vector<int>d = {30, 24, 18, 84, 69, 54, 138, 114, 90};
        ASSERT_EQ(c, d);
    }
}

TEST(Matrix_Mult_Only_Hor_A, Test_Sequential_Matrix_On_Size_Equal_One) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        std::vector<int>a = {5}, b = {10}, c, d = {50};
        c = matrix_mult_sequential(a, b, 1);
        ASSERT_EQ(c, d);
    }
}

TEST(Matrix_Mult_Only_Hor_A, Test_Parallel_Matrix_On_Small_Size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int>a, b, c, d;
    if (rank == 0) {
        for (int i = 0; i < 9; i++) {
            a.push_back(i+1);
            b.push_back(9-i);
        }
        c = matrix_mult_sequential(a, b, 3);
    }
    d = matrix_mult_parallel(a, b, 3);
    if (rank == 0) {
        ASSERT_EQ(c, d);
    }
}

TEST(Matrix_Mult_Only_Hor_A, Test_Parallel_Matrix_On_Big_Size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int>a, b, c, d;
    int size = 500;
    if (rank == 0) {
        a = generate_matrix(size, 0);
        b = generate_matrix(size, 1);
        c = matrix_mult_sequential(a, b, size);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    d = matrix_mult_parallel(a, b, size);
    if (rank == 0) {
        ASSERT_EQ(c, d);
    }
}

TEST(Matrix_Mult_Only_Hor_A, Test_Parallel_Matrix_On_Size_Equal_One) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int>a = {5}, b = {10}, c, d;
    if (rank == 0) {
        c = matrix_mult_sequential(a, b, 1);
    }
    d = matrix_mult_parallel(a, b, 1);
    if (rank == 0) {
        ASSERT_EQ(c, d);
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

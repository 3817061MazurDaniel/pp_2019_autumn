// Copyright 2019 Makarova Viktoria

#include <mpi.h>
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <vector>
#include "../../../modules/task_2/makarova_v_matrix_vector_mult/matrix_vector_mult.h"

TEST(Test_Matrix_Vector_Mult, Seq_Mult) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::vector<int> matrix = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int rows = 4, cols = 3;
        std::vector<int> vector = {2, 2, 2};
        std::vector<int> result = {12, 30, 48, 66};
        std::vector<int> a = multiply_seq(vector, matrix, cols, rows);
        ASSERT_EQ(a, result);
    }
}

TEST(Test_Matrix_Vector_Mult, Matrix_12) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix = {1, 12, 122,
                               4536, 3, 5,
                               8, 888, 5674,
                               754, 7253, 685745};
    int rows = 4, cols = 3;
    std::vector<int> vector = {3, 18, 8};
    std::vector<int> result = multiply_seq(vector, matrix, cols, rows);
    std::vector<int> a = multiply(vector, matrix, cols, rows);
    if (rank == 0) {
        ASSERT_EQ(a, result);
    }
}

TEST(Test_Matrix_Vector_Mult, Parl_Mult) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int rows = 4, cols = 3;
    std::vector<int> vector = {2, 2, 2};
    std::vector<int> result = {12, 30, 48, 66};
    std::vector<int> a = multiply(vector, matrix, cols, rows);
    if (rank == 0) {
        ASSERT_EQ(a, result);
    }
}

TEST(Test_Matrix_Vector_Mult, Random_Matrix_20) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix = getRandomMatrix(4, 5);
    int rows = 4, cols = 5;
    std::vector<int> vector = getRandomVector(5);
    std::vector<int> result = multiply_seq(vector, matrix, cols, rows);
    std::vector<int> a = multiply(vector, matrix, cols, rows);
    if (rank == 0) {
        ASSERT_EQ(a, result);
    }
}

TEST(Test_Matrix_Vector_Mult, Random_Matrix_40) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix = getRandomMatrix(4, 10);
    int rows = 4, cols = 10;
    std::vector<int> vector = getRandomVector(10);
    std::vector<int> result = multiply_seq(vector, matrix, cols, rows);
    std::vector<int> a = multiply(vector, matrix, cols, rows);
    if (rank == 0) {
        ASSERT_EQ(a, result);
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

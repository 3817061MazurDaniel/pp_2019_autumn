// Copyright 2019 Sadikov Artem
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./simple_method.h"

TEST(Simple_Method_Slae, Test_On_Matrix_Size_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> Matrix(12);
    if (rank == 0) {
        Matrix = { 4, 1, 1, 9, 1, 6, -1, 10, 1, 2, 5, 20 };
    }

    std::vector<double> actual(3);

    actual = get_res(Matrix, 3, 0.22);

    if (rank == 0) {
        std::vector<double> expected(3);
        expected = { 1.03958, 2.00833, 3.05 };
        EXPECT_TRUE(is_equal(actual, expected));
    }
}


TEST(Simple_Method_Slae, Cant_Get_Res_With_Wrong_Size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> matrix(4);

    if (rank == 0) {
        matrix = get_rand_matrix(4);
    }
    if (rank == 0) {
        std::vector<double> act(4);
        ASSERT_ANY_THROW(act = get_res(matrix, 2, 0.001));
    }
}

TEST(Simple_Method_Slae, Test_On_Matrix_Size_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> matrix(20);
    if (rank == 0) {
        matrix = { 10, -1, 2, 0, 6,
                 -1, 11, -1, 3, 25,
                 2, -1, 10, -1, -11,
                 0, 3, -1, 8, 15 };
    }

    std::vector<double> actual(4);

    actual = get_res(matrix, 4, 0.99);

    std::vector<double> expected(4);
    if (rank == 0) {
        expected = { 1.04727,
                     1.7159,
                     -0.80522,
                     0.88522 };

        EXPECT_TRUE(is_equal(actual, expected));
    }
}

TEST(Simple_Method_Slae, Not_Create_Matrix_If_Size_Less_Then_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> matrix(1);
    if (rank == 0) {
        ASSERT_ANY_THROW(matrix = get_rand_matrix(1));
    }
}

TEST(Simple_Method_Slae, Can_Create_Matrix_On_5_Size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> matrix(5);
    if (rank == 0) {
        ASSERT_NO_THROW(matrix = get_rand_matrix(5));
    }
}

TEST(Simple_Method_Slae, Can_Create_Matrix_On_10_Size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> matrix(10);
    if (rank == 0) {
        ASSERT_NO_THROW(matrix = get_rand_matrix(10));
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

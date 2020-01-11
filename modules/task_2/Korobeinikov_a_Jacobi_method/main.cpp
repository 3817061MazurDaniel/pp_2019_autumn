// Copyright 2019 Korobeinikov Aleksei
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include "./Jacoby_method.h"

TEST(Jacobi_method, First_Sample_With_Size_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector <double> matrix(9);
    std::vector <double> b(3);
    double eps = 0.001;
    if (rank == 0) {
        matrix = { 10, 1, -1, 1, 10, -1, -1, 1, 10 };
        b = { 11, 10, 10 };
    }

    std::vector<double> result = GetSolveOfTask(matrix, b, 3, eps);

    if (rank == 0) {
        double error = getNormForAns(matrix, result, b);
        ASSERT_NEAR(error, 0, 0.005);
    }
}

TEST(Jacobi_method, Second_Sample_With_Size_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector <double> matrix(9);
    std::vector <double> b(3);
    double eps = 0.001;
    if (rank == 0) {
        matrix = { 8, 1, -4, 2, -6, 1, -1, 1, 4 };
        b = { 6, -9, 5 };
    }

    std::vector<double> result = GetSolveOfTask(matrix, b, 3, eps);

    if (rank == 0) {
        double error = getNormForAns(matrix, result, b);
        ASSERT_NEAR(error, 0, 0.005);
    }
}

TEST(Jacobi_method, Third_Sample_With_Size_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector <double> matrix(9);
    std::vector <double> b(3);
    double eps = 0.001;
    if (rank == 0) {
        matrix = { 10, -3, 2, 3, -10, -2, 2, -3, 10 };
        b = { 10, -23, 26 };
    }

    std::vector<double> result = GetSolveOfTask(matrix, b, 3, eps);

    if (rank == 0) {
        double error = getNormForAns(matrix, result, b);
        ASSERT_NEAR(error, 0, 0.005);
    }
}

TEST(Jacobi_method, First_Sample_With_Size_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector <double> matrix(16);
    std::vector <double> b(4);
    double eps = 0.001;
    if (rank == 0) {
        matrix = { 20.9, 1.2, 2.1, 0.9, 1.2, 21.2, 1.5, 2.5, 2.1,
         1.5, 19.8, 1.2, 0.9, 2.5, 1.3, 32.1};
        b = { 21.7, 27.46, 28.76, 49.72 };
    }

    std::vector<double> result = GetSolveOfTask(matrix, b, 4, eps);

    if (rank == 0) {
        double error = getNormForAns(matrix, result, b);
        ASSERT_NEAR(error, 0, 0.05);
    }
}

TEST(Jacobi_method, Second_Sample_With_Size_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector <double> matrix(16);
    std::vector <double> b(4);
    double eps = 0.001;
    if (rank == 0) {
        matrix = { 5, 2, 1, 1, 2, 6, 2, 1, 1,
         2, 7, 1, 1, 1, 2, 8 };
        b = { 29, 31, 26, 19 };
    }

    std::vector<double> result = GetSolveOfTask(matrix, b, 4, eps);

    if (rank == 0) {
        double error = getNormForAns(matrix, result, b);
        ASSERT_NEAR(error, 0, 0.05);
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

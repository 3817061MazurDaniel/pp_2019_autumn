// Copyright 2019 Savosina
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "./seidel_method.h"

TEST(Parallel_Siedel_Method_MPI, Test_Create_Vector_Positive) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 3;
    std::vector<double> a(n);
    if (rank == 0) {
        ASSERT_NO_THROW(a = getRandomVector(n));
    }
}

TEST(Parallel_Siedel_Method_MPI, Test_Cannot_Create_Vector_Negative) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = -3;
    std::vector<double> a(3);
    if (rank == 0) {
        ASSERT_ANY_THROW(a = getRandomVector(n));
    }
}

TEST(Parallel_Siedel_Method_MPI, Test_Sequential) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 3;
    std::vector<double> a(n * n);
    std::vector<double> b(n);
    std::vector<double> x(n);
    std::vector<double> t(n);
    if (rank == 0) {
        a[0] = 3; a[1] = 2; a[2] = 1;
        a[3] = 2; a[4] = 1; a[5] = 2;
        a[6] = 1; a[7] = 1; a[8] = 1;
        b[0] = 2; b[1] = 1; b[2] = 3;
        for (int i = 0; i < n; i++)
            x[i] = 0;
        t[0] = 0.666667;
        t[1] = - 0.333333;
        t[2] = 2.66667;

        std::vector<double> d(n);
        d = getSequentialOperations(a, b, n);
        ASSERT_NEAR(Norma(t), Norma(d), 1e-5);
    }
}

TEST(Parallel_Siedel_Method_MPI, Test_Parallel) {
    int n = 3;
    std::vector<double> a(n * n);
    std::vector<double> b(n);
    std::vector<double> x(n);
    std::vector<double> t(n);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        a[0] = 3; a[1] = 2; a[2] = 1;
        a[3] = 2; a[4] = 1; a[5] = 2;
        a[6] = 1; a[7] = 1; a[8] = 1;
        b[0] = 2; b[1] = 1; b[2] = 3;

        t[0] = 0.666667;
        t[1] = - 0.333333;
        t[2] = 2.66667;
    }
    x = getParallelOperations(a, b, n);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_NEAR(Norma(x), Norma(t), 1e-5);
    }
}

TEST(Parallel_Siedel_Method_MPI, Test_Parallel_And_Sequential) {
    int n = 3;
    std::vector<double> a(n * n);
    std::vector<double> b(n);
    std::vector<double> x(n);
    std::vector<double> tmp(n);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        a[0] = 3; a[1] = 2; a[2] = 1;
        a[3] = 2; a[4] = 1; a[5] = 2;
        a[6] = 1; a[7] = 1; a[8] = 1;
        b[0] = 2; b[1] = 1; b[2] = 3;

        tmp = getSequentialOperations(a, b, n);
        // std::cout << "Vector new tmp:" << std::endl;
        // PrintVector(tmp, n);
    }

    x = getParallelOperations(a, b, n);
    // std::cout << "Vector new x:" << std::endl;
    MPI_Barrier(MPI_COMM_WORLD);
    // PrintVector(x, n);

    if (rank == 0) {
        ASSERT_NEAR(Norma(x), Norma(tmp), 1e-5);
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

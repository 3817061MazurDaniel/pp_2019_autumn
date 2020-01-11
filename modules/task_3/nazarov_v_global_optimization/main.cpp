// Copyright 2019 Nazarov Vladislav
#define _USE_MATH_DEFINES

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./global_optimization.h"

TEST(Global_Optimization_MPI, Test_First_Function) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double(*fptr)(double, double) = f1;
    resultTwoVar res = solveTwoVar(-5, 5, -5, 5, fptr);
    if (rank == 0) {
        resultTwoVar resEq = solveTwoVarSequential(-5, 5, -5, 5, fptr);
        ASSERT_TRUE(compareResults(res, resEq, 0.1));
    }
}

TEST(Global_Optimization_MPI, Test_Second_Function) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double(*fptr)(double, double) = f2;
    resultTwoVar res = solveTwoVar(-0.9, 1, -1, 1, fptr);
    if (rank == 0) {
        resultTwoVar expectedRes = {0, 0, 4};
        ASSERT_TRUE(compareResults(res, expectedRes, 0.1));
    }
}

TEST(Global_Optimization_MPI, Test_Third_Function) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double(*fptr)(double, double) = f3;
    resultTwoVar res = solveTwoVar(0, 2, 0.3, 1, fptr);
    if (rank == 0) {
        resultTwoVar expectedRes = {1, 0.5, 4};
        ASSERT_TRUE(compareResults(res, expectedRes, 0.1));
    }
}

TEST(Global_Optimization_MPI, Test_Fourth_Function) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double(*fptr)(double, double) = f4;
    resultTwoVar res = solveTwoVar(0.1, 5, -5, 5, fptr);
    if (rank == 0) {
        resultTwoVar expectedRes = {5, -5, -136.18};
        ASSERT_TRUE(compareResults(res, expectedRes, 0.1));
    }
}

TEST(Global_Optimization_MPI, Test_Fifth_Function) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double(*fptr)(double, double) = f5;
    resultTwoVar res = solveTwoVar(1, 3, 1, 2, fptr);
    if (rank == 0) {
        resultTwoVar expectedRes = {2, 1.25, -1.5};
        ASSERT_TRUE(compareResults(res, expectedRes, 0.1));
    }
}

TEST(Global_Optimization_MPI, Test_Sixth_Function) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double(*fptr)(double, double) = f6;
    resultTwoVar res = solveTwoVar(1, 5, 0.5, 4, fptr);
    if (rank == 0) {
        resultTwoVar expectedRes = {3*M_PI_2, M_PI, -3};
        ASSERT_TRUE(compareResults(res, expectedRes, 0.1));
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

// Copyright 2019 Bykovskaya Alina
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "./hor_gauss.h"


TEST(Hor_Gauss_MPI, Test_3x4_Not_Parallel) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> a;
    const int rows = 3;
    const int cols = 4;

    if (rank == 0) {
        a = {
            1, 2, 3, 8,
            2, 6, 8, 12,
            3, 8, 12, 16
        };
    }

    if (rank == 0) {
        std::vector <double> ans = Gauss(a, rows, cols);
        std::vector <double> x = {16.0, 2.0, -4.0};
        for (int i = 0; i < rows; ++i)
            ASSERT_NEAR(x[i], ans[i], 0.000001);
    }
}

TEST(Hor_Gauss_MPI, Test_3x4_Parallel) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> a;
    const int rows = 3;
    const int cols = 4;

    if (rank == 0) {
        a = {
            1, 2, 3, 8,
            2, 6, 8, 12,
            3, 8, 12, 16
        };
    }
    std::vector <double> ans = parallelGauss(a, rows, cols);

    if (rank == 0) {
        std::vector <double> x = {16.0, 2.0, -4.0};
        for (int i = 0; i < rows; ++i)
            ASSERT_NEAR(x[i], ans[i], 0.000001);
    }
}


TEST(Hor_Gauss_MPI, Test_2x3_Parallel_vc_Not_Parallel) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> a;
    const int rows = 2;
    const int cols = 3;

    if (rank == 0) {
        a = {2, 3, 12, 3, -1, 7};
    }
    std::vector <double> ans = parallelGauss(a, rows, cols);

    if (rank == 0) {
        std::vector <double> x = Gauss(a, rows, cols);
        for (int i = 0; i < rows; ++i)
            ASSERT_NEAR(x[i], ans[i], 0.000001);
    }
}

TEST(Hor_Gauss_MPI, Test_3x4_Parallel_vc_Not_Parallel) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> a;
    const int rows = 3;
    const int cols = 4;

    if (rank == 0) {
        a = {
            1, 2, 3, 8,
            2, 6, 8, 12,
            3, 8, 12, 16
        };
    }
    std::vector <double> ans = parallelGauss(a, rows, cols);

    if (rank == 0) {
        std::vector <double> x = Gauss(a, rows, cols);
        for (int i = 0; i < rows; ++i)
            ASSERT_NEAR(x[i], ans[i], 0.000001);
    }
}

TEST(Hor_Gauss_MPI, Test_4x5_Parallel_vc_Not_Parallel) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> a;
    const int rows = 4;
    const int cols = 5;

    if (rank == 0) {
        a = {
            2, 5, 4, 1, 20,
            1, 3, 2, 1, 11,
            2, 10, 9, 7, 40,
            3, 8, 9, 2, 37
        };
    }
    std::vector <double> ans = parallelGauss(a, rows, cols);

    if (rank == 0) {
        std::vector <double> x = Gauss(a, rows, cols);
        for (int i = 0; i < rows; ++i)
            ASSERT_NEAR(x[i], ans[i], 0.000001);
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

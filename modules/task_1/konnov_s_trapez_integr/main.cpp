// Copyright 2019 Konnov Sergey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include <algorithm>
#include <cmath>
#include <functional>
#include "./trapez_integr.h"

TEST(Trapez_Integr, Test_Sequential_Integral) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        std::function<double(double)>f = [](double x) {return x * x; };
        double i1 = GetTrapezIntegrSequential(2, 5, 3, f);
        double i2 = 39.5;
        EXPECT_NEAR(i1, i2, 1e-2);
    }
}

TEST(Trapez_Integr, Test_Sequetional_Integral_On_Sin_Function) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        std::function<double(double)>f = [](double x) {return sin(x); };
        double i1 = GetTrapezIntegrSequential(-10, 10, 40, f);
        double i2 = 0;
        EXPECT_NEAR(i1, i2, 1e-2);
    }
}

TEST(Trapez_Integr, Test_Parallel_Integral) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(double)>f = [](double x) {return x * x; };
    double i1 = GetTrapezIntegrParallel(2, 5, 3, f);
    if (rank == 0) {
        double i2 = GetTrapezIntegrSequential(2, 5, 3, f);
        EXPECT_NEAR(i1, i2, 1e-2);
    }
}

TEST(Trapez_Integr, Test_Parallel_Integral_On_Sin_Function) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(double)>f = [](double x) {return sin(x); };
    double i1 = GetTrapezIntegrParallel(-10, 10, 20, f);
    if (rank == 0) {
        double i2 = GetTrapezIntegrSequential(-10, 10, 20, f);
        EXPECT_NEAR(i1, i2, 1e-2);
    }
}

TEST(Trapez_Integr, Test_Parallel_Integral_On_Big_Length) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(double)>f = [](double x) {return x * x + sin(x); };
    double i1 = GetTrapezIntegrParallel(-10000, 10000, 20000, f);
    if (rank == 0) {
        double i2 = GetTrapezIntegrSequential(-10000, 10000, 20000, f);
        EXPECT_NEAR(i1, i2, 1e-2);
    }
}

TEST(Trapez_Integr, Test_Parallel_integral_On_Small_Length) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(double)>f = [](double x) {return x * x + sin(x); };
    double i1 = GetTrapezIntegrParallel(-1, 1, 20000, f);
    if (rank == 0) {
        double i2 = GetTrapezIntegrSequential(-1, 1, 20000, f);
        EXPECT_NEAR(i1, i2, 1e-2);
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

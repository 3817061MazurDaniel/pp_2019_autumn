// Copyright 2019 Arisova Anasrasiia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./rectangle_integration.h"

double F1(double x) {
    return x*x;
}

double F2(double x) {
    return 2.0*x;
}

double F3(double x) {
    return 1.0/(x * x);
}

TEST(SequentialIntegration, Test_F1) {
    int N = 10000;
    std::vector<double> vec(N + 1);
    double a = 0.0, b = 2.0;
    double h = (b - a)/(static_cast<double> (N));
    for (int i = 0; i < N; i++) {
            vec[i] = a + i * h;
    }
    vec[N] = b;
    double sum = 0.0;
    sum = getSequentialIntegration(vec, F1, h);

      ASSERT_NEAR(2.667, sum, 1e-2);
}

TEST(SequentialIntegration, Test_F2) {
    int N = 10000;
    std::vector<double> vec(N + 1);
    double a = 0.0, b = 2.0;
    double h = (b - a)/(static_cast<double> (N));
    for (int i = 0; i < N; i++) {
            vec[i] = a + i * h;
    }
    vec[N] = b;
    double sum = 0.0;
    sum = getSequentialIntegration(vec, F2, h);

      ASSERT_NEAR(4.0, sum, 1e-2);
}

TEST(Parallel_Integration_MPI, Test_MPI_F1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N = 10000;
    std::vector<double> global_vector(N + 1);
    double a = 0.0, b = 3.0;
    double h = (b - a)/(static_cast<double> (N));

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            global_vector[i] = a + i * h;
        }
        global_vector[N] = b;
    }
    double sum = 9.0;
    double g_sum = getParallelIntegration(global_vector, F1, a, b, N);

    if (rank == 0) {
        ASSERT_NEAR(sum, g_sum, 1e-2);
    }
}

TEST(Parallel_Integration_MPI, Test_MPI_F1_with_negative_values) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N = 10000;
    std::vector<double> global_vector(N + 1);
    double a = -2.0, b = -1.0;
    double h = (b - a)/(static_cast<double> (N));

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            global_vector[i] = a + i * h;
        }
        global_vector[N] = b;
    }
    double sum = 7.0/3.0;
    double g_sum = getParallelIntegration(global_vector, F1, a, b, N);

    if (rank == 0) {
        ASSERT_NEAR(sum, g_sum, 1e-2);
    }
}

TEST(Parallel_Integration_MPI, Test_MPI_F2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N = 10000;
    std::vector<double> global_vector(N + 1);
    double a = 0.0, b = 2.0;
    double h = (b - a)/(static_cast<double> (N));

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            global_vector[i] = a + i * h;
        }
        global_vector[N] = b;
    }
    double sum = 4.0;
    double g_sum = getParallelIntegration(global_vector, F2, a, b, N);

    if (rank == 0) {
        ASSERT_NEAR(sum, g_sum, 1e-2);
    }
}

TEST(Parallel_Integration_MPI, Test_MPI_F3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N = 10000;
    std::vector<double> global_vector(N + 1);
    double a = 1.0, b = 3.0;
    double h = (b - a)/(static_cast<double> (N));

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            global_vector[i] = a + i * h;
        }
        global_vector[N] = b;
    }
    double sum = 2.0/3.0;
    double g_sum = getParallelIntegration(global_vector, F3, a, b, N);

    if (rank == 0) {
        ASSERT_NEAR(sum, g_sum, 1e-2);
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

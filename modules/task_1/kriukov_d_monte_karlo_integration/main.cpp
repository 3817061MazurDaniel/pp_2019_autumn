// // Copyright 2019 Kriukov Dmitry
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <math.h>
#include <vector>
#include "./monte_carlo_integration.h"

#define POINT_COUNT 10000
#define ABS_ERROR 0.5

double linear_function(double x) {
    return x;
}

double quadratic_function(double x) {
    return x * x;
}

TEST(Monte_Carlo_Integration_MPI, Linear_function_integration) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const double upper_limit = 5.0;
    const double lower_limit = 0.0;

    double integration_result = monteCarloIntegration(lower_limit, upper_limit, linear_function, POINT_COUNT);

    if (rank == 0) {
        ASSERT_NEAR((upper_limit - lower_limit)*(upper_limit - lower_limit) / 2, integration_result, ABS_ERROR);
    }
}

TEST(Monte_Carlo_Integration_MPI, Linear_function_integration_with_negative_limit) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const double upper_limit = 0.0;
    const double lower_limit = 5.0;

    double integration_result = monteCarloIntegration(lower_limit, upper_limit, linear_function, POINT_COUNT);

    if (rank == 0) {
        ASSERT_NEAR(-(upper_limit - lower_limit)*(upper_limit - lower_limit) / 2, integration_result, ABS_ERROR);
    }
}

TEST(Monte_Carlo_Integration_MPI, Quadratic_function_integration) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const double upper_limit = 3.0;
    const double lower_limit = 0.0;

    double integration_result = monteCarloIntegration(lower_limit, upper_limit, quadratic_function, POINT_COUNT);

    if (rank == 0) {
        ASSERT_NEAR((upper_limit - lower_limit)*(upper_limit - lower_limit)*(upper_limit - lower_limit) / 3,
                    integration_result, ABS_ERROR);
    }
}

TEST(Monte_Carlo_Integration_MPI, Trigonometric_function_integration) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const double pi = 3.14159265358979323846;
    const double upper_limit = pi / 2;
    const double lower_limit = 0.0;

    double integration_result = monteCarloIntegration(lower_limit, upper_limit, sin, POINT_COUNT);

    if (rank == 0) {
        ASSERT_NEAR(1, integration_result, ABS_ERROR);
    }
}

TEST(Monte_Carlo_Integration_MPI, Throws_then_negative_point_count) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const double pi = 3.14159265358979323846;
    const double upper_limit = pi / 2;
    const double lower_limit = 0.0;

    if (rank == 0) {
        ASSERT_ANY_THROW(monteCarloIntegration(lower_limit, upper_limit, sin, -POINT_COUNT));
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

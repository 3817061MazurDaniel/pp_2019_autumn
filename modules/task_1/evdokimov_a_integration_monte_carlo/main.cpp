// Copyright 2019 Evdokimov Artem
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./integration_monte_carlo.h"

#define COUNT_OF_DOTS 1000.0
#define FOULT 1.5

double linear_function(double value) {
    return value;
}

double custom_monotonically_increasing_function(double value) {
    return value / 2.0;
}

double exponential_function(double value) {
    return value * value;
}

TEST(Integration_Monte_Carlo, Exponential_Function_Integration) {
    // Arrange
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double upper_limit = 2.0;
    double lower_limit = 0.0;
    // Act
    double integration_result = getResultOfIntegral(
        upper_limit,
        lower_limit,
        COUNT_OF_DOTS,
        exponential_function);

    if (rank == 0) {
        ASSERT_NEAR((upper_limit - lower_limit)* (upper_limit - lower_limit) *(upper_limit - lower_limit) / 3,
            integration_result, FOULT);
    }
}

TEST(Integration_Monte_Carlo, Monotonically_Increasing_Function_Integration) {
    // Arrange
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double upper_limit = 3.0;
    double lower_limit = 0.0;
    // Act
    double integration_result = getResultOfIntegral(
        upper_limit,
        lower_limit,
        COUNT_OF_DOTS,
        custom_monotonically_increasing_function);

    if (rank == 0) {
        ASSERT_NEAR((upper_limit - lower_limit) *
            (custom_monotonically_increasing_function(upper_limit) -
            custom_monotonically_increasing_function(lower_limit)) / 2,
            integration_result, FOULT);
    }
}

TEST(Integration_Monte_Carlo, Linear_Function_Integration) {
    // Arrange
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double upper_limit = 4.0;
    double lower_limit = 0.0;
    // Act
    double integration_result = getResultOfIntegral(
        upper_limit,
        lower_limit,
        COUNT_OF_DOTS,
        linear_function);
    if (rank == 0) {
        ASSERT_NEAR((upper_limit - lower_limit)*(upper_limit - lower_limit) / 2, integration_result, FOULT);
    }
}

TEST(Integration_Monte_Carlo, Count_Of_Dots_Cant_Be_Negative) {
    ASSERT_ANY_THROW(getResultOfIntegral(
        5.0,
        0.0,
        -5,
        linear_function));
}

TEST(Integration_Monte_Carlo, Lower_Limit_Cant_Be_More_Then_Upper) {
    ASSERT_ANY_THROW(getResultOfIntegral(
        2.0,
        6.0,
        COUNT_OF_DOTS,
        linear_function));
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

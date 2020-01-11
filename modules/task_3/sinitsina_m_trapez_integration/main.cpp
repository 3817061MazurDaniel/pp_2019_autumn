// Copyright 2019 Sinitsina Maria
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <math.h>
#include <vector>
#include "../../../modules/task_3/sinitsina_m_trapez_integration/sinitsina_m_trapez_integration.h"

double f1(std::vector<double> vec) {
    double x = vec[0];
    double y = vec[1];
    return (x * x + y * y);
}

double f2(std::vector<double> vec) {
    double x = vec[0];
    double y = vec[1];
    double z = vec[2];
    return (x + y + z);
}

double f3(std::vector<double> vec) {
    double x = vec[0];
    double y = vec[1];
    double z = vec[2];
    return (log10(2 * x*x) + sqrt(y) + 5 * z);
}

double f4(std::vector<double> vec) {
    double x = vec[0];
    double y = vec[1];
    double z = vec[2];
    double t = vec[3];
    return (x + 5 * y - 2 * z + t);
}

TEST(Multiple_Trapez_Integraion, Test_1_Integral_with_2_dimension_f1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int dimension = 2;

    std::vector<double> x(dimension);
    std::vector<double> y(dimension);
    x = { 2, 10 };
    y = { -5, 40 };
    const int n = 100;

    double result = ParallelIntegr(f1, x, y, n);

    if (rank == 0) {
        double error = 0.0001;
        ASSERT_NEAR(result, SequentialIntegr(f1, x, y, n), error);
    }
}

TEST(Multiple_Trapez_Integraion, Test_2_Integral_with_3_dimension_f2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int dimension = 3;

    std::vector<double> x(dimension);
    std::vector<double> y(dimension);
    x = { 2, 10, 3 };
    y = { -5, 40, 8 };
    const int n = 100;

    double result = ParallelIntegr(f2, x, y, n);

    if (rank == 0) {
        double error = 0.0001;
        ASSERT_NEAR(result, SequentialIntegr(f2, x, y, n), error);
    }
}

TEST(Multiple_Trapez_Integraion, Test_3_Integral_with_3_dimension_f3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int dimension = 3;

    std::vector<double> x(dimension);
    std::vector<double> y(dimension);
    x = { 2, 10, 15 };
    y = { -5, 40, 20 };
    const int n = 100;

    double result = ParallelIntegr(f3, x, y, n);

    if (rank == 0) {
        double error = 0.0001;
        ASSERT_NEAR(result, SequentialIntegr(f3, x, y, n), error);
    }
}

TEST(Multiple_Trapez_Integraion, Test_4_Wrong_Data_Negative_in_n) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int dimension = 2;

    std::vector<double> x(dimension);
    std::vector<double> y(dimension);
    x = { 2, 1 };
    y = { -5, 6 };
    const int n = -100;

    if (rank == 0) {
        ASSERT_ANY_THROW(ParallelIntegr(f1, x, y, n));
    }
}

TEST(Multiple_Trapez_Integraion, Test_5_Wrong_Data_Negative_in_size_of_first_coord) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int dimension = 2;

    std::vector<double> x(dimension);
    std::vector<double> y(dimension);
    x = { 2 };
    y = { -5, 6 };
    const int n = 100;

    if (rank == 0) {
        ASSERT_ANY_THROW(ParallelIntegr(f1, x, y, n));
    }
}

TEST(Multiple_Trapez_Integraion, Test_6_Wrong_Data_Negative_in_size_of_second_coord) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int dimension = 2;

    std::vector<double> x(dimension);
    std::vector<double> y(dimension);
    x = { 2, 10 };
    y = { -5, 6, 13 };
    const int n = 100;

    if (rank == 0) {
        ASSERT_ANY_THROW(ParallelIntegr(f1, x, y, n));
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

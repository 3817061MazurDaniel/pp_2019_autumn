  // Copyright 2019 Arisova Anastasiia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cmath>
#include "./Strongin_global_search.h"

#define PI 3.14159265

double Q1(double x) {
    return ((x + 1) * (x + 1) - 2);
}
double Q2(double x) {
    return (std::sin(x));
}
double Q3(double x) {
    return (- (x + 1) * (x + 1) - 3);
}

TEST(Task_Strongin_global_search, Test_1) {
    int size;
    double ans;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    ans = goParallelAlgorithm(-3.0, 0.0, Q1);
    EXPECT_NEAR(ans, -2.0, 1.2e-4);
}

TEST(Task_Strongin_global_search, Test_2) {
    int size;
    double ans;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    ans = goParallelAlgorithm(0.0, 3.0, Q1);
    EXPECT_NEAR(ans, -1.0, 1.2e-4);
}

TEST(Task_Strongin_global_search, Test_3) {
    int size;
    double ans;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    ans = goParallelAlgorithm(PI / 4, 3 * PI / 4, Q2);
    EXPECT_NEAR(ans, 1 / (std::sqrt(2)), 1.2e-4);
}

TEST(Task_Strongin_global_search, Test_4) {
    int size;
    double ans;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    ans = goParallelAlgorithm(-2.0, 1.0, Q3);
    EXPECT_NEAR(ans, -7.0, 1.2e-4);
}

TEST(Task_Strongin_global_search, Test_5) {
    int size;
    double ans;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    ans = goParallelAlgorithm(0.0, PI, Q2);
    EXPECT_NEAR(ans, 0.0, 1.2e-4);
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

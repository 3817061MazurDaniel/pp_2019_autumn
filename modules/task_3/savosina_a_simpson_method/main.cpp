// Copyright 2019 Savosina
#define _USE_MATH_DEFINES

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cmath>
#include <utility>
#include <vector>
#include "./simpson_method.h"

TEST(Simpson_Method_MPI, Test_First_Function) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(const std::vector<double>&)> func = [](const std::vector<double>& vec){
        return sin(vec[0]+vec[1]);};
    std::vector<std::pair<double, double>> scope = { {0, M_PI_2}, {0, M_PI_4}};  // expected 1.00028
    scopeCheck(scope);
    double res = calculateIntegral(scope, 100, func, 100);
    if (rank == 0) {
        ASSERT_LE(std::abs(res - 1.0), 0.01);
    }
}

TEST(Simpson_Method_MPI, Test_Second_Function) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(const std::vector<double>&)> func = [](const std::vector<double>& vec){
        return sin(vec[0])*vec[1]*cos(vec[2]);};
    std::vector<std::pair<double, double>> scope = { {0, 1}, {-2, 2}, {3, 7} };  // expected ~ 0
    scopeCheck(scope);
    double res = calculateIntegral(scope, 100, func, 100);
    if (rank == 0) {
        ASSERT_LE(std::abs(res - 0.0), 0.01);
    }
}

TEST(Simpson_Method_MPI, Test_Third_Function) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(const std::vector<double>&)> func = [](const std::vector<double>& vec){
        return std::sqrt(1 + 2*std::pow(vec[0], 2) - std::pow(vec[0], 3));};
    std::vector<std::pair<double, double>> scope = { {1.2, 2} };  // expected 1.09
    scopeCheck(scope);
    double res = calculateIntegral(scope, 100, func, 100);
    if (rank == 0) {
        ASSERT_LE(std::abs(res - 1.09), 0.01);
    }
}

TEST(Simpson_Method_MPI, Test_Fourth_Function) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(const std::vector<double>&)> func = [](const std::vector<double>& vec){
    return vec[0] / (std::pow(vec[0], 4) + 4);};
    std::vector<std::pair<double, double>> scope = { {0, 5} };  // expected 0.377
    scopeCheck(scope);
    double res = calculateIntegral(scope, 100, func, 100);
    if (rank == 0) {
        ASSERT_LE(std::abs(res - 0.377), 0.01);
    }
}

TEST(Simpson_Method_MPI, Test_Fifth_Function) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(const std::vector<double>&)> func = [](const std::vector<double>& vec){
        return std::pow(vec[0], 2) + vec[1] + vec[2] + std::pow(vec[3], 2) + std::pow(vec[4], 3);};
    std::vector<std::pair<double, double>> scope = { {0, 1}, {1, 2}, {-1, 1}, {-2, 2}, {0, 1} };  // expected ~ 27
    scopeCheck(scope);
    double res = calculateIntegral(scope, 5, func, 5);
    if (rank == 0) {
        ASSERT_LE(std::abs(res - 27), 1);
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

// Copyright 2019 Konnov Sergey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include <functional>
#include <cmath>
#include "./monte_carlo_multiple_integration.h"

#define abs_error 0.75  // ?
#define count_of_dots 1000000
const double PI = acos(-1);


TEST(monteCarloMultipleIntegraion, One_Dimensional_Function_On_Sin_Sequentional) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        double lower_limit = 0., upper_limit = PI/2;
        std::function<double(const std::vector<double>&)> f = [](const std::vector<double>& v) {return sin(v[0]);};
        double ans = monteCarloMultipleIntegraionSequentional({lower_limit}, {upper_limit}, count_of_dots, f, 1);
        double check_ans = 1;
        ASSERT_NEAR(ans, check_ans, abs_error);
    }
}

TEST(monteCarloMultipleIntegraion, One_Dimensional_Function_On_Big_Interval_Sequentional) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        std::function<double(const std::vector<double>&)>f =
                    [](const std::vector<double>& v) {return v[0]*v[0];};
        double lower_limit = 0., upper_limit = 5.;
        double ans = monteCarloMultipleIntegraionSequentional({lower_limit}, {upper_limit}, count_of_dots, f, 2);
        double ans_check = 41.666666667;
        ASSERT_NEAR(ans, ans_check, abs_error);
    }
}

TEST(monteCarloMultipleIntegraion, Two_Dimensional_Function_On_Big_Interval_Sequentional) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        std::function<double(const std::vector<double>&)>f =
                        [](const std::vector<double>& v) {return v[0]*v[0] + v[1]*v[1];};
        std::vector<double>lower_limits = {0., 0.}, upper_limits = {3., 3.};
        double ans = monteCarloMultipleIntegraionSequentional(lower_limits, upper_limits, count_of_dots, f, 3);
        double ans_check = 54.;
        ASSERT_NEAR(ans, ans_check, abs_error);
    }
}

TEST(monteCarloMultipleIntegraion, One_Dimensional_Function_On_Big_Interval) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(const std::vector<double>&)>f =
                    [](const std::vector<double>& v) {return v[0]*v[0];};
    double lower_limit = 0., upper_limit = 5.;
    double ans = monteCarloMultipleIntegraion({lower_limit}, {upper_limit}, count_of_dots, f, 4);
    if (rank == 0) {
        std::function<double(double)> f = [](double x) {return x*x*x/3;};
        double ans_check = f(upper_limit) - f(lower_limit);
        ASSERT_NEAR(ans, ans_check, abs_error);
    }
}

TEST(monteCarloMultipleIntegraion, One_Dimensional_Function_On_Small_Interval) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(const std::vector<double>&)>f =
                    [](const std::vector<double>& v) {return v[0]*v[0];};
    double lower_limit = 0., upper_limit = 1.;
    double ans = monteCarloMultipleIntegraion({lower_limit}, {upper_limit}, count_of_dots, f, 5);
    if (rank == 0) {
        std::function<double(double)> f = [](double x) {return x*x*x/3;};
        double ans_check = f(upper_limit) - f(lower_limit);
        ASSERT_NEAR(ans, ans_check, abs_error);
    }
}

TEST(monteCarloMultipleIntegration, Two_Dimensional_Function_On_Small_Interval) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(const std::vector<double>&)>f =
                    [](const std::vector<double>& v) {return v[0]*v[0] + v[1]*v[1];};
    std::vector<double>lower_limits = {0., 0.}, upper_limits = {3., 3.};
    double ans = monteCarloMultipleIntegraion(lower_limits, upper_limits, count_of_dots, f, 6);
    if (rank == 0) {
        double ans_check = monteCarloMultipleIntegraionSequentional(lower_limits, upper_limits, count_of_dots, f, 7);
        ASSERT_NEAR(ans, ans_check, abs_error);
    }
}

TEST(monteCarloMultipleIntegration, Many_Dimensional_Function_On_Small_Interval) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::function<double(const std::vector<double>&)>f =
                    [](const std::vector<double>& v)
                        {return std::abs(v[0] + v[1] + v[2] + v[3] + v[4] + v[5]);};
    std::vector<double> lower_limits(6), upper_limits(6);
    for (int i = 0; i < 6; i++) {
        lower_limits[i] = -1., upper_limits[i] = 1.;
    }
    double ans = monteCarloMultipleIntegraion(lower_limits, upper_limits, count_of_dots, f, 8);
    if (rank == 0) {
        double ans_check = monteCarloMultipleIntegraionSequentional(lower_limits, upper_limits, count_of_dots, f, 9);
        ASSERT_NEAR(ans, ans_check, abs_error);
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

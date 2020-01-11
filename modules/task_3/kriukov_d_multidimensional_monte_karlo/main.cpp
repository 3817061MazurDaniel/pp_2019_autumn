// // Copyright 2019 Kriukov Dmitry
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <math.h>
#include <vector>
#include "./multidimensional_monte_karlo.h"

#define POINT_COUNT 10000

bool area_circle_radius_2(std::vector<double> args) {
    return (args[0]* args[0] + args[1] * args[1]) < 4;
}

bool area_sphere_radius_2(std::vector<double> args) {
    return (args[0] * args[0] + args[1] * args[1] + args[2] * args[2]) < 4;
}

bool area_three_sphere_radius_2(std::vector<double> args) {
    return (args[0] * args[0] + args[1] * args[1] + args[2] * args[2] + args[3]*args[3]) < 4;
}

bool area_quart_sphere_radius_2(std::vector<double> args) {
    return (args[0] > 0 && args[2] > 0 && (args[0] * args[0] + args[1] * args[1] + args[2] * args[2]) < 4);
}


double func_const(std::vector<double> args) {
    return 1;
}

double func_quadratic(std::vector<double> args) {
    return args[0] * args[0];
}

double func_linear(std::vector<double> args) {
    return args[0] + args[1];
}

bool area_2_parabols(std::vector<double> args) {
    return (args[1] > (args[0] * args[0] - 1)) && (args[1] < -args[0] * args[0] + 1);
}

TEST(Multidimensional_Monte_Karlo_MPI, const_func_integration_on_area_circle) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const double abs_errror = 1.0;

    std::vector<double> s_point = { -2, -2 };

    double integration_result = multidimensionalIntegration(s_point, 4, func_const,
                                                            area_circle_radius_2, 2, POINT_COUNT);
    if (rank == 0) {
        ASSERT_NEAR(3.14 * 4, integration_result, abs_errror);
    }
}

TEST(Multidimensional_Monte_Karlo_MPI, linear_func_integration_on_area_2_parabols) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const double abs_errror = 1.0;

    std::vector<double> s_point = { -1, -1 };

    double integration_result = multidimensionalIntegration(s_point, 2, func_linear, area_2_parabols, 2, POINT_COUNT);

    if (rank == 0) {
        ASSERT_NEAR(0, integration_result, abs_errror);
    }
}

TEST(Multidimensional_Monte_Karlo_MPI, const_func_integration_on_area_sphere) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const double abs_errror = 5.0;

    std::vector<double> s_point = { -2, -2, -2};

    double integration_result = multidimensionalIntegration(s_point, 4, func_const,
        area_sphere_radius_2, 3, POINT_COUNT);
    if (rank == 0) {
        ASSERT_NEAR(3.14 * 4 * 8 / 3, integration_result, abs_errror);
    }
}

TEST(Multidimensional_Monte_Karlo_MPI, quadratic_func_integration_on_area_sphere) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const double abs_errror = 5.0;

    std::vector<double> s_point = { -2, -2, -2 };

    double integration_result = multidimensionalIntegration(s_point, 4, func_quadratic,
        area_quart_sphere_radius_2, 3, POINT_COUNT);
    if (rank == 0) {
        ASSERT_NEAR(3.14 * 32 / 15, integration_result, abs_errror);
    }
}

TEST(Multidimensional_Monte_Karlo_MPI, const_func_integration_on_area_three_sphere) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const double abs_errror = 10.0;

    std::vector<double> s_point = { -2, -2 , -2, -2};

    double integration_result = multidimensionalIntegration(s_point, 4, func_const,
        area_three_sphere_radius_2, 4, POINT_COUNT);
    if (rank == 0) {
        ASSERT_NEAR(3.14 * 3.14 * 16 / 2, integration_result, abs_errror);
    }
}

#ifdef MULTIDIMENSIONAL_MONTE_KARLO_TIME_TEST

TEST(Multidimensional_Monte_Karlo_MPI, Time_Test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const double abs_errror = 0.5;
    const int point_count = 1000000;

    std::vector<double> s_point = { -2, -2 , -2, -2 };
    double a1, b1, a2, b2;
    double resSequaintal;

    if (rank == 0) {
        a2 = MPI_Wtime();
        resSequaintal = multidimensionalIntegrationSequential(s_point, 4, func_const,
            area_three_sphere_radius_2, 4, point_count);
        b2 = MPI_Wtime();
    }

    if (rank == 0)
        a1 = MPI_Wtime();
    double integration_result = multidimensionalIntegration(s_point, 4, func_const,
        area_three_sphere_radius_2, 4, point_count);
    if (rank == 0)
        b1 = MPI_Wtime();

    if (rank == 0) {
        std::cout << "Sequential " << b2 - a2 << std::endl;
        std::cout << "Parralel " << b1 - a1;
    }
    if (rank == 0) {
        ASSERT_NEAR(3.14 * 3.14 * 16 / 2, integration_result, abs_errror);
    }
}

#endif

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

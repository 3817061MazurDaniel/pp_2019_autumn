// Copyright 2019 Trenina Elizaveta

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./rect_intg.h"


double linear(double x) { return x; }
double quadratic(double x) { return x * x; }

TEST(Rect_Intg, Test_L_Sequential_Linear) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        double res = get_l_rect_intg_sequential(0.0, 5.0, 1000, linear);
        double exp = 12.5;
        EXPECT_NEAR(res, exp, 0.01251);
    }
}

TEST(Rect_Intg, Test_L_Sequential_Quadratic) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        double res = get_l_rect_intg_sequential(0.0, 10.0, 1000, quadratic);
        double exp = 333.33333;
        EXPECT_NEAR(res, exp, 1.0);
    }
}

TEST(Rect_Intg, Test_L_Parallel_Linear) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double res = get_l_rect_intg_parallel(0.0, 5.0, 1000, linear);
    if (rank == 0) {
        double exp = get_l_rect_intg_sequential(0.0, 5.0, 1000, linear);
        EXPECT_NEAR(res, exp, 0.025);
    }
}

TEST(Rect_Intg, Test_L_Parallel_Quadratic) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double res = get_l_rect_intg_parallel(0.0, 10.0, 1000, quadratic);
    if (rank == 0) {
        double exp = get_l_rect_intg_sequential(0.0, 10.0, 1000, quadratic);
        EXPECT_NEAR(res, exp, 1.0);
    }
}

TEST(Rect_Intg, Test_Midpoint_Sequential_Linear) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        double res = get_midpoint_intg_sequential(0.0, 5.0, 1000, linear);
        double exp = 12.5;
        EXPECT_NEAR(res, exp, 5.2e-6);  // 0.000005);
    }
}

TEST(Rect_Intg, Test_Midpoint_Sequential_Quadratic) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        double res = get_midpoint_intg_sequential(0.0, 10.0, 1000, quadratic);
        double exp = 333.33333;
        EXPECT_NEAR(res, exp, 8.4e-5);
    }
}

TEST(Rect_Intg, Test_Midpoint_Parallel_Linear) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double res = get_midpoint_intg_parallel(0.0, 5.0, 1000, linear);
    if (rank == 0) {
        double exp = get_midpoint_intg_sequential(0.0, 5.0, 1000, linear);
        EXPECT_NEAR(res, exp, 0.4e-5);
    }
}

TEST(Rect_Intg, Test_Midpoint_Parallel_Quadratic) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double res = get_midpoint_intg_parallel(0.0, 10.0, 1000, quadratic);
    if (rank == 0) {
        double exp = get_midpoint_intg_sequential(0.0, 10.0, 1000, quadratic);
        EXPECT_NEAR(res, exp, 8.33e-5);
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

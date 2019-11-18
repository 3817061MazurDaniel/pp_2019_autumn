// // Copyright 2019 Kriukov Dmitry
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <math.h>
#include <vector>
#include "./fixed_point_iteration.h"

#define FIXED_POINT_ITERATION_ERR 0.0001


TEST(Fixed_Point_Iteration_MPI, Test_Sequential) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        std::vector<double> matrix = {6.25, -1, 0.5 , -1, 5, 2.12, 0.5, 2.12, 3.6};
        std::vector<double> fterm = { 7.5, -8.68, -0.24 };
        std::vector<double> res;
        res = fixedPointIterationSequential(matrix, fterm, 3, FIXED_POINT_ITERATION_ERR);
        double err = getAbsError(matrix, fterm, res, 3);
        ASSERT_NEAR(err, 0, 1.0);
    }
}

TEST(Fixed_Point_Iteration_MPI, Test_Parralel_size_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> matrix;
    std::vector<double> fterm;
    std::vector<double> res;

    if (rank == 0) {
        matrix = { 6.25, -1, 0.5 , -1, 5, 2.12, 0.5, 2.12, 3.6 };
        fterm = { 7.5, -8.68, -0.24 };
    }
    res = fixedPointIterationParralel(matrix, fterm, 3, FIXED_POINT_ITERATION_ERR);
    if (rank == 0) {
        double err = getAbsError(matrix, fterm, res, 3);
        ASSERT_NEAR(err, 0, 1.0);
    }
}

TEST(Fixed_Point_Iteration_MPI, Test_Parralel_size_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> matrix;
    std::vector<double> fterm;
    std::vector<double> res;

    if (rank == 0) {
        matrix = { 0.69,  0.09, 0, -0.03, 0.17, 0.66, -0.22,
                   -0.2, 0.00012, 0.00003, 0.00402, 0.00011, 0.09,  0.06, 0.3, -0.76 };
        fterm = { 4.06 , -0.73, -0.03751, 4.01 };
    }
    res = fixedPointIterationParralel(matrix, fterm, 4, FIXED_POINT_ITERATION_ERR);
    if (rank == 0) {
        double err = getAbsError(matrix, fterm, res, 4);
        ASSERT_NEAR(err, 0, 1.0);
    }
}

TEST(Fixed_Point_Iteration_MPI, Test_Sequential_random) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> matrix;
    std::vector<double> fterm;
    std::vector<double> res;
    const int varcount = 10;

    if (rank == 0) {
        matrix = getNormalMatrix(varcount);
        fterm = getRandomVector(varcount);
        res = fixedPointIterationSequential(matrix, fterm, varcount, FIXED_POINT_ITERATION_ERR);
        double err = getAbsError(matrix, fterm, res, varcount);
        ASSERT_NEAR(err, 0, 1.0);
    }
}

TEST(Fixed_Point_Iteration_MPI, Test_Parralel_random) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> matrix;
    std::vector<double> fterm;
    std::vector<double> res;
    const int varcount = 10;

    if (rank == 0) {
        matrix = getNormalMatrix(varcount);
        fterm = getRandomVector(varcount);
    }
    res = fixedPointIterationParralel(matrix, fterm, varcount, FIXED_POINT_ITERATION_ERR);
    if (rank == 0) {
        double err = getAbsError(matrix, fterm, res, varcount);
        ASSERT_NEAR(err, 0, 1.0);
    }
}

#ifdef FIXED_POINT_ITERATION_TIME_TEST

TEST(Fixed_Point_Iteration_MPI, Time_Test_Parallel_random) {
    int rank;
    std::vector<double> resParallel;
    std::vector<double> matrix;
    std::vector<double> vector;
    double a, b;
    const int vsize = 1000;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matrix = getNormalMatrix(vsize);
        vector = getRandomVector(vsize);
    }
    a = MPI_Wtime();
    resParallel = fixedPointIterationParralel(matrix, vector, vsize, FIXED_POINT_ITERATION_ERR);
    b = MPI_Wtime();
    if (rank == 0)
        std::cout << b - a;
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

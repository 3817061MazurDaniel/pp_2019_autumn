// Copyright 2019 Savkin Yuriy
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include <cstdlib>
#include <vector>
#include "./conjugate_gradient_method.h"

TEST(Conjugate_Gradient_Method, simple_matrix) {
    int size = 3, rank;
    double** a = nullptr;
    double* b = nullptr;
    std::vector<double> x;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        a = new double*[size];
        a[0] = new double[size * size];
        for (int i = 1; i < size; ++i)
            a[i] = a[i - 1] + size;
        b = new double[size];

        a[0][0] = 1;
        a[0][1] = 2;
        a[0][2] = 3;
        a[1][0] = 2;
        a[1][1] = 6;
        a[1][2] = 8;
        a[2][0] = 3;
        a[2][1] = 8;
        a[2][2] = 12;

        b[0] = 8;
        b[1] = 12;
        b[2] = 16;
    }

    x = conjugateGradientMethod(a, b, size);

    if (rank == 0) {
        std::vector<double> w = conjugateGradientMethodOneProc(a, b, size);

        delete[] a[0];
        delete[] a;
        delete[] b;

        for (size_t i = 0; i < x.size(); ++i)
            ASSERT_NEAR(x[i], w[i], 0.000001);
    }
}

TEST(Conjugate_Gradient_Method, big_numbers_test) {
    int size = 10, rank;
    double** a = nullptr;
    double* b = nullptr;
    std::vector<double> x;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        a = getRandomMatrix(size, 0, 50, 23);
        b = getRandomVector(size, 0, 50, 24);
    }

    x = conjugateGradientMethod(a, b, size);

    if (rank == 0) {
        std::vector<double> w = conjugateGradientMethodOneProc(a, b, size);

        delete[] a[0];
        delete[] a;
        delete[] b;

        for (size_t i = 0; i < x.size(); ++i)
            ASSERT_NEAR(x[i], w[i], 0.001);
    }
}

TEST(Conjugate_Gradient_Method, big_size_test) {
    int size = 100, rank;
    double** a = nullptr;
    double* b = nullptr;
    std::vector<double> x;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        a = getRandomMatrix(size, 0, 10, 51);
        b = getRandomVector(size, 0, 10, 66);
    }

    x = conjugateGradientMethod(a, b, size);

    if (rank == 0) {
        std::vector<double> w = conjugateGradientMethodOneProc(a, b, size);

        delete[] a[0];
        delete[] a;
        delete[] b;

        for (size_t i = 0; i < x.size(); ++i)
            ASSERT_NEAR(x[i], w[i], 0.05);
    }
}

TEST(Conjugate_Gradient_Method, big_size_and_numbers_test) {
    int size = 100, rank;
    double** a = nullptr;
    double* b = nullptr;
    std::vector<double> x;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        a = getRandomMatrix(size, 0, 50, 92);
        b = getRandomVector(size, 0, 50, 55);
    }

    x = conjugateGradientMethod(a, b, size);

    if (rank == 0) {
        std::vector<double> w = conjugateGradientMethodOneProc(a, b, size);

        delete[] a[0];
        delete[] a;
        delete[] b;

        for (size_t i = 0; i < x.size(); ++i)
            ASSERT_NEAR(x[i], w[i], 0.05);
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

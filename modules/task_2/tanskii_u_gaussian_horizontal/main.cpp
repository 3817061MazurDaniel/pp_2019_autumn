// Copyright 2019 Tanskii Yuriy
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./gaussian_horizontal.h"

TEST(Parallel_Operations_MPI, test_paralell_3_x4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> global_vec;
    std::vector<double> reference_vec;
    std::vector<double> result_vec;

    int m = 3;
    int n = 4;
    if (rank == 0)
        global_vec = {
      2, 10, -3, 38,
        -3, -24, 5, -86,
        1, 3, -5, 27
    };
    reference_vec.resize(m);
    result_vec.resize(m);
    result_vec = gaussianParallel(global_vec, m, n);
    reference_vec = { -2.0, 3.0, -4.0 };
    if (rank == 0) {
        for (int i = 0; i < m; i++) {
            EXPECT_NEAR(reference_vec[i], result_vec[i], 0.0001);
        }
    }
}
TEST(Parallel_Operations_MPI, test_equality_sequential_parallel) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> global_vec;
    std::vector<double> sequential_vec;
    std::vector<double> parallel_vec;

    int m = 3;
    int n = 4;
    if (rank == 0)
        global_vec = {
      2, 10, -3, 38,
        -3, -24, 5, -86,
        1, 3, -5, 27
    };
    parallel_vec = gaussianParallel(global_vec, m, n);
    if (rank == 0) {
        sequential_vec = gaussianSequential(global_vec, m, n);
        for (int i = 0; i < m; i++) {
            EXPECT_NEAR(parallel_vec[i], sequential_vec[i], 0.0001);
        }
    }
}

TEST(Parallel_Operations_MPI, Test_size_below_zero) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> global_vec;

    int m = -3;
    int n = 4;
    if (rank == 0)
        global_vec = {
      2, 10, -3, 38,
        -3, -24, 5, -86,
        1, 3, -5, 27
    };
    if (rank == 0) {
        for (int i = 0; i < m; i++) {
            ASSERT_ANY_THROW(global_vec = gaussianParallel(global_vec, m, n));
        }
    }
}
TEST(Parallel_Operations_MPI, test_exception_infinity) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> global_vec;
    int m = 2;
    int n = 4;
    if (rank == 0)
        global_vec = {
      2, 10, -3, 38,
        -3, -24, 5, -86
    };
    if (rank == 0) {
        for (int i = 0; i < m; i++) {
            ASSERT_ANY_THROW(global_vec = gaussianParallel(global_vec, m, n));
        }
    }
}
TEST(Parallel_Operations_MPI, test_exception_no_solution) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> global_vec;
    int m = 3;
    int n = 3;
    if (rank == 0)
        global_vec = {
      2, 10, -3,
        -3, -24, 5,
        1, 2, 3
    };
    if (rank == 0) {
        for (int i = 0; i < m; i++) {
            ASSERT_ANY_THROW(global_vec = gaussianParallel(global_vec, m, n));
        }
    }
}
TEST(Parallel_Operations_MPI, Test_correct_Size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    int m = 4;
    int n = 5;
    size_t matr_size = m * n;
    if (rank == 0) {
        global_vec = getRandomMatrix(m, n);
    }
    if (rank == 0) {
        ASSERT_EQ(matr_size, global_vec.size());
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

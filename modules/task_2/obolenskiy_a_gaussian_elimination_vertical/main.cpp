// Copyright 2019 Obolenskiy Arseniy
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "./gaussian_elimination.h"

static const double EPS = 1e-9;

static bool checkEqual(const std::vector <double> &a, const std::vector <double> &b) {
    if (a.size() != b.size()) {
        return 0;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        if (std::abs(a[i] - b[i]) > EPS) {
            return false;
        }
    }
    return true;
}

static bool checkSolution(const std::vector <double> &a, size_t rows, size_t cols,
                          const std::vector <double> &x) {
    if (rows * cols != a.size()) {
        throw std::runtime_error("Matrix sizes does not match");
    }
    if (rows + 1 != cols) {
        throw std::runtime_error("Incorrect amount of rows and cols");
    }

    std::vector <double> result(rows, 0);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < rows; ++j) {
            result[i] += a[i * cols + j] * x[j];
        }
    }
    for (size_t i = 0; i < rows; ++i) {
        if (result[i] - a[i * cols + rows] > EPS) {
            return false;
        }
    }
    return true;
}

TEST(Gaussian_Elimination_MPI, Test_2x3_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> a;
    const int rows = 2;
    const int cols = 3;

    if (rank == 0) {
        a = {
            1, -1, -5,
            2, 1, -7
        };
    }

    std::vector <double> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <double> seqAnswer = solveSequential(a, rows, cols);
        ASSERT_TRUE(checkEqual(seqAnswer, answer));
        ASSERT_TRUE(checkSolution(a, rows, cols, answer));
    }
}

TEST(Gaussian_Elimination_MPI, Test_2x3_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> a;
    const int rows = 2;
    const int cols = 3;

    if (rank == 0) {
        a = {
            2, 3, 12,
            3, -1, 7
        };
    }

    std::vector <double> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <double> seqAnswer = solveSequential(a, rows, cols);
        ASSERT_TRUE(checkEqual(seqAnswer, answer));
        ASSERT_TRUE(checkSolution(a, rows, cols, answer));
    }
}

TEST(Gaussian_Elimination_MPI, Test_2x3_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> a;
    const int rows = 2;
    const int cols = 3;

    if (rank == 0) {
        a = {
            3, -2, -6,
            5, 1, 3
        };
    }

    std::vector <double> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <double> seqAnswer = solveSequential(a, rows, cols);
        ASSERT_TRUE(checkEqual(seqAnswer, answer));
        ASSERT_TRUE(checkSolution(a, rows, cols, answer));
    }
}

TEST(Gaussian_Elimination_MPI, Test_3x4_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> a;
    const int rows = 3;
    const int cols = 4;

    if (rank == 0) {
        a = {
            2, 3, -1, 9,
            1, -2, 1, 3,
            1, 0, 2, 2
        };
    }

    std::vector <double> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <double> seqAnswer = solveSequential(a, rows, cols);
        ASSERT_TRUE(checkEqual(seqAnswer, answer));
        ASSERT_TRUE(checkSolution(a, rows, cols, answer));
    }
}

TEST(Gaussian_Elimination_MPI, Handling_Of_Incorrect_Matrix_Size_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> a;
    const int rows = 3;
    const int cols = 4;

    if (rank == 0) {
        a = {
            2, 3, -1, 9,
            1, -2, 1, 3,
            1, 0, 2, 2
        };
    }

    EXPECT_ANY_THROW(solveParallel(a, rows, cols + 1));
}

TEST(Gaussian_Elimination_MPI, Handling_Of_Incorrect_Matrix_Size_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> a;
    const int rows = 3;
    const int cols = 4;

    if (rank == 0) {
        a = {
            2, 3, -1,
            1, -2, 1,
            1, 0, 2
        };
    }

    EXPECT_ANY_THROW(solveParallel(a, rows, cols));
}

TEST(Gaussian_Elimination_MPI, Handling_Of_Incorrect_Matrix_Size_1_Seq) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> a;
    const int rows = 3;
    const int cols = 4;

    if (rank == 0) {
        a = {
            2, 3, -1, 9,
            1, -2, 1, 3,
            1, 0, 2, 2
        };
    }

    if (rank == 0) {
        EXPECT_ANY_THROW(solveSequential(a, rows, cols + 1));
    }
}

TEST(Gaussian_Elimination_MPI, Handling_Of_Incorrect_Matrix_Size_2_Seq) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> a;
    const int rows = 3;
    const int cols = 4;

    if (rank == 0) {
        a = {
            2, 3, -1,
            1, -2, 1,
            1, 0, 2
        };
    }

    if (rank == 0) {
        EXPECT_ANY_THROW(solveSequential(a, rows, cols));
    }
}

TEST(Gaussian_Elimination_MPI, Test_2x3_Homogeneous_System) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> a;
    const int rows = 2;
    const int cols = 3;

    if (rank == 0) {
        a = {
            3, -2, 0,
            5, 1, 0
        };
    }

    std::vector <double> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <double> seqAnswer = solveSequential(a, rows, cols);
        ASSERT_TRUE(checkEqual(seqAnswer, answer));
        ASSERT_TRUE(checkSolution(a, rows, cols, answer));
    }
}

TEST(Gaussian_Elimination_MPI, Test_5x6_Homogeneous_System) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> a;
    const int rows = 5;
    const int cols = 6;

    if (rank == 0) {
        a = {
            3, -2, 1, 5, 6, 0,
            5, 1, 3, 4, 5, 0,
            1, 2, 3, 9, 8, 0,
            7, 5, -5, 9, 1, 0,
            1, -8, -2, 9, 1, 0
        };
    }

    std::vector <double> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <double> seqAnswer = solveSequential(a, rows, cols);
        ASSERT_TRUE(checkEqual(seqAnswer, answer));
        ASSERT_TRUE(checkSolution(a, rows, cols, answer));
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

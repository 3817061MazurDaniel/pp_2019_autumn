// Copyright 2019 Sadikov Artem
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./vector_sum.h"

TEST(Vector_Sum, Test_Wrong_Length) {
    std::vector<int> vec;
    ASSERT_ANY_THROW(vec = getRandomVector(-20));
}

TEST(Vector_Sum, Test_With_Even_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(20);
    }

    int actual = getSumMPI(vec, 20);

    if (rank == 0) {
        int expected = getSum(vec);
        EXPECT_EQ(expected, actual);
    }
}

TEST(Vector_Sum, Test_With_Odd_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(21);
    }

    int actual = getSumMPI(vec, 21);

    if (rank == 0) {
        int expected = getSum(vec);
        EXPECT_EQ(actual, expected);
    }
}

TEST(Vector_Sum, Test_With_Eq_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;

    if (rank == 0) {
        vec = {5, 5, 5, 5, 5, 5, 5, 5};
    }

    int actual = getSumMPI(vec, 8);

    if (rank == 0) {
        int expected = getSum(vec);
        EXPECT_EQ(expected, actual);
    }
}

TEST(Vector_Sum, Test_With_20000_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(20000);
    }

    int expected = getSumMPI(vec, 20000);

    if (rank == 0) {
        int actual = getSum(vec);
        EXPECT_EQ(expected, actual);
    }
}

TEST(Vector_Sum, Test_With_200000_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(200000);
    }

    int expected = getSumMPI(vec, 200000);

    if (rank == 0) {
        int actual = getSum(vec);
        EXPECT_EQ(expected, actual);
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


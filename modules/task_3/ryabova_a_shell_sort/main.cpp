// Copyright 2019 Ryabova Alyona
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include "./shell_sort.h"

TEST(shell_sort, sort_random_array) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> array = { 15, -17, 18, 8, -14, -10, 16, 12, -6, -9 };
    std::vector<int> result = { -17, -14, -10, -9, -6, 8, 12, 15, 16, 18 };

    std::vector<int> sortArray = shell_sort(array);

    if (rank == 0) {
        ASSERT_EQ(sortArray, result);
    }
}

TEST(shell_sort, sort_ascending_array) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> array = { 9, 8, 7, 6, 5, 4 };
    std::vector<int> result = { 4, 5, 6, 7, 8, 9 };

    std::vector<int> sortArray = shell_sort(array);

    if (rank == 0) {
        ASSERT_EQ(sortArray, result);
    }
}

TEST(shell_sort, sort_decreasing_array) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> array = { 1, 2, 3, 4, 5, 6, 7 };
    std::vector<int> result = { 1, 2, 3, 4, 5, 6, 7 };

    std::vector<int> sortArray = shell_sort(array);

    if (rank == 0) {
        ASSERT_EQ(sortArray, result);
    }
}

TEST(shell_sort, sort_array_with_one_element) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> array = { 7 };
    std::vector<int> result = { 7 };

    std::vector<int> sortArray = shell_sort(array);

    if (rank == 0) {
        ASSERT_EQ(sortArray, result);
    }
}

TEST(shell_sort, return_empty_array) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> array = { };
    std::vector<int> result = { };

    std::vector<int> sortArray = shell_sort(array);

    if (rank == 0) {
        ASSERT_EQ(sortArray, result);
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

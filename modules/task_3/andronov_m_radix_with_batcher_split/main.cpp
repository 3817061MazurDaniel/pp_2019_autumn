// Copyright 2019 Andronov Maxim
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "../../../modules/task_3/andronov_m_radix_with_batcher_split/radix_with_batcher_split.h"

TEST(Radix_With_Batcher_Split, throw_radix_sort_with_0_elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::vector<int> vec;
        ASSERT_ANY_THROW(RadixSort(vec));
    }
}

TEST(Radix_With_Batcher_Split, radix_sort_with_even_number_elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::vector<int> vec, exp_vec;
        vec = GetRandomVector(30);
        exp_vec = vec;
        std::sort(exp_vec.begin(), exp_vec.end());
        vec = RadixSort(vec);

        EXPECT_EQ(exp_vec, vec);
    }
}

TEST(Radix_With_Batcher_Split, radix_sort_with_odd_number_elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::vector<int> vec, exp_vec;
        vec = GetRandomVector(35);
        exp_vec = vec;
        std::sort(exp_vec.begin(), exp_vec.end());
        vec = RadixSort(vec);

        EXPECT_EQ(exp_vec, vec);
    }
}

TEST(Radix_With_Batcher_Split, sort_with_even_number_elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> array;
    const int size = 30;

    if (rank == 0) {
        array = GetRandomVector(size);
    }

    std::vector<int> result_array = ParallelRadixSortBatcherSplit(array, size);

    if (rank == 0) {
        sort(array.begin(), array.end());
        EXPECT_EQ(array, result_array);
    }
}

TEST(Radix_With_Batcher_Split, sort_with_odd_number_elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> array;
    const int size = 35;

    if (rank == 0) {
        array = GetRandomVector(size);
    }

    std::vector<int> result_array = ParallelRadixSortBatcherSplit(array, size);

    if (rank == 0) {
        sort(array.begin(), array.end());
        EXPECT_EQ(array, result_array);
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

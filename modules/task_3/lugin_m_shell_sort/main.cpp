/*
 * Copyright (C) 2019 LOOGIN. All Rights Reserved.
 * Available types: char(Int8), int(Int32), float(Float32) and double(Float64).
 */

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "shellSort.h"
#include <string>

TEST(shell_sort, return_empty_array_when_empty_array) {
    ASSERT_NO_THROW(IShellSort(std::vector<char>(),
    [&](char left, char right){ return left < right; }));
    ASSERT_NO_THROW(IShellSort(std::vector<int>(),
    [&](int left, int right){ return left < right; }));
    ASSERT_NO_THROW(IShellSort(std::vector<float>(),
    [&](float left, float right){ return left < right; }));
    ASSERT_NO_THROW(IShellSort(std::vector<double>(),
    [&](double left, double right){ return left < right; }));
}

TEST(shell_sort, sort_bottom_up_random_int_array) {
    srand(time(NULL));
    int procRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    for (int percentSkipped = 10; percentSkipped <= 45; percentSkipped += 20) {
        for (int percentRepeated = 20; percentRepeated <= 55; percentRepeated += 20) {
            for (int size = 10; size <= 100; size *= 10) {
                std::vector<int> intVect = getSortedRandomArrayBottomUp(1, size, percentSkipped, percentRepeated);
                std::vector<int> notSortedIntVect = RandomizeArray(intVect, 10);
                std::vector<int> sortedIntVect = IShellSort(notSortedIntVect,
                [&](int left, int right){ return left < right; });
                if (procRank == 0) {
                    ASSERT_EQ(intVect, sortedIntVect);
                }
            }
        }
    }
}

TEST(shell_sort, sort_bottom_up_random_float_array) {
    srand(time(NULL));
    int procRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    for (int percentSkipped = 10; percentSkipped <= 45; percentSkipped += 5) {
        for (int percentRepeated = 20; percentRepeated <= 55; percentRepeated += 5) {
            for (int size = 10; size <= 100; size *= 10) {
                std::vector<float> floatVect = getSortedRandomArrayBottomUp(static_cast<float>(1.),
                size, percentSkipped, percentRepeated);
                std::vector<float> notSortedFloatVect = RandomizeArray(floatVect, 10);
                std::vector<float> sortedFloatVect = IShellSort(notSortedFloatVect,
                [&](float left, float right){ return left < right; });
                if (procRank == 0) {
                    ASSERT_EQ(floatVect, sortedFloatVect);
                }
            }
        }
    }
}

TEST(shell_sort, sort_bottom_up_random_double_array) {
    srand(time(NULL));
    int procRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    for (int percentSkipped = 15; percentSkipped <= 45; percentSkipped += 15) {
        for (int percentRepeated = 45; percentRepeated <= 55; percentRepeated += 15) {
            for (int size = 10; size <= 100; size *= 10) {
                std::vector<double> doubleVect = getSortedRandomArrayBottomUp(1.,
                size, percentSkipped, percentRepeated);
                std::vector<double> notSortedDoubleVect = RandomizeArray(doubleVect, 10);
                std::vector<double> sortedDoubleVect = IShellSort(notSortedDoubleVect,
                [&](double left, double right){ return left < right; });
                if (procRank == 0) {
                    ASSERT_EQ(doubleVect, sortedDoubleVect);
                }
            }
        }
    }
}

TEST(shell_sort, sort_top_down_random_int_array) {
    srand(time(NULL));
    int procRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    for (int percentSkipped = 10; percentSkipped <= 45; percentSkipped += 5) {
        for (int percentRepeated = 20; percentRepeated <= 55; percentRepeated += 5) {
            for (int size = 10; size <= 100; size *= 10) {
                std::vector<int> intVect = getSortedRandomArrayTopDown(1, size, percentSkipped, percentRepeated);
                std::vector<int> notSortedIntVect = RandomizeArray(intVect, 10);
                std::vector<int> sortedIntVect = IShellSort(notSortedIntVect,
                [&](int left, int right){ return left > right; });
                if (procRank == 0) {
                    ASSERT_EQ(intVect, sortedIntVect);
                }
            }
        }
    }
}

TEST(shell_sort, sort_top_down_random_float_array) {
    srand(time(NULL));
    int procRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    for (int percentSkipped = 10; percentSkipped <= 45; percentSkipped += 5) {
        for (int percentRepeated = 20; percentRepeated <= 55; percentRepeated += 5) {
            for (int size = 10; size <= 100; size *= 10) {
                std::vector<float> floatVect = getSortedRandomArrayTopDown(static_cast<float>(1.),
                size, percentSkipped, percentRepeated);
                std::vector<float> notSortedFloatVect = RandomizeArray(floatVect, 10);
                std::vector<float> sortedFloatVect = IShellSort(notSortedFloatVect,
                [&](float left, float right){ return left > right; });
                if (procRank == 0) {
                    ASSERT_EQ(floatVect, sortedFloatVect);
                }
            }
        }
    }
}

TEST(shell_sort, sort_top_down_random_double_array) {
    srand(time(NULL));
    int procRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    for (int percentSkipped = 10; percentSkipped <= 45; percentSkipped += 5) {
        for (int percentRepeated = 20; percentRepeated <= 55; percentRepeated += 5) {
            for (int size = 10; size <= 100; size *= 10) {
                std::vector<double> doubleVect = getSortedRandomArrayTopDown(1., size, percentSkipped, percentRepeated);
                std::vector<double> notSortedDoubleVect = RandomizeArray(doubleVect, 10);
                std::vector<double> sortedDoubleVect = IShellSort(notSortedDoubleVect,
                [&](double left, double right){ return left > right; });
                if (procRank == 0) {
                    ASSERT_EQ(doubleVect, sortedDoubleVect);
                }
            }
        }
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}

// Copyright 2019 Utkin Konstantin
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include "./odd_even_merge_radix.h"

TEST(Odd_Even_Merge_Radix, Test_Sequential_Radix_Sort_Size_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        std::vector<double> vec1(1);
        vec1[0] = 0.0;
        std::vector<double> vec2 = radixSort(vec1);
        EXPECT_EQ(vec1, vec2);
    }
}

TEST(Odd_Even_Merge_Radix, Test_Sequential_Radix_Sort_Size_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int sizeVec = 1000;
        std::vector<double> vec1 = getRandVec(sizeVec);
        std::vector<double> vec2 = vec1;
        vec1 = radixSort(vec1);
        std::sort(vec2.begin(), vec2.end());
        EXPECT_EQ(vec1, vec2);
    }
}

TEST(Odd_Even_Merge_Radix, Test_Sequential_Radix_Sort_Size_1005) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int sizeVec = 1005;
        std::vector<double> vec1 = getRandVec(sizeVec);
        std::vector<double> vec2 = vec1;
        vec1 = radixSort(vec1);
        std::sort(vec2.begin(), vec2.end());
        EXPECT_EQ(vec1, vec2);
    }
}

TEST(Odd_Even_Sort, Test_Parallel_Odd_Even_Merge_Size_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sizeVec = 4;
    std::vector<double> vec1 = getRandVec(sizeVec);
    std::vector<double> vec2 = vec1;
    vec1 = parOddEvenMerge(vec1);
    if (rank == 0) {
        vec2 = radixSort(vec2);
        EXPECT_EQ(vec1, vec2);
    }
}

TEST(Odd_Even_Sort, Test_Parallel_Odd_Even_Merge_Size_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sizeVec = 5;
    std::vector<double> vec1 = getRandVec(sizeVec);
    std::vector<double> vec2 = vec1;
    vec1 = parOddEvenMerge(vec1);
    if (rank == 0) {
        vec2 = radixSort(vec2);
        EXPECT_EQ(vec1, vec2);
    }
}

TEST(Odd_Even_Sort, Test_Parallel_Odd_Even_Merge_Size_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sizeVec = 100;
    std::vector<double> vec1 = getRandVec(sizeVec);
    std::vector<double> vec2 = vec1;
    vec1 = parOddEvenMerge(vec1);
    if (rank == 0) {
        vec2 = radixSort(vec2);
        EXPECT_EQ(vec1, vec2);
    }
}

TEST(Odd_Even_Sort, Test_Parallel_Odd_Even_Merge_Size_105) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sizeVec = 105;
    std::vector<double> vec1 = getRandVec(sizeVec);
    std::vector<double> vec2 = vec1;
    vec1 = parOddEvenMerge(vec1);
    if (rank == 0) {
        vec2 = radixSort(vec2);
        EXPECT_EQ(vec1, vec2);
    }
}

TEST(Odd_Even_Sort, Test_Parallel_Odd_Even_Merge_Size_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sizeVec = 1000;
    std::vector<double> vec1 = getRandVec(sizeVec);
    std::vector<double> vec2 = vec1;
    vec1 = parOddEvenMerge(vec1);
    if (rank == 0) {
        vec2 = radixSort(vec2);
        EXPECT_EQ(vec1, vec2);
    }
}

TEST(Odd_Even_Sort, Test_Parallel_Odd_Even_Merge_Size_1005) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sizeVec = 1005;
    std::vector<double> vec1 = getRandVec(sizeVec);
    std::vector<double> vec2 = vec1;
    vec1 = parOddEvenMerge(vec1);
    if (rank == 0) {
        vec2 = radixSort(vec2);
        EXPECT_EQ(vec1, vec2);
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

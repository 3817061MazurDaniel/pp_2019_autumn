// Copyright 2019 Utkin Konstantin
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include "./odd_even_sort.h"

TEST(Odd_Even_Sort, Test_Sequential_Sort_Size_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        std::vector<int> vec1(1);
        std::vector<int> vec2 = seqSort(vec1);
        EXPECT_EQ(vec1, vec2);
    }
}

TEST(Odd_Even_Sort, Test_Sequential_Sort_Size_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int sizeVec = 1000;
        std::vector<int> vec1 = getRandVec(sizeVec);
        std::vector<int> vec2 = vec1;
        vec1 = seqSort(vec1);
        std::sort(vec2.begin(), vec2.end());
        EXPECT_EQ(vec1, vec2);
    }
}

TEST(Odd_Even_Sort, Test_Sequential_Sort_Size_1005) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int sizeVec = 1005;
        std::vector<int> vec1 = getRandVec(sizeVec);
        std::vector<int> vec2 = vec1;
        vec1 = seqSort(vec1);
        std::sort(vec2.begin(), vec2.end());
        EXPECT_EQ(vec1, vec2);
    }
}

TEST(Odd_Even_Sort, Test_Parallel_Sort_Size_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sizeVec = 1000;
    std::vector<int> vec1 = getRandVec(sizeVec);
    vec1 = parSort(vec1);
    if (rank == 0) {
        std::vector<int> vec2 = vec1;
        vec2 = seqSort(vec2);
        EXPECT_EQ(vec1, vec2);
    }
}

TEST(Odd_Even_Sort, Test_Parallel_Sort_Size_1005) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sizeVec = 1005;
    std::vector<int> vec1 = getRandVec(sizeVec);
    vec1 = parSort(vec1);
    if (rank == 0) {
        std::vector<int> vec2 = vec1;
        vec2 = seqSort(vec2);
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

// Copyright 2019 Isaev Ilya
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "./odd_even.h"

TEST(Bubble_Sort, CANT_CREATE_VECTOR_WITH_NEGATIVE_SIZE) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        ASSERT_ANY_THROW(getRandomVector(-15));
    }
}

TEST(Bubble_Sort, SERIAL_SORT_ALG_CORRECTNESS) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        std::vector<int> a = getRandomVector(100);
        std::vector<int> b = a;
        std::sort(b.begin(), b.end());
        ASSERT_EQ(sequentialBubbleSort(a), b);
    }
}

TEST(Bubble_Sort, SEQUENTIAL_SORT_AND_PARALLEL_ARE_THE_SAME_SIZE1000) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> a, b;
    if (ProcRank == 0) {
        a = getRandomVector(1000);
    }
    b = parallelOddEvenSort(a, 1000);
    if (ProcRank == 0) {
        std::sort(a.begin(), a.end());
        ASSERT_EQ(b, a);
    }
}

TEST(Bubble_Sort, SEQUENTIAL_SORT_AND_PARALLEL_ARE_THE_SAME_SIZE2001) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> a, b;
    if (ProcRank == 0) {
        a = getRandomVector(20000);
    }
    b = parallelOddEvenSort(a, 20000);
    if (ProcRank == 0) {
        std::sort(a.begin(), a.end());
        ASSERT_EQ(b, a);
    }
}

TEST(Bubble_Sort, THROWS_ON_WRONG_SIZE) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> a;
    if (ProcRank == 0) {
        a = getRandomVector(100);
        ASSERT_ANY_THROW(parallelOddEvenSort(a, 5));
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

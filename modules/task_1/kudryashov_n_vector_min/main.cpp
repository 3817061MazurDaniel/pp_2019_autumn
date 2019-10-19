// Copyright 2019 Kudryashov Nikita
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include "./vector_min.h"

TEST(SequentialVectorMin, low_positive) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> low(4);
    for (int i = 0; i < 4; i++) {
        low[i] = i;
    }

    std::vector<int>::iterator res = std::min_element(low.begin(), low.end());

    if (rank == 0) {
        ASSERT_EQ(*res, 0);
    }
}

TEST(SequentialVectorMin, all_positive_except_one_negative) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> one_pos(100);
    for (int i = 100; i < 199; i++) {
        one_pos[i - 100] = i * i;
    }
    one_pos[99] = - 1;
    std::vector<int>::iterator res = std::min_element(one_pos.begin(), one_pos.end());

    if (rank == 0) {
        ASSERT_EQ(*res, -1);
    }
}

TEST(ParallelVectorMin, low_amount) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> low(5);

    for (int i = 0; i < 5; i++) {
        low[i] = i;
    }

    int paral_res = getVectorMinParallel(low);
    std::vector<int>::iterator seq_res = std::min_element(low.begin(), low.end());

    if (rank == 0) {
        ASSERT_EQ(paral_res, *seq_res);
    }
}

TEST(ParallelVectorMin, high_amount_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> high;

    high = getRandomVector(100);

    int paral_res = getVectorMinParallel(high);
    std::vector<int>::iterator seq_res = std::min_element(high.begin(), high.end());

    if (rank == 0) {
        ASSERT_EQ(paral_res, *seq_res);
    }
}

TEST(ParallelVectorMin, high_amount_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> high;

    high = getRandomVector(500);

    int paral_res = getVectorMinParallel(high);
    std::vector<int>::iterator seq_res = std::min_element(high.begin(), high.end());

    if (rank == 0) {
        ASSERT_EQ(paral_res, *seq_res);
    }
}

TEST(ParallelVectorMin, high_amount_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> high;

    high = getRandomVector(1000);

    int paral_res = getVectorMinParallel(high);
    std::vector<int>::iterator seq_res = std::min_element(high.begin(), high.end());

    if (rank == 0) {
        ASSERT_EQ(paral_res, *seq_res);
    }
}

TEST(ParallelVectorMin, all_positive_decreasing) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> decr(100);

    for (int i = 0; i < 100; i++) {
        decr[i] = 100 - i;
    }

    int paral_res = getVectorMinParallel(decr);
    std::vector<int>::iterator seq_res = std::min_element(decr.begin(), decr.end());

    if (rank == 0) {
        ASSERT_EQ(*seq_res, paral_res);
    }
}

TEST(ParallelVectorMin, all_negative_increasing) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> incr(100);

    for (int i = 0; i < 100; i++) {
        incr[i] = -100 + i;
    }

    int paral_res = getVectorMinParallel(incr);
    std::vector<int>::iterator seq_res = std::min_element(incr.begin(), incr.end());

    if (rank == 0) {
        ASSERT_EQ(*seq_res, paral_res);
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

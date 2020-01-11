// Copyright 2019 Vedrukov Pavel
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include "./freq_symb.h"

TEST(freq_symb, zero_freq) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str = "ChargeOfDarkness";
    char symb = '0';
    double freq = getFreq(str, symb);
    if (rank == 0) {
        ASSERT_DOUBLE_EQ(0.0, freq);
    }
}

TEST(freq_symb, full_freq) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str = "oooooooo";
    char symb = 'o';
    double freq = getFreq(str, symb);
    if (rank == 0) {
        ASSERT_DOUBLE_EQ(1.0, freq);
    }
}

TEST(count_frequency, some_freq_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str = "pool";
    char symb = 'o';
    double freq = getFreq(str, symb);
    if (rank == 0) {
        ASSERT_DOUBLE_EQ(0.5, freq);
    }
}

TEST(count_frequency, some_freq_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str = "pointclick";
    char symb = 'i';
    double freq = getFreq(str, symb);
    if (rank == 0) {
        ASSERT_DOUBLE_EQ(0.2, freq);
    }
}

TEST(count_frequency, some_freq_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str = "limbobimbo";
    char symb = 'b';
    double freq = getFreq(str, symb);
    if (rank == 0) {
        ASSERT_DOUBLE_EQ(0.3, freq);
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

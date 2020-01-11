// Copyright 2019 Ryabova Alyona
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include "./count_frequency.h"

TEST(count_frequency, count_frequency_in_random_str) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str = getRandomString(5000);
    char c = 'a';

    int count = getCountCharInStr(str, c);

    if (rank == 0) {
       int count2 = getCount(str, c);
       ASSERT_EQ(count2, count);
    }
}

TEST(count_frequency, count_frequency_in_str_with_one_char) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str = "aaaaaaaaaaa";
    char c = 'a';

    int count = getCountCharInStr(str, c);

    if (rank == 0) {
        ASSERT_EQ(11, count);
    }
}

TEST(count_frequency, count_frequency_in_str_without_char) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str = "sdfghjkl";
    char c = 'a';

    int count = getCountCharInStr(str, c);

    if (rank == 0) {
        ASSERT_EQ(0, count);
    }
}

TEST(count_frequency, count_frequency_in_odd_str) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str = "arianadad";
    char c = 'a';

    int count = getCountCharInStr(str, c);

    if (rank == 0) {
        ASSERT_EQ(4, count);
    }
}

TEST(count_frequency, count_frequency_in_even_str) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str = "ariana";
    char c = 'a';

    int count = getCountCharInStr(str, c);

    if (rank == 0) {
        ASSERT_EQ(3, count);
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

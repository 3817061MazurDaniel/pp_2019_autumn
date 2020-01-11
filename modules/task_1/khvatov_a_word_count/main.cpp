// Copyright 2019 Khvatov Alexander
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>

#include <string>

#include "./word_count.h"

TEST(Word_Count_MPI, Seq_Incorrect_String) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (0 == rank) {
        ASSERT_ANY_THROW(seq_word_count(""));
    }
}

TEST(Word_Count_MPI, Par_Incorrect_String) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    ASSERT_ANY_THROW(parallel_word_count(""));
}

TEST(Word_Count_MPI, Seq_Empty_String) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str;

    if (0 == rank) {
        ASSERT_ANY_THROW(seq_word_count(str));
    }
}

TEST(Word_Count_MPI, Parallel_Empty_String) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str;

    ASSERT_ANY_THROW(parallel_word_count(str));
}

TEST(Word_Count_MPI, Seq_Correct_Count) {
    int rank = 0, result = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (0 == rank) {
        result = seq_word_count("two words");
        ASSERT_EQ(2, result);
    }
}

TEST(Word_Count_MPI, Parallel_Correct_Count) {
    int rank = 0, result = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    result = parallel_word_count("two words");

    if (0 == rank) {
        ASSERT_EQ(2, result);
    }
}

TEST(Word_Count_MPI, Parallel_Count_Equal_Seq_Count) {
    int rank = 0, seq_result = 0, par_result = 0;
    std::string test = "string for test";
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    par_result = parallel_word_count(test);
    if (0 == rank) {
        seq_result = seq_word_count(test);
    }

    if (0 == rank) {
        ASSERT_EQ(seq_result, par_result);
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

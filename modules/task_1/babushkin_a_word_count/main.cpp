// Copyright 2019 Babsuhkin Alexey

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>

#include <string>

#include "../../../modules/task_1/babushkin_a_word_count/word_count.h"

TEST(Word_Count_MPI, Test_Seq_Counting) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::string testString("this is a test string");
        int wCountSeq = wordCount(testString);

        ASSERT_EQ(wCountSeq, 5);
    }
}

TEST(Word_Count_MPI, Test_Parll_Counting) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string testString;
    if (rank == 0) {
        testString  = "this is a test string";
    }
    int wCount = wordCountParallel(testString);

    if (rank == 0) {
        ASSERT_EQ(wCount, 5);
    }
}

TEST(Word_Count_MPI, Test_Parll_Counting_Size_20) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string testString;
    if (rank == 0) {
        testString = randomString(20);
    }
    int wCount = wordCountParallel(testString);

    if (rank == 0) {
        ASSERT_EQ(20, wCount);
    }
}

TEST(Word_Count_MPI, Test_Parll_Counting_Size_50) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string testString;

    if (rank == 0) {
        testString = randomString(50);
    }
    int wCount = wordCountParallel(testString);

    if (rank == 0) {
        ASSERT_EQ(50, wCount);
    }
}

TEST(Word_Count_MPI, Test_Parll_Counting_Size_35) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string testString;

    if (rank == 0) {
        testString = randomString(35);
    }
    int wCount = wordCountParallel(testString);

    if (rank == 0) {
        ASSERT_EQ(35, wCount);
    }
}

TEST(Word_Count_MPI, Test_Empty_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::string testString("");
        ASSERT_ANY_THROW(wordCount(testString));
    }
}

TEST(Word_Count_MPI, Test_Extra_Space) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string testString;
    if (rank == 0) {
        testString = "this is a test string ";
    }
    int wCount = wordCountParallel(testString);

    if (rank == 0) {
        int wCountSeq = wordCount(testString) - 1;
        ASSERT_NE(wCountSeq, wCount);
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

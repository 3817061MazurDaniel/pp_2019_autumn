// Copyright 2019 Bykovskaya Alina
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <cstring>

#include "./compare_strings.h"

TEST(Compare_Strings_MPI, Test_Not_Parallel_Compare) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char> s1(10, 'a'), s2(10, 'a'), s3(10, 'a');
    s3[0] = 'b';

    int ans1 = compare(s1, s2, s1.size());
    int ans2 = compare(s1, s3, s1.size());

    if (rank == 0) {
        ASSERT_EQ(0, ans1);
        ASSERT_NE(0, ans2);
    }
}

TEST(Compare_Strings_MPI, Test_Parallel_Compare) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char> s1(10, 'a'), s2(10, 'a'), s3(10, 'a');
    s3[0] = 'b';

    int ans1 = parallelCompareStrings(s1, s2, s1.size());
    int ans2 = parallelCompareStrings(s1, s3, s1.size());

    if (rank == 0) {
        ASSERT_EQ(0, ans1);
        ASSERT_NE(0, ans2);
    }
}

TEST(Compare_Strings_MPI, Different_Compare_Gives_Same_Answer) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char> s1(10, 'a'), s2(10, 'a'), s3(10, 'a');
    s3[0] = 'b';

    int ans1 = parallelCompareStrings(s1, s2, s1.size());
    int ans2 = parallelCompareStrings(s1, s3, s1.size());


    if (rank == 0) {
        int ref_ans1 = compare(s1, s2, s1.size());
        int ref_ans2 = compare(s1, s3, s1.size());
        ASSERT_EQ(ref_ans1, ans1);
        ASSERT_EQ(ref_ans2, ans2);
    }
}

TEST(Compare_Strings_MPI, Test_Strings_Size_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char> s1, s2;
    int size = 5;
    if (rank == 0) {
        s1 = getRandomString(size);
        s2 = getRandomString(size);
    }

    int ans = parallelCompareStrings(s1, s2, size);

    if (rank == 0) {
        int ref_ans = compare(s1, s2, size);
        ASSERT_EQ(ref_ans, ans);
    }
}

TEST(Compare_Strings_MPI, Test_Strings_Size_500) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char> s1, s2;
    int size = 500;
    if (rank == 0) {
        s1 = getRandomString(size);
        s2 = getRandomString(size);
    }

    int ans = parallelCompareStrings(s1, s2, size);

    if (rank == 0) {
        int ref_ans = compare(s1, s2, size);
        ASSERT_EQ(ref_ans, ans);
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

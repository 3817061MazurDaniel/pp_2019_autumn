// Copyright 2019 Kurakin Mikhail
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include "./count_diff_ch.h"

TEST(Count_Diff_Ch_MPI, Test_Rand_Str) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    srand(time(NULL));
    unsigned int expLen = 4;
    if (rank == 0) {
    std::string str = getRandomString(4);
        ASSERT_EQ(expLen, str.length());
    }
}
TEST(Count_Diff_Ch_MPI, Test_Find_Count_Diff_Ch_Sequential) {
    int rank;
    int expResult = 17;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str1 = "Mika12345";
    std::string str2 = "arbuz6789x";
    int countDiff;
    if (rank == 0) {
        countDiff = getCountDiffChSeq(str1, str2, 0, str1.length());
        ASSERT_EQ(expResult, countDiff);
    }
}
TEST(Count_Diff_Ch_MPI, Test_Find_Count_Diff_Ch_Sequential_Rand) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str1, str2;
    if (rank == 0) {
    str1 = getRandomString(200);
    str2 = getRandomString(200);
    ASSERT_NO_THROW(getCountDiffChSeq(str1, str2, 0, str1.length()));
    }
}

TEST(Count_Diff_Char_MPI, Count_Diff_Char_Same_Str) {
  int rank;
  int expResult = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string str1, str2;
  str1 = "123456789";
  str2 = "123456789";
  int countDiff;
  countDiff = getCountDiffChPar(str1, str2);
  if (rank == 0) {
    ASSERT_EQ(expResult, countDiff);
  }
}

TEST(Count_Diff_Ch_MPI, Test_Find_Count_Diff_Ch_Parallel) {
    int rank;
    int expResult;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str1;
    std::string str2;
    for (int i = 0; i < 10000; i++) {
        str1.append("2");
        str2.append("1");
    }
    str1.append("0");
    str2.append("0");
    int countDiff = getCountDiffChPar(str1, str2);
    if (rank == 0) {
        expResult = getCountDiffChSeq(str1, str2, 0, str1.length());
        ASSERT_EQ(expResult, countDiff);
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


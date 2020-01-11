// Copyright 2019 Poletueva Anastasia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "./poletueva_a_counting_the_number_of_words_mpi.h"

TEST(Count_Words_MPI, Eseq_Str) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::string testStr = "Life is a nightmare that prevents one from sleeping";
  if (rank == 0) {
    int res = getCountWords(testStr);
    ASSERT_EQ(res, 9);
  }
}

TEST(Count_Words_MPI, Paral_Str) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string testStr = "As cool as a cucumber";
  int res = getCountWordsParallel(testStr);

  if (rank == 0) {
    ASSERT_EQ(res, 5);
  }
}

TEST(Count_Words_MPI, Empty_String) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    std::string testStr("");
    ASSERT_ANY_THROW(getCountWords(testStr));
  }
}

TEST(Count_Words_MPI, Eseq_String_Spaces) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::string testStr = "Life   is a nightmare that prevents one   from sleeping ";
  if (rank == 0) {
    int res = getCountWords(testStr);
    ASSERT_EQ(res, 9);
  }
}

TEST(Count_Words_MPI, String) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string testStr;

  if (rank == 0) {
    testStr = getRandomStr();
  }

  int global_count = getCountWordsParallel(testStr);
  int reference_count;

  if (rank == 0) {
    reference_count = getCountWords(testStr);
  }

  if (rank == 0) {
    ASSERT_EQ(global_count, reference_count);
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

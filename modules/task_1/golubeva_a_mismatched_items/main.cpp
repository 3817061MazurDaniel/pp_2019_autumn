  // Copyright 2019 Golubeva Anna
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cstdlib>
#include <cstring>
#include "../../../modules/task_1/golubeva_a_mismatched_items/mismatched_items.h"

TEST(Count_Diff_Char_MPI, count_diff_char_in_same_size_str_even) {
  int rank;
  int expResult = 6;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  char str1[] = "sdaaaaaa";
  char str2[] = "sdgggggg";
  int countDiff;
  countDiff = getCountDiffChar(str1, str2);
  if (rank == 0) {
    ASSERT_EQ(expResult, countDiff);
  }
}
TEST(Count_Diff_Char_MPI, count_diff_char_in_same_size_str_odd) {
  int rank;
  int expResult = 5;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  char str1[] = "sdaaaaa";
  char str2[] = "sdggggg";
  int countDiff;
  countDiff = getCountDiffChar(str1, str2);
  if (rank == 0) {
    ASSERT_EQ(expResult, countDiff);
  }
}
TEST(Count_Diff_Char_MPI, count_diff_char_in_diff_size_str) {
  int rank;
  int expResult = 5;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  char str1[] = "sdaaa";
  char str2[] = "sdggggg";
  int countDiff;
  countDiff = getCountDiffChar(str1, str2);
  if (rank == 0) {
    ASSERT_EQ(expResult, countDiff);
  }
}
TEST(Count_Diff_Char_MPI, count_diff_char_in_identical_str) {
  int rank;
  int expResult = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  char str1[] = "qwerty";
  char str2[] = "qwerty";
  int countDiff;
  countDiff = getCountDiffChar(str1, str2);
  if (rank == 0) {
    ASSERT_EQ(expResult, countDiff);
  }
}
TEST(Count_Diff_Char_MPI, get_random_str) {
  int size = 23;
  char *str = new char[size];
  ASSERT_NO_THROW(getRandomStr(str, size));
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

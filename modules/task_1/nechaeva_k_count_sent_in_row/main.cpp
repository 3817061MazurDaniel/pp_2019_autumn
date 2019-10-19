// Copyright 2019 Nechaeva Ekaterina
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include "../../../modules/task_1/nechaeva_k_count_sent_in_row/count_sent.h"

TEST(Number_of_sentences, correct_size) {
  std::string a;
  ASSERT_ANY_THROW(a = RandomString(-5));
}

TEST(Number_of_sentences, can_create_string) {
  std::string a;
  ASSERT_NO_THROW(a = RandomString(20));
}

TEST(Number_of_sentences, number_of_sentences_in_simple_string) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::string a = "asdfghjkl";
  int k = GetCountInText(a);

  if (rank == 0) {
    int ks = GetCountInPart(a);
    ASSERT_EQ(k, ks);
  }
}

TEST(Number_of_sentences, number_of_sentences) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::string a = "asd.fgh.jkl.";
  int k = GetCountInText(a);
  if (rank == 0) {
    ASSERT_EQ(k, 3);
  }
}
TEST(Number_of_sentences, random_string) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::string a;
  a = RandomString(20);
  int k = GetCountInText(a);
  if (rank == 0) {
    int ks = GetCountInPart(a);
    ASSERT_EQ(k, ks);
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

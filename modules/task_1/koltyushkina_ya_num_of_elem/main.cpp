// Copyright 2019 Koltyushkina Yanina
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <string>
#include "../../../modules/task_1/koltyushkina_ya_num_of_elem/num_of_elem.h"

TEST(Number_of_elements, correct_size) {
  std::string str;
  ASSERT_ANY_THROW(str = RandomString(-1));
}

TEST(Number_of_elements, create_string) {
  std::string str;
  ASSERT_NO_THROW(str = RandomString(7));
}

TEST(Number_of_elements, correct_num_of_elem_norm) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::string str = "abc de";
  int n = GetNumOnText(str);
  if (rank == 0) {
    ASSERT_EQ(n, 5);
  }
}

TEST(Number_of_elements, correct_num_of_elem_2_space) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::string str = "abc  de";
  int n = GetNumOnText(str);
  if (rank == 0) {
    ASSERT_EQ(n, 5);
  }
}

TEST(Number_of_elements, correct_num_of_elem_random_string) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::string str;
  str = RandomString(11);
  int n = GetNumOnText(str);
  if (rank == 0) {
    int nn = GetNumOnPart(str);
    ASSERT_EQ(n, nn);
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

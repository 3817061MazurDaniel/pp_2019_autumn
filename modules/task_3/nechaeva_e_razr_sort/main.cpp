// Copyright 2019 Nechaeva Ekaterina
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <random>
#include <cassert>
#include <vector>
#include "../../../modules/task_3/nechaeva_e_razr_sort/razr_sort.h"

TEST(Razr_Sort, sort_random_vec) {
  int nrank;
  MPI_Comm_rank(MPI_COMM_WORLD, &nrank);
  std::vector<double> mas(6);
  Rand(mas, 6);
  mas = ParallSort(mas, 6);
  if (nrank == 0) {
    ASSERT_TRUE(Tru(mas, 6) == true);
  }
}

TEST(Razr_Sort, sort_pol_vec) {
  int nrank;
  MPI_Comm_rank(MPI_COMM_WORLD, &nrank);
  std::vector<double> mas = {3.7, 30.1, 11.5, 1.6};
  std::vector<double> sort = { 1.6, 3.7, 11.5, 30.1 };
  mas = ParallSort(mas, mas.size());
  if (nrank == 0) {
    ASSERT_EQ(mas, sort);
  }
}

TEST(Razr_Sort, error_size) {
  std::vector<double> mas = { 3.7, -30.1, -1.5, 1.6 };
  std::vector<double> sort = { -30.1 , -1.5, 1.6, 3.7 };
  ASSERT_ANY_THROW(ParallSort(mas, 0));
}

TEST(Razr_Sort, sort_neg_vec) {
  int nrank;
  MPI_Comm_rank(MPI_COMM_WORLD, &nrank);
  std::vector<double> mas = { -30.7, -300.1, -1.7, -25.6 };
  std::vector<double> sort = { -300.1, -30.7, -25.6, -1.7};
  mas = ParallSort(mas, mas.size());
  if (nrank == 0) {
    ASSERT_EQ(mas, sort);
  }
}

TEST(Razr_Sort, sort_neg_and_pol_vec) {
  int nrank;
  MPI_Comm_rank(MPI_COMM_WORLD, &nrank);
  std::vector<double> mas = { 3.7, -30.1, -10.5, 1.6 };
  std::vector<double> sort = { -30.1 , -10.5, 1.6, 3.7 };
  mas = ParallSort(mas, mas.size());
  if (nrank == 0) {
    ASSERT_EQ(mas, sort);
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

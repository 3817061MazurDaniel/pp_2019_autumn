// Copyright 2019 Kukushkina Ksenia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./quick_batcher.h"

TEST(Quick_Batcher_Sort, Random_Generation) {
  std::vector<int> vec(1000);
  ASSERT_NO_THROW(generateRand(&vec));
}

TEST(Quick_Batcher_Sort, Empty_Vector) {
  std::vector<int> vec(0);
  ASSERT_ANY_THROW(quickBatcher(&vec));
}

TEST(Quick_Batcher_Sort, Odd_Size) {
  std::vector<int> vec(101);
  generateRand(&vec);
  ASSERT_NO_THROW(quickBatcher(&vec));
}

TEST(Quick_Batcher_Sort, Pow_2_Size) {
  std::vector<int> vec(256);
  generateRand(&vec);
  ASSERT_NO_THROW(quickBatcher(&vec));
}

TEST(Quick_Batcher_Sort, Quick_Sort) {
  std::vector<int> vec(100);
  generateRand(&vec);
  quickSort(&vec, 0, 99);
  ASSERT_EQ(isSorted(vec, 99), true);
}

TEST(Quick_Batcher_Sort, Batcher_Sort) {
  std::vector<int> vec(1000);
  generateRand(&vec);
  quickBatcher(&vec);
  ASSERT_EQ(isSorted(vec, 999), true);
}


/*TEST(Quick_Batcher_Sort, Efficiency) {
  std::vector<int> vec(1000), vec1(1000);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double t1, t2, finish;
  generateRand(&vec);
  vec1 = vec;
  double start = MPI_Wtime();
  quickSort(&vec, 0, 999);
  finish = MPI_Wtime();
  t1 = finish - start;
  start = MPI_Wtime();
  quickBatcher(&vec1);
  finish = MPI_Wtime();
  t2 = finish - start;
  if (rank == 0)
    std::cout << "Qsort: " << t1 << "s\nBatcher: " << t2 << "s\n" << "k = " << t1 / t2 << std::endl;
  ASSERT_EQ(t1 > t2, true);
}*/

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

// Copyright 2019 Soboleva Julia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "../../../modules/task_3/soboleva_j_strongin_search/strongin_search.h"

TEST(StronginSearch, Can_Create_VectorPoint1) {
  ASSERT_NO_THROW(VectorPointX A(-10, 10, 1));
}

TEST(StronginSearch, Can_Create_VectorPoint2) {
  std::vector<double> k(4);
  ASSERT_NO_THROW(VectorPointX B(-10, 10, 1, k));
}

TEST(StronginSearch, Count_Z_Is_Correct) {
  VectorPointX A(-10, 10, 2, { 1, -4, 4 });
  ASSERT_EQ(A.countZ(2), 0);
}

TEST(StronginSearch, Can_Add_Point) {
  VectorPointX A(-10, 10, 1);
  ASSERT_NO_THROW(A.addPoint(-7));
}

TEST(StronginSearch, Add_Point_Throws) {
  VectorPointX A(-10, 10, 1);
  ASSERT_ANY_THROW(A.addPoint(17));
}

TEST(StronginSearch, Search_Is_Correct) {
  VectorPointX A(-10, 10, 2, { 1, 2, -3 });
  ASSERT_EQ(static_cast<int>(stronginSearch(A, 1, 3)), -1);
}

TEST(StronginSearch, par_Search_Is_Correct) {
  VectorPointX A(-10, 10, 2, { 1, 2, -3 });
  ASSERT_EQ(static_cast<int>(parStronginSearch(A, 1, 3)), -1);
}

/*TEST(StronginSearch, Eff_Test) {
	VectorPointX A(-10000, 10000, 20);
	double start, end, res;
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		start = MPI_Wtime();
		res = stronginSearch(A, 20, 6);
		end = MPI_Wtime();
		cout << "Non-parallel time: " << end - start << "\n";
		start = MPI_Wtime();
		res = parStronginSearch(A, 20, 6);
		end = MPI_Wtime();
		cout << "Parallel time: " << end - start << "\n";
}*/


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}

// Copyright 2019 Soboleva Julia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "../../../modules/task_2/soboleva_j_hypercube/hypercube.h"

TEST(My_Hypercube, Get_Dimension_Is_Correct) {
  EXPECT_EQ(getDimension(63), 6);
}

TEST(My_Hypercube, Get_Dimension_Throws) {
  ASSERT_ANY_THROW(getDimension(-7));
}

TEST(My_Hypercube, Get_Count_Vertex_Is_Correct) {
  EXPECT_EQ(getCountVertex(12), 4096);
}

TEST(My_Hypercube, Get_Count_Vertex_Throws) {
  ASSERT_ANY_THROW(getCountVertex(-7));
}

TEST(My_Hypercube, Can_Get_Hypercube) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int dim = getDimension(size);

  if ((size == getCountVertex(dim)) && (dim > 0)) {
    MPI_Comm cube = getHypercube(dim);
    int myDim;
    MPI_Cartdim_get(cube, &myDim);
    EXPECT_EQ(dim, myDim);
  }
}

TEST(My_Hypercube, Get_Hypercube_throws) {
  ASSERT_ANY_THROW(getHypercube(-7));
}

TEST(My_Hypercube, Can_Transfer_Data) {
  int rank, size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int dim = getDimension(size);
  int tmp = 0;

  if ((size == getCountVertex(dim)) && (dim > 0)) {
    MPI_Comm cube = getHypercube(dim);
    if (rank == 0) {
      tmp = 17;
    }
    for (int i = 0; i < dim; i++) {
      int count = getCountVertex(i);
      for (int j = 0; j < count; j++) {
        if (rank == j)
          MPI_Send(&tmp, 1, MPI_INT, j + count, j + count, cube);
        if (rank == j + count) {
          MPI_Status status;
          MPI_Recv(&tmp, 1, MPI_INT, j, j + count, cube, &status);
        }
      }
    }
    ASSERT_EQ(tmp, 17);
  } else {
    ASSERT_EQ(tmp, 0);
  }
}
/*
TEST(My_Hypercube, Eff_Test) {
	int rank, size;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int dim = getDimension(size);
	int tmp = 0;
	double start, end;
	if (rank == 0) {
		tmp = 17;
	}
	if ((size == getCountVertex(dim)) && (dim > 0)) {
		start = MPI_Wtime();
		MPI_Comm cube = getHypercube(dim);		
		for (int i = 0; i < dim; i++) {
			int count = getCountVertex(i);
			for (int j = 0; j < count; j++) {
				if (rank == j)
					MPI_Send(&tmp, 1, MPI_INT, j + count, j + count, cube);
				if (rank == j + count) {
					MPI_Status status;
					MPI_Recv(&tmp, 1, MPI_INT, j, j + count, cube, &status);
				}

			}
		}
		end = MPI_Wtime();
		if(rank == size - 1)
			cout << "Hypercube time:  " << end - start << "\n";
	}
	start = MPI_Wtime();
	MPI_Bcast(&tmp, 1, MPI_INT, 0, MPI_COMM_WORLD);
	end = MPI_Wtime();
	if (rank == size - 1)
		cout << "MPI_COMM_WORLD time:  " << end - start << "\n";
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

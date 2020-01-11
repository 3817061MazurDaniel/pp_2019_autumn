// Copyright 2019 Golubeva Anna
#include "../../../modules/task_2/golubeva_a_ring_topology/ring_topology.h"
#include <random>
#include <cstring>
#define NUM_DIMS 1

MPI_Comm createRingComm(MPI_Comm old, int size) {
  int reorder = 0;
  int dims[NUM_DIMS]{ 0 }, periods[NUM_DIMS]{ 1 };
  int oldSize;
  MPI_Comm_size(old, &oldSize);
  if (size <= 0 || size > oldSize)
    MPI_Dims_create(oldSize, NUM_DIMS, dims);
  else
    MPI_Dims_create(size, NUM_DIMS, dims);
  MPI_Comm commRing;
  MPI_Cart_create(MPI_COMM_WORLD, NUM_DIMS, dims, periods, reorder, &commRing);
  return commRing;
}

bool testRingTopology(MPI_Comm commRing) {
  if (!isRingTopology(commRing)) return false;
  int rank, size, source, dest;
  int A;
  int B;
  MPI_Status status;
  MPI_Comm_rank(commRing, &rank);
  MPI_Comm_size(commRing, &size);
  A = rank;
  B = -1;
  MPI_Cart_shift(commRing, 0, 1, &source, &dest);
  if (rank == 0) {
    MPI_Send(&A, 1, MPI_INT, dest, 1, commRing);
    MPI_Recv(&B, 1, MPI_INT, source, 1, commRing, &status);
  } else {
    MPI_Recv(&B, 1, MPI_INT, source, 1, commRing, &status);
    MPI_Send(&B, 1, MPI_INT, dest, 1, commRing);
  }
  MPI_Comm_free(&commRing);
  return true;
}

bool isRingTopology(MPI_Comm comm) {
  int numDims, status;
  MPI_Topo_test(comm, &status);
  if (status != MPI_CART) return false;
  MPI_Cartdim_get(comm, &numDims);
  if (numDims != NUM_DIMS) return false;
  int dims[NUM_DIMS], periods[NUM_DIMS], coords[NUM_DIMS];
  MPI_Cart_get(comm, NUM_DIMS, dims, periods, coords);
  if (periods[0] != 1) return false;
  return true;
}

// Copyright 2019 Ostapovich Denis

#include "../../../modules/task_2/ostapovich_d_linear_topology/linear.h"
#include <iostream>

MPI_Comm createBus(MPI_Comm baseComm) {
  MPI_Comm busComm = 0;
  // number of processes in bus
  int busSize[] = { 0 };
  // for linear topology
  int periodic[] = { 0 };
  int reorder = 0;
  // retrieving number of processes from given communicator
  MPI_Comm_size(baseComm, &busSize[0]);
  MPI_Dims_create(busSize[0], NUM_OF_DIMENSIONS, busSize);
  MPI_Cart_create(MPI_COMM_WORLD, NUM_OF_DIMENSIONS, busSize, periodic, reorder, &busComm);
  return busComm;
}

bool isBus(MPI_Comm comm) {
  int status, nDims;
  MPI_Topo_test(comm, &status);
  if (status == MPI_CART) {
    MPI_Cartdim_get(comm, &nDims);
    if (nDims != NUM_OF_DIMENSIONS) {
      return false;
    }
    int dims[NUM_OF_DIMENSIONS], periods[NUM_OF_DIMENSIONS], coords[NUM_OF_DIMENSIONS];
    MPI_Cart_get(comm, NUM_OF_DIMENSIONS, dims, periods, coords);
    if (periods[0] != 0) {
      return false;
    }
    return true;
  } else {
    return false;
  }
}

void pingBus(MPI_Comm comm) {
  MPI_Status status;
  MPI_Comm bus;
  int rank, size;

  if (isBus(comm)) {
    bus = comm;
  } else {
    throw "Not a bus topology";
  }

  MPI_Comm_rank(bus, &rank);
  MPI_Comm_size(bus, &size);
  int buffer;
  int src = 0, dst = 0;

  // sending forward
  buffer = rank;
  MPI_Cart_shift(bus, 0, 1, &src, &dst);
  MPI_Sendrecv_replace(&buffer, 1, MPI_INT, dst, 0, src, 0, bus, &status);
  // checking whether process recieving its left neighbour rank
  if ((buffer != rank - 1) && rank > 0) {
    throw "Error occured during forward transmission";
  }

  buffer = rank;
  MPI_Cart_shift(bus, 0, -1, &src, &dst);
  MPI_Sendrecv_replace(&buffer, 1, MPI_INT, dst, 0, src, 0, bus, &status);
  // checking whether process recieving its right neighbour rank
  if ((buffer != rank + 1) && (rank < size - 1)) {
    throw "Error occured during backward transmission";
  }

  MPI_Comm_free(&bus);
}


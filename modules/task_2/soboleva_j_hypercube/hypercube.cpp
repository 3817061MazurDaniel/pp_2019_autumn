// Copyright 2019 Soboleva Julia
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include "../../../modules/task_2/soboleva_j_hypercube/hypercube.h"

int getDimension(int n) {
  if (n < 0)
    throw "Cart size error!!!";
  int d = 0;
  int tmp = getCountVertex(d);

  while (tmp < n) {
    d++;
    tmp = getCountVertex(d);
  }
  return d;
}

int getCountVertex(int d) {
  if (d < 0)
    throw "Cart size error!!!";

  int v = 1;
  for (int i = 0; i < d; i++)
    v = v * 2;

  return v;
}

MPI_Comm getHypercube(int dim) {
  if (dim < 1) {
    throw "Cart size error!!!";
  }
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int* dimension = new int[dim];
  int* period = new int[dim];
  for (int i = 0; i < dim; i++) {
    period[i] = 1;
    dimension[i] = 2;
  }

  if (size == getCountVertex(dim)) {
    MPI_Comm cube;
    MPI_Cart_create(MPI_COMM_WORLD, dim, dimension, period, 1, &cube);
    return cube;
  } else {
    return MPI_COMM_WORLD;
  }
}

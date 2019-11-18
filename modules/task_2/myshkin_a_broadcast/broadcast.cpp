// Copyright 2019 Myshkin Andrew
#include <mpi.h>
#include <random>
#include <stdexcept>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_2/myshkin_a_broadcast/broadcast.h"


int* getRandomArrayInt(int sizeA) {
  if (sizeA <= 0)
    throw std::runtime_error("Error Array size");
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  int *localBuf = reinterpret_cast<int*>(malloc(sizeof(int) * sizeA));
  for (int i = 0; i < sizeA; i++) {
    localBuf[i] = gen() % 100;
  }
  return localBuf;
}


double* getRandomArrayDouble(int sizeA) {
  if (sizeA <= 0)
    throw std::runtime_error("Error Array size");
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  double *localBuf = reinterpret_cast<double*>(malloc(sizeof(double) * sizeA));
  for (int i = 0; i < sizeA; i++) {
    localBuf[i] = gen() % 100;
  }
  return localBuf;
}


float* getRandomArrayFloat(int sizeA) {
  if (sizeA <= 0)
    throw std::runtime_error("Error Array size");
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  float *localBuf = reinterpret_cast<float*>(malloc(sizeof(float) * sizeA));
  for (int i = 0; i < sizeA; i++) {
    localBuf[i] = gen() % 100;
  }
  return localBuf;
}


int getMaxArrayInt(int* buffer, int sizeA) {
  int max = buffer[0];
  for (int i = 0; i < sizeA; i++) {
    if (buffer[i] > max) {
      max = buffer[i];
    }
  }
  return max;
}


double getMaxArrayDouble(double* buffer, int sizeA) {
  double max = buffer[0];
  for (int i = 0; i < sizeA; i++) {
    if (buffer[i] > max) {
      max = buffer[i];
    }
  }
  return max;
}


float getMaxArrayFloat(float* buffer, int sizeA) {
  float max = buffer[0];
  for (int i = 0; i < sizeA; i++) {
    if (buffer[i] > max) {
      max = buffer[i];
    }
  }
  return max;
}


void myBroadcast(void *buf, int count, MPI_Datatype type, int root, MPI_Comm comm) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (count <= 0) {
    throw std::runtime_error("Error Array size");
  }

  if (rank == root) {
    for (int i = 0; i < size; i++) {
      if (i != rank) {
        MPI_Send(buf, count, type, i, 1, comm);
      }
    }
  } else {
    MPI_Status Status;
    MPI_Recv(buf, count, type, root, 1, comm, &Status);
  }
}

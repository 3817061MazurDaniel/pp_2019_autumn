// Copyright 2019 Zhbanova Nadezhda

// #include <windows.h>
#include <stdio.h>
#include <ctime>
#include <vector>
#include <iostream>
#include <cmath>
#include <random>
#include "../../../modules/task_2/zhbanova_n_data_network_topology_ring/data_network_topology_ring.h"
#include "./mpi.h"

const int MAX_BUFF_SIZE = 1000;

// filling a vector with random values
std::vector<int> randomInputVector(int size) {
  std::random_device rnd_device;
  std::mt19937 mersenne_engine(rnd_device());
  std::vector<int> vec;
  for (int i = 0; i < size; i++)
    vec.push_back(mersenne_engine() % 10);
  return vec;
}

// we pass the resulting vector from the receiver process to the sender process
// to check the correct delivery of the vector, in tests
std::vector<int> transferDataToCheck(int rank, int rankFrom, int rankTo, std::vector<int> vec) {
  std::vector<int> resultVec(VEC_SIZE);
  if (rank == rankTo) {
    MPI_Send(&vec[0], vec.size(), MPI_INT, rankFrom, 0, MPI_COMM_WORLD);
  }

  if (rank == rankFrom) {
    resultVec.resize(VEC_SIZE);
    MPI_Status status;   // received message parameter
    MPI_Recv(&resultVec[0], VEC_SIZE, MPI_INT, rankTo, 0, MPI_COMM_WORLD, &status);
  }
  return resultVec;
}

std::vector<int> sendVector(int rankFrom, int rankTo, std::vector<int> vec) {
  int rank;   // process
  int size;   // number of processes
  std::vector<int> resultVec;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rankFrom < 0 || rankFrom >= size) {
    throw std::runtime_error("Error: wrong process FROM numer");
  }
  if (rankTo < 0 || rankTo >= size) {
    throw std::runtime_error("Error: wrong process TO numer");
  }
  if (rankFrom == rankTo) {
    throw std::runtime_error("Error: request to send data to itself.");
  }

  if (rank == rankFrom) {   // if it is a sender process
    sendByRing(vec, rank, size);
  } else if (rank == rankTo) {   // if it is a recipient process
    resultVec = recvFromRing(rank, size);
  } else if (rankFrom < rankTo && rank > rankFrom && rank < rankTo) {   // 0 proc ->..-> N proc, when rankFrom < rankTo
    resultVec = recvFromRing(rank, size);
    sendByRing(resultVec, rank, size);
  } else if (rankFrom > rankTo  && ((rank > rankFrom && rank < size) || (rank < rankTo))) {
    //  ..-> 0 proc ->.., when rankFrom > rankTo
    resultVec = recvFromRing(rank, size);
    sendByRing(resultVec, rank, size);
  }
  return resultVec;
}

void sendByRing(std::vector<int> vec, int rank, int size) {
  int pos = 0;
  char buff[MAX_BUFF_SIZE];
  int vecSize = vec.size();

  MPI_Pack(&vecSize, 1, MPI_INT, buff, MAX_BUFF_SIZE, &pos, MPI_COMM_WORLD);   // how much we pack
  MPI_Pack(&vec[0], vecSize, MPI_INT, buff, MAX_BUFF_SIZE, &pos, MPI_COMM_WORLD);   // pack the vector itself
  // (rank + 1) % size - recipient process number
  MPI_Send(buff, pos, MPI_PACKED, (rank + 1) % size, 0, MPI_COMM_WORLD);
}

std::vector<int> recvFromRing(int rank, int size) {
  int pos = 0;
  char buff[MAX_BUFF_SIZE];
  int vecSize;
  std::vector<int> vec;
  MPI_Status status;   // received message parameter
  // (rank - 1 + size) % size - sender process number
  MPI_Recv(buff, MAX_BUFF_SIZE, MPI_PACKED, (rank - 1 + size) % size, 0, MPI_COMM_WORLD, &status);
  MPI_Unpack(buff, MAX_BUFF_SIZE, &pos, &vecSize, 1, MPI_INT, MPI_COMM_WORLD);
  vec.resize(vecSize);
  MPI_Unpack(buff, MAX_BUFF_SIZE, &pos, &vec[0], vecSize, MPI_INT, MPI_COMM_WORLD);
  return vec;
}



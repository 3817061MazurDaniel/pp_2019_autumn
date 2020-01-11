// Copyright 2019 Ostapovich Denis

#include "../../../modules/task_1/ostapovich_d_max_elem_vector/max_elem_vector.h"
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>

std::vector<int> GetVector(int size, int mode = 0) {
  std::mt19937 generator;
  generator.seed(time(0));
  int min, max;
  switch (mode) {
  case VMAX_POSITIVE: {min = 1;
    max = 100;
    break;
  }
  case VMAX_NEGATIVE: {min = -100;
    max = -1;
    break;
  }
  case VMAX_MIXED: {min = -100;
    max = 100;
    break;
  }
  default: {throw - 1;
    break;
  }
  }

  std::vector<int> vector;
  for (int i = 0; i < size; i++) {
    vector.push_back(generator() % (max - min + 1) + min);
  }

  return vector;
}

int SequentialMaxValue(std::vector<int> vec) {
  int max = *std::max_element(vec.begin(), vec.end());
  return max;
}

void segmentation(int size, int worldSize, int rank, int* segmentStart, int* segmentSize) {
  *segmentStart = size / worldSize * rank;
  *segmentSize = size / worldSize;
  if (rank == worldSize - 1) {
    *segmentSize += size % worldSize;
  }
}

int ParallelMaxValue(std::vector<int> publicVector) {
  int worldRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
  int worldSize;
  MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

  int segmentStart = 0;
  int segmentSize = 0;
  if (static_cast<int>(publicVector.size()) <= worldSize) {
    return SequentialMaxValue(publicVector);
  }
  segmentation(publicVector.size(), worldSize, worldRank, &segmentStart, &segmentSize);
  std::vector<int> privateVector = std::vector<int>(publicVector.begin() + segmentStart,
    publicVector.begin() + segmentStart + segmentSize);

  int parallelMax = 0;
  int privateMax = SequentialMaxValue(privateVector);
  MPI_Reduce(&privateMax, &parallelMax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

  return parallelMax;
}

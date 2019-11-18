// Copyright 2019 Ostapovich Denis

#include "../../../modules/task_1/ostapovich_d_max_elem_vector/max_elem_vector.h"
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>

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

int ParallelMaxValue(std::vector<int> publicVector) {
  int worldRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
  int worldSize;
  MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

  int step = publicVector.size() / worldSize;

  if (worldRank == 0) {
    for (int id = 1; id < worldSize; id++) {
      MPI_Send(&publicVector[0] + id * step, step, MPI_INT, id, 0, MPI_COMM_WORLD);
    }
  }

  std::vector<int> privateVector(step);

  if (worldRank == 0) {
    privateVector = std::vector<int>(publicVector.begin(), publicVector.begin() + step);
  } else {
    MPI_Recv(&privateVector[0], step, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  int parallelMax = 0;
  int seqMax = SequentialMaxValue(publicVector);
  MPI_Reduce(&seqMax, &parallelMax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

  return parallelMax;
}

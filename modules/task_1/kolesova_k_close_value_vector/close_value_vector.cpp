// Copyright 2019 Kolesova Kristina
#include <mpi.h>
#include <random>
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>
#include "../../../modules/task_1/kolesova_k_close_value_vector/close_value_vector.h"

std::vector<int> getRandomVector(int size) {
  std::random_device ran;
  std::vector<int> Vec(size);
  std::mt19937 engine(ran());
  for (int i = 0; i < size; i++) {
    Vec[i] = engine() % 100;
  }
  return Vec;
}

int getMinDifferenceOneProc(const std::vector <int> Vec) {
  int difMin = INT_MAX;
  if (Vec.size() != 0) {
    for (int i = 0; i < static_cast<int>(Vec.size()) - 1; i++) {
      if (difMin > abs(Vec[i] - Vec[i + 1]))
        difMin = abs(Vec[i] - Vec[i + 1]);
    }
  }
  if (Vec.size() == 1) {
    return 0;
  }
  return difMin;
}

int getMinDifference(const std::vector <int> Vec) {
  int difMin = INT_MAX;
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;

  int modSize = Vec.size() - 1;
  int modf = modSize / size;
  int res = modSize % size;

  if (rank == 0) {
    if (modf > 0) {
      for (int noProc = 1; noProc < size; noProc++)
        MPI_Send(&Vec[res] + modf * noProc, modf + 1, MPI_INT, noProc, 0, MPI_COMM_WORLD);
    }
  } else {
    if (modf > 0) {
      std::vector<int> tmp(modf + 1);
      MPI_Recv(&tmp[0], modf + 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      int minD = getMinDifferenceOneProc(tmp);
      MPI_Send(&minD, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  }
  if (rank == 0) {
    int procRest;
    std::vector<int> tmp(Vec.begin(), Vec.begin() + 1 + modf + res);
    difMin = getMinDifferenceOneProc(tmp);
    if (modf > 0) {
      for (int noProc = 1; noProc < size; noProc++) {
        MPI_Recv(&procRest, 1, MPI_INT, noProc, 0, MPI_COMM_WORLD, &status);
        difMin = std::min(difMin, procRest);
      }
    }
  }
  return difMin;
}

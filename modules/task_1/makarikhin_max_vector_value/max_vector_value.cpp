// Copyright 2019 Makarikhin Semen
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <stdexcept>
#include "../../../modules/task_1/makarikhin_max_vector_value/max_vector_value.h"


std::vector<int> getRandomVector(int sz) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> vec(sz);
  for (int i = 0; i < sz; i++) {
    vec[i] = gen() % 100;
  }
  return vec;
}

int GetMaxVectorValueParallel(std::vector<int> global_vec, int size_vector) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int delta = size_vector / size;
  const int residue_size_vector = size_vector % size;

  if (size_vector == 0) {
    throw std::runtime_error("Vector size 0");
  }

  if (size_vector < size) {
    return *std::max_element(global_vec.begin(), global_vec.end());
  }

  if (rank == 0) {
    for (int proc = 1; proc < size; proc++) {
      MPI_Send(&global_vec[0] + proc * delta + residue_size_vector, delta, MPI_INT, proc, 0, MPI_COMM_WORLD);
    }
  }

  std::vector<int> local_vec;

  if (rank == 0) {
    local_vec = std::vector<int>(global_vec.begin(), global_vec.begin() + delta + residue_size_vector);
  } else {
    local_vec.resize(delta);
    MPI_Status status;
    MPI_Recv(&local_vec[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }

  int local_max = *std::max_element(local_vec.begin(), local_vec.end());
  int global_max = 0;
  MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  return global_max;
}

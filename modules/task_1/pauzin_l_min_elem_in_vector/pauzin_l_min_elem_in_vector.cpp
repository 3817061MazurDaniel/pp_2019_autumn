// Copyright 2019 Pauzin Leonid
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/pauzin_l_min_elem_in_vector/pauzin_l_min_elem_in_vector.h"



int minElemInVec(const std::vector <int> global_vec, int n) {
  int rezult = 10;
  int min;
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int delta = n / size;
  const int remainder = n % size;
  std::vector<int> local_vec(delta);
  if (n < size) {
    if (rank == 0) {
      rezult = *std::min_element(global_vec.begin(), global_vec.end());
      return rezult;
    } else {
      return rezult;
    }
  }

  if (rank == 0) {
    for (int proc = 1; proc < size; proc++) {
      MPI_Send(&global_vec[0] + proc * delta + remainder, delta, MPI_INT, proc, 0, MPI_COMM_WORLD);
    }
    local_vec.resize(delta + remainder);
    local_vec = std::vector<int>(global_vec.begin(), global_vec.begin() + delta + remainder);
    min = *std::min_element(local_vec.begin(), local_vec.end());
  } else {
    MPI_Status status;
    MPI_Recv(&local_vec[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    min = *std::min_element(local_vec.begin(), local_vec.end());
  }

  MPI_Reduce(&min, &rezult, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
  return rezult;
}

std::vector<int> getVec(int n) {
  std::vector<int> vec1(n);
  for (int i = 0; i < n; i++) {
    vec1[i] = i;
  }
  return vec1;
}

std::vector<int> getRandomVector(int n) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> vec(n);
  for (int i = 0; i < n; i++) { vec[i] = gen() % 100; }
  return vec;
}

// Copyright 2019 Shashkin Evgeny
#include <mpi.h>
#include <random>
#include <ctime>
#include <vector>
#include <iostream>
#include "../../../modules/task_1/shashkin_e_violations_of_order_in_vector/violations_of_order_in_vector.h"


std::vector<int> GetRandomVector(size_t size) {
  std::mt19937 gen;
  gen.seed(time(0));
  std::vector<int> vec(size);

  for (size_t i = 0; i < size; ++i) {
    vec[i] = gen() % 10;
  }

  return vec;
}

int GetCountOfViolationsOfOrderInVectorParallel(const std::vector<int> &vec, int vec_size) {
  int size, rank;
  int local_count = 0;
  int total_count = 0;
  MPI_Status status;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int delta = vec_size / size;
  const int remainder = vec_size % size;

  if (vec_size < size) {
    return GetCountOfViolationsOfOrderInVector(vec);
  }
  std::vector<int> local_vec(delta+1);

  if (rank == 0) {
    if (delta > 0) {
      for (int proc = 1; proc < size; ++proc) {
        MPI_Send(&vec[0] + proc * delta+remainder-1, delta+1, MPI_INT, proc, 0, MPI_COMM_WORLD);
      }
    }
    local_vec = std::vector<int>(vec.begin(), vec.begin() + delta + remainder);
  } else {
    if (delta > 0) {
      MPI_Recv(&local_vec[0], delta+1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
  }

  local_count = GetCountOfViolationsOfOrderInVector(local_vec);

  if (delta > 0) {
    MPI_Reduce(&local_count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  }

  return total_count;
}

int GetCountOfViolationsOfOrderInVector(const std::vector<int> &vec) {
  int count_of_violations = 0;

  for (size_t i = 1; i < vec.size(); ++i) {
    if (vec[i-1] > vec[i]) {
      count_of_violations++;
    }
  }
  return count_of_violations;
}

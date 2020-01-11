// Copyright 2019 Zhbanova Nadezhda

#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include "../../../modules/task_1/zhbanova_n_most_diff_vectors_elems/most_diff_vectors_elems.h"
#include "./mpi.h"

// creates, fills a vector of a given length, returns it
std::vector<int> randomInput(int size) {
  std::random_device rnd_device;
  std::mt19937 mersenne_engine(rnd_device());
  std::vector<int> vec;
  for (int i = 0; i < size; i++)
    vec.push_back(mersenne_engine() % 10);
  return vec;
}

// returns the modulus of the largest difference of neighboring elements
int maxDiff(std::vector<int> v) {
  int diff = 0;
  if (v.size() >= 2) {
    // neighboring pairs are always 1 less than vector elements -> v.size() - 1
    for (unsigned int i = 0; i < v.size() - 1; i++) {
      if (abs(v[i] - v[i + 1]) > diff) {
        diff = abs(v[i] - v[i + 1]);
      }
    }
  } else {
    throw("error");
  }
  return diff;
}

// parallel solution
int maxDiffMpi(std::vector<int> v) {
  if (v.size() < 2) {
    throw("error");
  }

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int step = 1, rest = 0;
  // if there are fewer processes than the number of comparisons
  if ((unsigned int)size < v.size() - 1) {
    step = (v.size() - 1) / size;  // how many pairs should the process handle
    rest = (v.size() - 1) % size;  // balance - how many pairs remain incomparable
  }

  // process 0 will process the first step pairs and the remainder which is not distributed between processes
  // the rest will count step pairs with a shift from what process 0 counted
  if (rank == 0) {
    for (int proc_rank = 1; proc_rank < size; proc_rank++) {
      MPI_Send(&v[0] + proc_rank * step + rest, step + 1, MPI_INT, proc_rank, 0, MPI_COMM_WORLD);
    }
  }

  // each process forms a vector that will process
  std::vector<int> v_proc(step + 1);
  if (rank == 0) {
    // half interval used [..)
    v_proc = std::vector<int>(v.begin(), v.begin() + step + rest + 1);
  } else {
    // all processes need to receive data from process 0
    MPI_Status status;
    MPI_Recv(&v_proc[0], step + 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }

  int diff = 0, diff_proc = 0;

  // if there are more processes than pairs to be compared, only the first v.size() - 1 processes will work
  if ((unsigned int)rank < v.size() - 1) {
     // process counts its vector
     diff_proc = maxDiff(v_proc);
  }

  // combine results on all processes
  MPI_Reduce(&diff_proc, &diff, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  return diff;
}

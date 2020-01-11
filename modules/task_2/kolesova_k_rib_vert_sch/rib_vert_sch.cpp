// Copyright 2019 Kolesova Kristina

#include <../../../modules/task_2/kolesova_k_rib_vert_sch/rib_vert_sch.h>
#include <mpi.h>
#include <vector>
#include <random>

std::vector<int> randVec(std::size_t n) {
  std::vector<int> vec(n);
  std::mt19937 engine;
  for (std::size_t i = 0; i < n; i++) {
    vec[i] = engine() % 10;
  }
  return vec;
}

std::vector<int> randMatr(std::size_t m, std::size_t n) {
  std::vector<int> matr(m*n);
  std::mt19937 engine;
  for (std::size_t i = 0; i < m*n; i++) {
    matr[i] = engine() % 10;
  }
  return matr;
}

std::vector<int> calcMatrOneProc(std::vector<int> matr, std::vector<int> vec, std::size_t n, std::size_t m) {
  if (n != vec.size()) {
    throw - 1;
  }

  std::vector<int> resVec(m);

  for (std::size_t i = 0; i < m; i++) {
    for (std::size_t j = 0; j < n; j++) {
      resVec[i] += matr[i*n + j] * vec[j];
    }
  }
  return resVec;
}

std::vector<int> calcMatr(std::vector<int> matr, std::vector<int> vec, std::size_t n, std::size_t m) {
  if (n != vec.size()) {
    throw - 1;
  }

  int size, rank;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::size_t modf = m * n / size;
  std::size_t res = m * n % size;

  std::vector<int> resVec(m);
  std::vector<int> transpMatr(m*n);
  std::vector<int> multMatr(m*n);
  std::vector<int> tmpResVec(modf);

  int id = 0;

  for (std::size_t i = 0; i < n; i++) {
    for (std::size_t j = 0; j < m; j++) {
      transpMatr[id++] = matr[j*n + i];
    }
  }

  MPI_Scatter(&transpMatr[res], modf, MPI_INT, &tmpResVec[0], modf, MPI_INT, 0, MPI_COMM_WORLD);

  for (std::size_t i = 0; i < modf; i++) {
    tmpResVec[i] *= vec[(rank*modf + i + res)/m];
  }

  MPI_Gather(&tmpResVec[0], modf, MPI_INT, &multMatr[res], modf, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    for (std::size_t i = 0; i < res; i++) {
      multMatr[i] = transpMatr[i] * vec[i / m];
    }

    for (std::size_t i = 0; i < m*n; i++) {
      resVec[i%m] += multMatr[i];
    }
  }
  return resVec;
}

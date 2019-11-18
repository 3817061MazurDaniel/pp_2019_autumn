// Copyright 2019 Soboleva Julia
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/soboleva_j_max_in_matrix_row/max_in_matrix_row.h"

std::vector<int> getRandMatrix(int m, int n) {
  if ((m < 0) || (n < 0))
    throw "Matrix size error!!!";
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> a(m * n);
  for (int i = 0; i < m * n; i++) {
    a[i] = gen() % 100;
  }
  return a;
}

std::vector<int> getMaxInMatrixRows(const std::vector<int>& a, int m, int n) {
  std::vector<int> res;
  for (int i = 0; i < m; i++) {
    int first = i * n;
    int next = (i + 1) * n;

    res.push_back(*std::max_element(a.begin() + first, a.begin() + next));
  }
  return res;
}

std::vector<int> getParMaxInMatrixRows(const std::vector<int>& a, int m, int n) {
  if (m <= 0 || n <= 0)
    throw "Matrix size error!!!";
  int size, rank;
  std::vector<int> rez(m);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (size == 1) {
    rez = getMaxInMatrixRows(a, m, n);
  } else {
    const int interval = m / size;
    const int mod = m % size;
    std::vector<int> vec(interval * n);
    std::vector<int> max_in_sector(interval);

    if (rank == 0) {
      for (int pr = 1; pr < size; pr++)
        MPI_Send(&a[(interval + mod) * n] + (pr - 1) * interval * n, interval * n, MPI_INT, pr, 0, MPI_COMM_WORLD);
      vec = a;
      vec.resize((interval + mod) * n);
      max_in_sector.resize(interval + mod);
      max_in_sector = getMaxInMatrixRows(vec, interval + mod, n);
    }

    if (rank != 0) {
      MPI_Status status;
      MPI_Recv(&vec[0], interval * n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
      max_in_sector = getMaxInMatrixRows(vec, interval, n);
      MPI_Send(&max_in_sector[0], interval, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    if (rank == 0) {
      rez = max_in_sector;
      MPI_Status status;
      for (int pr = 1; pr < size; pr++) {
        std::vector<int> tmp(interval);
        MPI_Recv(&tmp[0], interval, MPI_INT, pr, 1, MPI_COMM_WORLD, &status);
        rez.insert(rez.end(), tmp.begin(), tmp.end());
      }
    }
  }
  return rez;
}

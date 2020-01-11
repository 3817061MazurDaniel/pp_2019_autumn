// Copyright 2019 Mityagina Daria
#include "../../../modules/task_3/mityagina_d_quick_sort/quick_sort_p.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cstring>
#include <ctime>
#include <random>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>

std::vector<int> getRandomVector(int size) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> _vector(size);
  for (int i = 0; i < size; ++i) {
    _vector[i] = gen() % 100;
  }
  return _vector;
}

std::vector<int> quick_s(std::vector<int> _vector, int left, int right) {
  if (left < right) {
    int t = part(_vector, left, right);
    _vector = part_vect(_vector, left, right);
    _vector = quick_s(_vector, left, t);
    _vector = quick_s(_vector, t + 1, right);
  }
  return _vector;
}

std::vector<int> Merge_my_vectors(std::vector<int> mv1, std::vector<int> mv2, int m, int n) {
  int i = 0, j = 0, k = 0;
  std::vector<int> result = std::vector<int>(m + n, 0);
  while (i < m && j < n) {
    if (mv1[i] <= mv2[j]) {
      result[k] = mv1[i];
      i++;
    } else {
      result[k] = mv2[j];
      j++;
    }
    k++;
  }
  if (i < m) {
    for (int p = i; p < m; p++) {
      result[k] = mv1[p];
      k++;
    }
  } else {
    for (int p = j; p < n; p++) {
      result[k] = mv2[p];
      k++;
    }
  }
  return result;
}

std::vector<int> main_work(std::vector<int> my_vector, int N) {
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int eachProc = (N / size > 0)? (N / size) : N;
  int add = N % size;
  MPI_Status st;
  std::vector<int> sub_my_vector;
  if (eachProc > 0)
    sub_my_vector = std::vector<int>(eachProc, 0);
  std::vector<int> result = std::vector<int>(N, 0);
  result = std::vector<int>(my_vector.begin(), my_vector.end());

  if (size > 1 && N / size > 0) {
    if (rank == 0) {
      for (int i = 1; i < size; i++) {
        if (eachProc * i + add <= N - eachProc && eachProc > 0)
          MPI_Send(&my_vector[add] + eachProc * i - 1, eachProc, MPI_INT, i, i, MPI_COMM_WORLD);
      }
    }
    if (rank == 0) {
      if (eachProc + add > 0)
        sub_my_vector.resize(eachProc + add);
      sub_my_vector = std::vector<int>(my_vector.begin(), my_vector.begin() + eachProc + add);
    } else {
        if (eachProc > 0 && eachProc < N) {
          sub_my_vector.resize(eachProc);
          MPI_Recv(&sub_my_vector[0], eachProc, MPI_INT, 0, rank, MPI_COMM_WORLD, &st);
        }
    }
    int right = (rank == 0)? eachProc + add - 1 : eachProc - 1;
    quick_s(sub_my_vector, 0, right);
    if (rank == 0) {
      for (int i = 0; i < eachProc + add; i++) {
        result[i] = sub_my_vector[i];
      }
    }
    if (rank != 0) {
      if (eachProc > 0)
        MPI_Send(&sub_my_vector[0], eachProc, MPI_INT, 0, rank * 10, MPI_COMM_WORLD);
    } else {
      for (int i = 1; i < size; i++) {
        if (eachProc > 0 && add + eachProc * i - 1 <= N - eachProc)
          MPI_Recv(&result[add] + eachProc * i - 1, eachProc, MPI_INT, MPI_ANY_SOURCE, i * 10, MPI_COMM_WORLD, &st);
      }
    }
    std::vector<int> out = std::vector<int>(N, 0);
    sort(result.begin(), result.end());
    out = result;
    return out;
  } else {
    std::vector<int> out = std::vector<int>(N, 0);
    out = quick_s(result, 0, N - 1);
    return out;
  }
}

std::vector<int> part_vect(std::vector<int> _vector, int left, int right) {
  int x = _vector[left];
  int tmp = 0;
  int t = left;
  for (int i = left + 1; i <= right; i++) {
    if (_vector[i] < x) {
      t++;
      tmp = _vector[t];
     _vector[t] = _vector[i];
     _vector[i] = tmp;
    }
  }
  tmp = _vector[left];
  _vector[left] = _vector[t];
  _vector[t] = tmp;
  return _vector;
}

int part(std::vector<int> _vector, int left, int right) {
  int x = _vector[left];
  int tmp = 0;
  int t = left;
  for (int i = left + 1; i <= right; i++) {
    if (_vector[i] < x) {
      t++;
      tmp = _vector[t];
     _vector[t] = _vector[i];
     _vector[i] = tmp;
    }
  }
  tmp = _vector[left];
  _vector[left] = _vector[t];
  _vector[t] = tmp;
  return t;
}

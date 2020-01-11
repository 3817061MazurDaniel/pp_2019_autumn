// Copyright 2019 Poletueva Anastasia
#include <mpi.h>
#include <ctime>
#include <algorithm>
#include <vector>
#include <random>
#include<iostream>
#include "../../../modules/task_2/poletueva_a_contrast_enhancement/poletueva_a_contrast_enhancement.h"

const int L = 256;

std::vector<int> ContrastFilter(const std::vector <int>& a, int rows, int cols, int _correction) {
  if (rows < 0 || cols < 0)
    throw Error;
  int correction = _correction;
  std::vector<int> res(cols*rows);

  int lAB = 0;
  int value = 0;

  // Find the brightness of all pixels

  for (int i = 0; i <rows*cols; i ++) {
    value += a[i];
  }

  // average brightness of all pixels
  lAB = value/(rows*cols);

  // Correction factor
  double k = 1.0 + correction / 100.0;

  // contrast change algorithm
  std::vector<int> temp(256);

  for (int i = 0; i < L; i++) {
    int delta = static_cast<int> (i) - lAB;
    temp[i] = static_cast<int>(lAB + k *delta);

    if (temp[i] <= 0)
      temp[i] = 0;

    if (temp[i] >= 255)
      temp[i] = 255;
  }

  for (int i = 0; i < cols*rows; i++) {
    res[i] = temp[a[i]];
    // std::cout << i << ' ' << res[i] << std::endl;
  }
  return res;
}

std::vector<int> ContrastFilterParallel(const std::vector <int> a, int rows, int cols, int _correction) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rows < 0 || cols < 0)
    throw Error;

  std::vector<int> res(cols*rows);
  int delta = rows*cols/size;
  int rem = rows*cols % size;

  if (delta < 1) {
    std::vector<int> r(rows*cols);
    if (rank == 0) {
      r = ContrastFilter(a, rows, cols, _correction);
    }
    MPI_Bcast(&r[0], delta, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    return r;
  }

    if (rank == 0) {
      for (int i = delta + rem; i <= rows * cols - delta; i += delta) {
        MPI_Send(&a[i], delta, MPI_INT, (i - rem) / delta, 0, MPI_COMM_WORLD);
    }
  }
  MPI_Status status;
  std::vector <int>a1(delta, 0);
  int tmp = 0;
  int s = 0;
  int lAB = 0;
  int correction = _correction;
  std::vector <int> temp(256);

  if (rank == 0) {
    for (int i = 0; i < delta + rem; i++)
      tmp += a[i];
  } else {
    MPI_Recv(&a1[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    for (int i = 0; i < delta; i++)
      tmp += a1[i];
  }

  MPI_Reduce(&tmp, &s, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    double k = 1.0 + correction / 100.0;
    lAB = s/ (rows*cols);
    for (int i = 0; i < L; i++) {
      int del = static_cast<int>(i) - lAB;
      temp[i] = static_cast<int>(lAB + k *del);

      if (temp[i] <= 0)
        temp[i] = 0;

      if (temp[i] >= 255)
        temp[i] = 255;
    }
  }


  std::vector <int>a2(delta, 0);

  MPI_Bcast(&temp[0], 256, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    for (int i = 0; i < delta + rem; i++) {
      res[i] = temp[a[i]];
     // std::cout << i << ' ' << res[i]<<' ' << a[i] << std::endl;
    }
  } else {
    for (int i = 0; i < delta; i++) {
      a2[i] = temp[a1[i]];
     // std::cout << rank * delta + rem + i << ' ' << a2[i] << ' ' << a1[i] <<' '<<rank << std::endl;
    }
  }

  if (rank != 0) {
     MPI_Send(&a2[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  std::vector <int> temp2(delta);

  if (rank == 0) {
    for (int proc = 1; proc < size; proc++) {
      MPI_Recv(&temp2[0], delta, MPI_INT, proc, 0, MPI_COMM_WORLD, &status);
      for (int i = 0; i < delta; i++) {
        res[proc*delta+i+rem] = temp2[i];
        }
    }
  }
  return res;
  }

std::vector<int> getRandomM(int rows, int cols) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));

  std::vector<int>a(rows*cols);

  for (int i = 0; i < rows*cols; i++) {
    a[i] = gen() % 256;
  }
  return a;
}

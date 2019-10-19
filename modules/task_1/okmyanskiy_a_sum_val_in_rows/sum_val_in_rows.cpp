// Copyright 2019 Okmyanskiy Andrey
#include <random>
#include <vector>
#include <stdexcept>
#include <ctime>
#include "../../../modules/task_1/okmyanskiy_a_sum_val_in_rows/sum_val_in_rows.h"
std::vector<int> getRandomMatrix(int rows, int cols) {
  if (rows <= 0 || cols <= 0)
    throw std::runtime_error("Matrix size <= 0");
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> vec(rows*cols);
  for (int i = 0; i < rows*cols; i++)
    vec[i] = gen() % 100;
  return vec;
}

std::vector<int> getParallelSumValInRows(const std::vector<int> VEC, int rows, int cols) {
  int ProcRank, ProcNum, ProcSum = 0;
  MPI_Status Status;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int whole = rows / ProcNum;
  int remainder = rows % ProcNum;
  std::vector<int> VEC_2(rows);
  std::vector<int> VV(cols*whole, 0);
  std::vector<int> SUM(whole);

  int err = 0;
  if (ProcRank == 0) {
  if (rows*cols != static_cast<int>(VEC.size()))
    err = -1;
  for (int i = 1; i < ProcNum; i++)
    MPI_Send(&err, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
  } else {
    MPI_Recv(&err, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &Status);
  }
  if (err == -1)
    throw std::runtime_error("Matrix size does not match description");

  if (ProcRank == 0) {
    if (whole > 0) {
      for (int i = 1; i < ProcNum; i++)
        MPI_Send(&VEC[remainder*cols] + (i*cols*whole), cols*whole, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
  VV = std::vector<int>(VEC.begin(), VEC.begin() + cols * (whole + remainder));
  SUM.resize(VV.size() / cols);
  } else {
    if (whole > 0)
      MPI_Recv(&VV[0], cols*whole, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
  }

  for (unsigned int i = 0, k = 0; i < SUM.size(); i++) {
    ProcSum = 0;
    for (int j = 0; j < cols; j++, k++) {
      ProcSum += VV[k];
    }
    SUM[i] = ProcSum;
  }

  if (ProcRank == 0) {
    for (unsigned int i = 0; i < SUM.size(); i++)
      VEC_2[i] = SUM[i];
  }

  if (ProcRank == 0 && whole > 0) {
    int y = SUM.size();
    for (int i = 1; i < ProcNum; i++) {
      MPI_Recv(&VEC_2[y] + whole * (i - 1), whole, MPI_INT, i, 0, MPI_COMM_WORLD, &Status);
    }
  } else if (whole > 0) {
    MPI_Send(&SUM[0], whole, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  return VEC_2;
}

std::vector<int> getSequintialSumValInRows(const std::vector<int> VEC, int rows, int cols) {
  if (rows * cols != static_cast<int>(VEC.size()))
    throw std::runtime_error("Matrix size does not match description");
  int Sum;
  std::vector<int> VEC_2(rows);
  for (int i = 0, k = 0; i < rows; i++) {
    Sum = 0;
    for (int j = 0; j < cols; j++, k++) {
      Sum += VEC[k];
    }
    VEC_2[i] = Sum;
  }
  return VEC_2;
}

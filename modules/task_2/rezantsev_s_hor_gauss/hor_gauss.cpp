// Copyright 2019 Rezantsev Sergey

#include "../../../modules/task_2/rezantsev_s_hor_gauss/hor_gauss.h"
#include <mpi.h>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

std::vector<double> getRandMatrix(int n) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<double> matrix(n * (n + 1));
  for (int i = 0; i < n * (n + 1); i++) {
    matrix[i] = gen() % 100;
  }
  return matrix;
}

std::vector<double> getGauss(const std::vector<double> &a, int n) {
  double m;
  std::vector<double> res(n);
  std::vector<double> temp(a);
  for (int k = 0; k < n; k++) {
    for (int j = k; j < n; j++) {
      m = temp[j * (n + 1) + k];
      for (int i = 0; i < n + 1; i++) {
        temp[j * (n + 1) + i] /= m;
      }
      if (j != k) {
        for (int i = 0; i < n + 1; i++) {
          temp[j * (n + 1) + i] = temp[j * (n + 1) + i] - temp[k * (n + 1) + i];
        }
      }
    }
  }
  for (int i = n - 1; i >= 0; i--) {
    res[i] = temp[i * (n + 1) + n];
    for (int c = 0; c < i; c++) {
      temp[c * (n + 1) + n] -= temp[c * (n + 1) + i] * res[i];
    }
  }
  return res;
}

bool isItTrueAnswer(const std::vector<double> &a, int n, const std::vector<double> &x) {
  std::vector<double> res(n);
  double e = 1e-9;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      res[i] += a[(n + 1) * i + j] * x[j];
    }
  }
  for (int i = 0; i < n; i++) {
    if (std::abs(res[i] - a[i * (n + 1) + n]) > e) {
      return false;
    }
  }
  return true;
}

std::vector<double> getParGauss(const std::vector<double> &a, int n) {
  int size, rank;
  std::vector<double> vec(n + 1);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int delta = n / size;
  int rem = n % size;
  int tmp = (delta + (rank < rem ? 1 : 0));
  if (rank == 0) {
    if (n <= 0) throw std::runtime_error("n <=0");
  }
  std::vector<double> temp(tmp * (n + 1));
  if (rank == 0) {
    for (int proc = size - 1; proc >= 0; proc--) {
      int ind = 0;
      for (int i = proc; i < n; i += size) {
        for (int j = 0; j < n + 1; j++) {
          temp[ind++] = a[j + i * (n + 1)];
        }
      }
      if (proc > 0) {
        MPI_Send(temp.data(), ind, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD);
      }
    }
  } else {
    MPI_Status status;
    MPI_Recv(temp.data(), temp.size(), MPI_DOUBLE, 0, 1, MPI_COMM_WORLD,
             &status);
  }
  std::vector<int> tapesCounts(size);
  std::vector<int> displs(size);
  int m = n + 1;
  displs[0] = 0;
  for (int i = 0; i < size; i++) {
    tapesCounts[i] = (delta + (i < rem ? 1 : 0)) * m;
    if (i > 0) {
      displs[i] = (displs[i - 1] + tapesCounts[i - 1]);
    }
  }
  std::vector<double> rowVec(m);
  for (int i = 0; i < displs[rank] / m; i++) {
    int root = 0;
    int sum = 0;
    for (int j = 0; j < size; j++, ++root) {
      sum += tapesCounts[j] / m;
      if (i < sum) {
        root = j;
        break;
      }
    }
    MPI_Bcast(rowVec.data(), m, MPI_DOUBLE, root, MPI_COMM_WORLD);
    for (int j = 0; j < tapesCounts[rank] / m; j++) {
      double s = rowVec[i] / temp[j * m + i];
      for (int k = i; k < m; k++) {
        temp[j * m + k] = s * temp[j * m + k] - rowVec[k];
      }
    }
  }
  for (int i = 0; i < tapesCounts[rank] / m; ++i) {
    for (int j = 0; j < m; j++) {
      rowVec[j] = temp[i * m + j];
    }
    MPI_Bcast(rowVec.data(), m, MPI_DOUBLE, rank, MPI_COMM_WORLD);
    for (int j = i + 1; j < tapesCounts[rank] / m; j++) {
      double s = rowVec[displs[rank] / m + i] /
                 temp[j * m + i + displs[rank] / m];
      for (int k = i + displs[rank] / m; k < m; k++) {
        temp[j * m + k] = s * temp[j * m + k] - rowVec[k];
      }
    }
  }
  int root = 0;
  for (int i = displs[rank] / m + tapesCounts[rank] / m; i < n; i++) {
    int sum = 0;
    for (int j = 0; j < size; j++, ++root) {
      sum += tapesCounts[j] / m;
      if (i < sum) {
        root = j;
        break;
      }
    }
    MPI_Bcast(rowVec.data(), m, MPI_DOUBLE, root, MPI_COMM_WORLD);
  }
  std::vector<double> res(0);
  if (rank == 0) res.resize(n * m);
  MPI_Gatherv(temp.data(), tmp * m, MPI_DOUBLE, res.data(),
              tapesCounts.data(), displs.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
  std::vector<double> result(0);
  if (rank == 0) {
    result.resize(n);
    for (int i = n - 1; i >= 0; i--) {
      double b = res[i * m + m - 1];
      for (int j = n - 1; j >= i + 1; j--) {
        b -= res[i * m + j] * result[j];
      }
      result[i] = b / res[i * m + i];
    }
  }
  return result;
}

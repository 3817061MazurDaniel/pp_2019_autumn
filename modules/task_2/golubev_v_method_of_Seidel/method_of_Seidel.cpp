// Copyright 2019 Golubev Vladislav
#include <vector>
#include "../../../modules/task_2/golubev_v_method_of_Seidel/method_of_Seidel.h"
using std::vector;

double EvklNorm(const vector<double>& x) {
  double norm = 0;
  for (auto i = x.cbegin(); i != x.cend(); ++i) {
    norm += *i * *i;
  }
  return sqrt(norm);
}

vector<double> SequentialZeidel(const vector<vector<double> >&  A, const vector<double>& b, int n, double eps) {
  vector<double> x(n, 0);
  vector<double> prev_x(x);
  vector<double> tmp(n);
  double sum;
  for (int i = 0; i < n; ++i) {
    x[i] = 0;
  }
  do {
    for (int i = 0; i < n; ++i) {
      prev_x[i] = x[i];
    }
    for (int i = 0; i < n; ++i) {
      sum = 0;
      for (int j = 0; j < i; ++j) {
        sum += A[i][j] * x[j];
      }
      for (int j = i + 1; j < n; ++j) {
        sum += A[i][j] * prev_x[j];
      }
      x[i] = (b[i] - sum) / A[i][i];
    }
    for (int i = 0; i < n; ++i) {
      tmp[i] = x[i] - prev_x[i];
    }
  } while (EvklNorm(tmp) / EvklNorm(prev_x) > eps);
  return x;
}

vector<double> ParallelZeidel(const vector<vector<double> >& A, const vector<double>& _b, int _n, double eps) {
  int procNum, procRank;
  int n = _n;
  vector<double> localB;
  vector<double> block;
  vector<double> tmp(n);

  int procBlock;
  int lastBlock;
  int diag_elem;
  bool isEnd = 1;
  double lsum = 0;
  double rsum = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  MPI_Status status;
  if (n < procNum || procNum == 1) {
    return SequentialZeidel(A, _b, _n, eps);
  }
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  vector<double> x(n);
  vector<double> prev_x(n, 0);
  procBlock = n / procNum;
  lastBlock = procBlock + n % procNum;
  int ostatok = n % procNum;
  int blockSize = n * procBlock;

  if (procRank == procNum - 1) {
    localB = vector<double>(lastBlock);
    block = vector<double>((size_t)lastBlock * n);
  }  else {
    block = vector<double>(blockSize);
    localB = vector<double>(procBlock);
  }
  MPI_Scatter(&_b[0], procBlock, MPI_DOUBLE, &localB[0], procBlock, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  if (procRank == 0) {
    for (int i = 1; i < procNum; ++i) {
      for (int j = 0; j < procBlock; ++j) {
        MPI_Send(&A[(size_t)i * procBlock + j][0], n, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
      }
    }
    if (ostatok != 0) {
      MPI_Send(&_b[(size_t)procNum * procBlock], ostatok, MPI_DOUBLE, procNum - 1, 0, MPI_COMM_WORLD);
      for (int i = 0; i < ostatok; ++i) {
         MPI_Send(&A[(size_t)procNum * procBlock + i][0], n, MPI_DOUBLE, procNum - 1, 0, MPI_COMM_WORLD);
      }
    }
  } else if (procRank == procNum - 1) {
    for (int i = 0; i < procBlock; ++i) {
      MPI_Recv(&block[(size_t)n * i], n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }
    if (ostatok != 0) {
      MPI_Recv(&localB[(size_t)procBlock], ostatok, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
      for (int i = 0; i < ostatok; ++i) {
        MPI_Recv(&block[(size_t)((size_t)procBlock + i) * n], n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
      }
    }
  } else {
    for (int i = 0; i < procBlock; ++i) {
      MPI_Recv(&block[(size_t)n * i], n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }
  }
  do {
    if (procRank == 0) {
      for (int i = 0; i < procBlock; ++i) {
        for (int j = i + 1; j < n; ++j) {
          rsum += prev_x[j] * A[i][j];
        }
        for (int j = 0; j < i; ++j) {
          lsum += x[j] * A[i][j];
        }
        x[i] = (localB[i] - rsum - lsum) / A[i][i];

        lsum = rsum = 0;
        for (int k = 1; k < procNum; ++k) {
          MPI_Send(&x[i], 1, MPI_DOUBLE, k, 0, MPI_COMM_WORLD);
        }
      }
    } else if (procRank == procNum - 1) {
      for (int i = 0; i < procRank; ++i) {
        for (int j = 0; j < procBlock; ++j) {
          MPI_Recv(&x[(size_t)i * procBlock + j], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
        }
      }
      for (int i = 0; i < lastBlock; ++i) {
        diag_elem = procBlock * procRank + i;
        for (int j = diag_elem + 1; j < n; ++j) {
          rsum += block[(size_t)i * n + j] * prev_x[j];
        }
        for (int j = 0; j < diag_elem; ++j) {
          lsum += block[(size_t)i * n + j] * x[j];
        }
        x[diag_elem] = (localB[i] - rsum - lsum) / block[(size_t)i * n + diag_elem];
        lsum = rsum = 0;
      }
    } else {
      for (int i = 0; i < procRank; ++i) {
        for (int j = 0; j < procBlock; ++j) {
          MPI_Recv(&x[(size_t)i * procBlock + j], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
        }
      }
      for (int i = 0; i < procBlock; ++i) {
        diag_elem = procBlock * procRank + i;
        for (int j = diag_elem + 1; j < n; ++j) {
          rsum += block[(size_t)i * n + j] * prev_x[j];
        }
        for (int j = 0; j < diag_elem; ++j) {
          lsum += block[(size_t)i * n + j] * x[j];
        }
        x[diag_elem] = (localB[i] - rsum - lsum) / block[(size_t)i * n + diag_elem];
        lsum = rsum = 0;
        for (int k = procRank + 1; k < procNum; ++k) {
          MPI_Send(&x[diag_elem], 1, MPI_DOUBLE, k, 0, MPI_COMM_WORLD);
        }
      }
    }

    if (procRank == procNum - 1) {
      MPI_Send(&x[0], n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    if (procRank == 0) {
      MPI_Recv(&x[0], n, MPI_DOUBLE, procNum - 1, 0, MPI_COMM_WORLD, &status);
      for (int i = 0; i < n; ++i) {
        tmp[i] = x[i] - prev_x[i];
      }
      if (EvklNorm(tmp) / EvklNorm(prev_x) < eps) {
        isEnd = false;
      }
    }
    for (int i = 0; i < n; ++i) {
      prev_x[i] = x[i];
    }
    MPI_Bcast(&prev_x[0], n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&isEnd, 1, MPI_C_BOOL, 0, MPI_COMM_WORLD);
  } while (isEnd);
  return x;
}


// Copyright 2019 Tihomirova Mariya
#include <mpi.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include "../../../modules/task_2/tihomirova_m_jacobi/jacobi.h"

double getNorm(const std::vector <double>& A, const std::vector<double> x, const std::vector <double>& b) {
  double norm = 0.0;
  int n = b.size();
  for (int i = 0; i < n; i++) {
    double local_norm = 0.0;
    for (int j = 0; j < n; j++) {
      local_norm += A[i * n + j] * x[j];
    }
    norm += (local_norm - b[i]);
  }
  return norm;
}

std::vector<double> Jacobi(const std::vector <double>& A, std::vector <double> x,
  double eps, int n, int rank, int size, int numprocs) {
  std::vector <double> x_old(n);
  std::vector <double> x_new(n);
  int i, first;
  double dNorm, dVal;
  std::vector <int> displs(numprocs);
  std::vector <int> sendcounts(numprocs);
  MPI_Scan(&size, &first, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  first -= size;
  MPI_Allgather(&size, 1, MPI_INT, &sendcounts[0], 1, MPI_INT, MPI_COMM_WORLD);
  displs[0] = 0;
  for (i = 1; i < numprocs; i++)
    displs[i] = displs[i - 1] + sendcounts[i - 1];
  do {
    for (int i = 0; i < n; ++i) {
      x_old[i] = x[i];
    }
    for (int i = 0; i < size; ++i) {
      double sum = 0;
      for (int j = 0; j < n; j++) {
        if (j != i + first)
          sum += A[i * (n + 1) + j] * x_old[j];
      }
      x[i + first] = (A[i * (n + 1) + n] - sum) /
        A[i * (n + 1) + i + first];
    }
    MPI_Allgatherv(&x[0] + first, size, MPI_DOUBLE, &x_new[0], &sendcounts[0], &displs[0], MPI_DOUBLE, MPI_COMM_WORLD);
    x = x_new;
    dNorm = 0;
    if (rank == 0) {
      for (i = 0; i < n; i++) {
        dVal = fabs(x[i] - x_old[i]);
        if (dNorm < dVal) dNorm = dVal;
      }
    }
    MPI_Bcast(&dNorm, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  } while (eps < dNorm);
  return x;
}
std::vector <double> Parallel(std::vector<double> matrix, std::vector <double> b, int Size, double eps) {
  std::vector <double> A(1);
  int n;
  int numprocs, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    n = Size;
    A = std::vector <double>(n * (n + 1));
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        A[i * (n + 1) + j] = matrix[i * n + j];
      }
      A[i * (n + 1) + n] = b[i];
    }
  }
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&eps, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  std::vector <double> x(n);
  MPI_Bcast(&x[0], n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  int local_n = n / numprocs + (rank < (n % numprocs) ? 1 : 0);
  std::vector <double> A_local(local_n * (n + 1) + (local_n > 0 ? 0 : 1));
  std::vector <int> sendcounts(numprocs);
  std::vector <int> displs(numprocs);
  int local_size = (n + 1) * local_n;
  MPI_Gather(&local_size, 1, MPI_INT, &sendcounts[0], 1, MPI_INT, 0, MPI_COMM_WORLD);
  displs[0] = 0;
  for (int i = 1; i < numprocs; i++)
    displs[i] = displs[i - 1] + sendcounts[i - 1];
  MPI_Scatterv(&A[0], &sendcounts[0], &displs[0], MPI_DOUBLE, &A_local[0], local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  x = Jacobi(A_local, x, eps, n, rank, local_n, numprocs);
  return x;
}

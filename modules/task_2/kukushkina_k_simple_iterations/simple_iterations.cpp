// Copyright 2019 Kukushkina Ksenia
#include <vector>
#include <iostream>
#include <cmath>
#include "../../../modules/task_2/kukushkina_k_simple_iterations/simple_iterations.h"

double Norm(const std::vector<double>& b) {
  int n = static_cast<int>(b.size());
  double norm = 0;
  for (int i = 0; i < n; i++) {
    if (std::abs(b[i]) > norm)
      norm = std::abs(b[i]);
  }
  return norm;
}

std::vector<double> Simple_Iterations(std::vector<double> A, std::vector<double> b, double precision) {
  if (A.size() != b.size() * b.size())
    throw "Non-equal dimensions";
  int n = static_cast<int>(b.size());
  std::vector<double> tmp(n);
  double temp, norm = 0;
  for (int i = 0; i < n; i++) {  // rows, modifying matrix
    temp = A[i * n + i];  // main diagonal element
    if (temp == 0)
      throw "Zero element";
    b[i] /= temp;
    for (int j = 0; j < n; j++) {  // columns
      if (i == j)
        A[i * n + j] = 0;  // index = row * n + column
      else
        A[i * n + j] /= temp;
      if (std::abs(A[i * n + j]) > norm)
        norm = std::abs(A[i * n + j]);
    }
  }

  if (norm >= 1)
    throw "No diagonal prevalence";
  std::vector<double> xold(b);
  std::vector<double> xnew(b);

  do {
    xold = xnew;
    xnew = b;
    norm = 0;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++)
        xnew[i] -= A[i * n + j] * xold[j];
      if (std::abs(xnew[i] - xold[i]) > norm)
        norm = std::abs(xnew[i] - xold[i]);
    }
  } while (norm > precision);
  return xnew;
}

std::vector<double> Simple_Iterations_MPI(std::vector<double> A, std::vector<double> b, double precision) {
  if (A.size() != b.size() * b.size())
    throw "Non-equal dimensions";
  int n = static_cast<int>(b.size());
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (n < size)
    return Simple_Iterations(A, b, precision);
  std::vector<int> countsvec(size), countsmat(size), displsvec(size), displsmat(size);
  int len = n / size;
  int rem = n % size;
  countsvec[0] = len + rem;
  countsmat[0] = (len + rem) * n;
  displsvec[0] = displsmat[0] = 0;
  for (int i = 1; i < size; i++) {
    countsvec[i] = len;
    countsmat[i] = len * n;
    displsvec[i] = rem + len * i;
    displsmat[i] = (rem + len * i) * n;
  }

  std::vector<double> lA(countsmat[rank]);
  std::vector<double> lb(countsvec[rank]);
  std::vector<double> xnew(countsmat[rank]);
  std::vector<double> xold(n);

  MPI_Scatterv(&A[0], &countsmat[0], &displsmat[0], MPI_DOUBLE, &lA[0], countsmat[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatterv(&b[0], &countsvec[0], &displsvec[0], MPI_DOUBLE, &lb[0], countsvec[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

  double diag, norm, lnorm = 0, lflag = 1, flag;
  for (int i = 0; i < countsvec[rank]; i++) {  // preprocess: rows
    diag = lA[displsvec[rank] + i * n + i];  // main diag element
    if (diag == 0)
      lflag = 0;
    lb[i] /= diag;
    for (int j = 0; j < n; j++) {  // cols
      if (j == displsvec[rank] + i)
        lA[n * i + j] = 0;
      else
        lA[n * i + j] /= diag;
      if (std::abs(lA[n * i + j]) > lnorm)
        lnorm = std::abs(lA[n * i + j]);
    }
  }
  MPI_Allreduce(&lflag, &flag, 1, MPI_DOUBLE, MPI_PROD, MPI_COMM_WORLD);
  if (flag == 0)
    throw "Zero element";

  MPI_Allreduce(&lnorm, &norm, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  if (norm >= 1)
    throw "No diagonal prevalence";

  MPI_Gatherv(&lb[0], countsvec[rank], MPI_DOUBLE,
    &xold[0], &countsvec[0], &displsvec[0], MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&xold[0], n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  do {
    lnorm = 0;
    for (int i = 0; i < countsvec[rank]; i++) {  // cols
      xnew[i] = lb[i];
      for (int j = 0; j < n; j++) {
        xnew[i] -= lA[i * n + j] * xold[j];
      }
      if (std::abs(xnew[i] - xold[displsvec[rank] + i]) > lnorm) {
        lnorm = std::abs(xnew[i] - xold[displsvec[rank] + i]);
      }
    }
    MPI_Gatherv(&xnew[0], countsvec[rank], MPI_DOUBLE,
      &xold[0], &countsvec[0], &displsvec[0], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // refreshing xold
    MPI_Bcast(&xold[0], n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Allreduce(&lnorm, &norm, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  } while (norm > precision);

  return xold;
}

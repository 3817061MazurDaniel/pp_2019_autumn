// Copyright 2019 Tihomirova Mariya
#include <mpi.h>
#include "../../../modules/task_1/tihomirova_m_trapezoid_integration/trapezoid_integration.h"

double Trapezoid_area(double(*f)(double), double x1, double x2, double h) {
  double S;
  S = ((f(x1) + f(x2)) * h) / 2.0;
  return S;
}

double  Parallelization(double(*f)(double), double a, double b, int n) {
  if (n <= 0)
    throw - 1;
  double Integral;
  double h;
  int k1;
  h = (b - a) / n;
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  k1 = n / size;
  double S = 0.0;
  double x1, x2;
  for (int i = rank * k1; i < k1 * (rank + 1); i++) {
    x1 = a + h * i;
    x2 = x1 + h;
    S += Trapezoid_area(f, x1, x2, h);
  }
  if (rank == 0) {
    for (int i = k1 * size; i < n; i++) {
      x1 = a + h * i;
      x2 = x1 + h;
      S += Trapezoid_area(f, x1, x2, h);
    }
  }
  MPI_Reduce(&S, &Integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  return Integral;
}

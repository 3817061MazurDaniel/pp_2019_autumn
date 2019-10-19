// Copyright 2019 Karin Timofey
#include <mpi.h>
#include <iostream>
#include <cmath>
#include <random>
#include "../../../modules/task_1/karin_rect_int/rec_int.h"

double GetRand() {
  return rand()%2000;
}

double QuadrFunInt(double a, double b, double c, double k1, double k2) {
  return (a * k2 * k2 * k2 / 3 + b * k2 * k2 / 2 + c * k2) - (a * k1 * k1 * k1 / 3 + b * k1 * k1 / 2 + c * k1);
}

double Integration(double(*f)(double), double a, double b, int k) {
  double res = 0;
  double h = (b - a) / k;
  for (double x = a+h/2; x < b + 1e-6; x += h) {
    res += f(x) * h;
  }
  return res;
}

double ParIntegration(double(*f)(double), double a, double b, int k) {
  if (k <= 0)
    throw -1;
  int rank;
  int size;

  double res,
    local_res = 0.0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  double x;
  double h = (b - a) / k;

  for (int i = rank + 1; i <= k; i += size) {
    x = a + (h * (static_cast<double>(i) - 0.5));
    local_res += f(x)*h;
  }

  MPI_Reduce(&local_res, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  return res;
}

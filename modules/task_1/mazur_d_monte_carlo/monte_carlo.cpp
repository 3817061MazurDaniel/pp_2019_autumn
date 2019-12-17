// Copyright 2019 Mazur Daniil
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <random>
#include <cmath>
#include "../../../modules/task_1/mazur_d_monte_carlo/monte_carlo.h"

double soloIntegral(int nPoints, double(*func)(double), double x1, double x2) {
  if (nPoints <= 0)
    throw - 1;
  double result = 0;


  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::uniform_real_distribution<double> x = std::uniform_real_distribution<double>(x1, x2);
  for (int i = 0; i < nPoints; ++i) {
    result += func(x(gen));
  }
  return (result * (x2 - x1) / nPoints);
}

double parIntegral(int nPoints, double(*func)(double), double x1, double x2) {
  if (nPoints <= 0)
    throw - 1;

  int procRank, procNum;
  double result = 0;
  double globalRes = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::uniform_real_distribution<double> x = std::uniform_real_distribution<double>(x1, x2);
  for (int i = procRank; i < nPoints; i += procNum) {
    result += func(x(gen));
  }
  MPI_Reduce(&result, &globalRes, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  return (globalRes * (x2 - x1) / nPoints);
}

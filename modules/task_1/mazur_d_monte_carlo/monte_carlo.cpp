  // Copyright 2019 Mazur Daniil
#include "../../../modules/task_1/mazur_d_monte_carlo/monte_carlo.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <random>
#include <cmath>

double getIntegral(int nPoints, double(*func)(double), double x1, double x2) {
  if (nPoints <= 0)
  throw - 1;

  int procRank, procNum;
  int uPoints = 0;
  int globalPoints;
  double step;
  double max = 0;
  double globalMax = 0;
  double xPoint, yPoint;
  double result;
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  step = (x2 - x1) / 10000;
  for (int i = procRank; i < nPoints; i += procNum) {
    if (max < func(x1 + step * i))
      max = func(x1 + step * i);
  }
  MPI_Reduce(&max, &globalMax, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Bcast(&globalMax, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  for (int i = procRank; i < nPoints; i += procNum) {
    std::random_device rd1;
    std::mt19937 mt1(rd1());
    std::uniform_real_distribution<double> dist1(x1, x2);
    xPoint = dist1(mt1);
    std::random_device rd2;
    std::mt19937 mt2(rd2());
    std::uniform_real_distribution<double> dist2(0, globalMax);
    yPoint = dist2(mt2);
    if (yPoint <= func(xPoint))
      ++uPoints;
  }

  MPI_Reduce(&uPoints, &globalPoints, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  result = (x2 - x1) * globalMax * globalPoints / nPoints;
  return result;
}

// Copyright 2019 Soboleva Julia
#include <mpi.h>
#include <math.h>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <ctime>
#include "../../../modules/task_3/soboleva_j_strongin_search/strongin_search.h"

double VectorPointX::countZ(double newx) {
  double res = 0;
  for (int i = n; i >= 0; i--) {
    res += g[n - i] * pow(newx, i);
  }
  return res;
}

std::vector<double> getRandVector(int _n) {
  if (_n <= 0)
    throw "Vector size error!!!";
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<double> mt(_n + 1);
  for (int i = 0; i < _n + 1; i++) {
    mt[i] = static_cast<double>(static_cast<int>(gen()) % 10000) / 100;
  }
  return mt;
}

VectorPointX::VectorPointX(double _a, double _b, int _n) {
  a = _a;
  b = _b;
  n = _n;
  g.resize(n + 1);
  g = getRandVector(n);

  count = 2;

  x.resize(count);
  x[0] = a;
  x[1] = b;

  z.resize(count);
  z[0] = countZ(a);
  z[1] = countZ(b);
}

VectorPointX::VectorPointX(double _a, double _b, int _n, std::vector<double> _g) {
  a = _a;
  b = _b;
  n = _n;
  g.resize(n + 1);
  for (int i = 0; i < n + 1; i++)
    g[i] = _g[i];

  count = 2;

  x.resize(count);
  x[0] = a;
  x[1] = b;

  z.resize(count);
  z[0] = countZ(a);
  z[1] = countZ(b);
}

void VectorPointX::addPoint(double newx) {
  if ((newx <= a) || (newx >= b))
    throw "Coordinate error!";

  int i = count - 1;
  count++;
  x.resize(count);
  z.resize(count);
  while (newx < x[i]) {
    i--;
  }
  for (int j = count - 1; j > i; j--) {
    x[j] = x[j - 1];
    z[j] = z[j - 1];
  }
  x[i + 1] = newx;
  z[i + 1] = countZ(newx);
}

double getM(VectorPointX  u, double r) {
  double max = (std::abs(u.z[1] - u.z[0]))/(u.x[1] - u.x[0]), res = 1.0;
  for (int i = 2; i < u.count; i++) {
    if ((u.z[i] - u.z[i - 1]) / (u.x[i] - u.x[i - 1]) > max)
      max = (std::abs(u.z[i] - u.z[i - 1])) /(u.x[i] - u.x[i - 1]);
  }
  if (max == 0)
    res = 1.0;
  else if (max > 0)
    res = r * max;
  return res;
}

std::vector<double> getR(VectorPointX  u, double r) {
  double m = getM(u, r);
  std::vector<double> res;
  for (int i = 1; i < u.count; i++) {
    double c = m * (u.x[i] - u.x[i - 1]) +
      (u.z[i] - u.z[i - 1]) * (u.z[i] - u.z[i - 1]) / (m * (u.x[i] - u.x[i - 1])) - 2 * (u.z[i] + u.z[i - 1]);
    res.push_back(c);
  }
  return res;
}

double stronginSearch(VectorPointX  u, double r, int e) {
  double eps = pow(10, -e);
  double delta = u.b - u.a;
  double p;
  int imax = 1;
  while (delta > eps) {
    std::vector<double> ch = getR(u, r);
    double max = ch[0];
    for (int i = 1; i < u.count; i++) {
      if (ch[i - 1] > max) {
        max = ch[i - 1];
        imax = i;
      }
    }
    double m = getM(u, r);
    delta = u.x[imax] - u.x[imax - 1];
    if (u.x[imax] - u.x[imax - 1] != 0) {
      p = (u.x[imax - 1] + u.x[imax]) / 2.0;
    } else {
      p = (u.x[imax - 1] + u.x[imax]) / 2.0 - (u.z[imax] - u.z[imax - 1]) / (2 * m);
    }
    u.addPoint(p);
  }
  return p;
}

double parStronginSearch(VectorPointX  u, double r, int e) {
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> ch1, ch2;
  double p, cur = u.a;
  double k[] = { u.a, u.b };

  if (rank != 0) {
    MPI_Status status;
    MPI_Recv(k, 2, MPI_DOUBLE, rank - 1, rank, MPI_COMM_WORLD, &status);
  }

  if (rank != size - 1) {
    p = (k[0] + k[1]) / 2.0;
    VectorPointX f(k[0], p, u.n, u.g), s(p, k[1], u.n, u.g);
    ch1 = getR(f, r);
    ch2 = getR(s, r);
    if (ch1 > ch2) {
      k[1] = p;
      cur = stronginSearch(s, r, e);
    } else {
      k[0] = p;
      cur = stronginSearch(f, r, e);
    }
    MPI_Send(k, 2, MPI_DOUBLE, rank + 1, rank + 1, MPI_COMM_WORLD);
  }
  if (rank == size - 1) {
    VectorPointX f(k[0], k[1], u.n, u.g);
    cur = stronginSearch(f, r, e);
  }

  if (rank != 0) {
    MPI_Send(&cur, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }
  if (rank == 0) {
    MPI_Status status;
    double x;
    p = cur;
    double min = u.countZ(cur);
    for (int i = 1; i < size; i++) {
      MPI_Recv(&x, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
      if (u.countZ(x) < min) {
        min = u.countZ(x);
        p = x;
      }
    }
  }
  MPI_Bcast(&p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  return p;
}

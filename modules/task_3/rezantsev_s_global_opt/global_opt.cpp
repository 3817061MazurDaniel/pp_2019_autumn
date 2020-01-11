// Copyright 2019 Rezantsev Sergey

#include "../../../modules/task_3/rezantsev_s_global_opt/global_opt.h"

Point::Point(double _x, double _y) : x(_x), y(_y) {}

OneDimensionalOpt::OneDimensionalOpt(double _a, double _b, double _currX,
                                     double (*f)(double _x, double _y),
                                     double _eps, int _Nmax, double _r_p)
    : a(_a),
      b(_b),
      currX(_currX),
      function(f),
      eps(_eps),
      Nmax(_Nmax),
      r_p(_r_p) {}

double OneDimensionalOpt::calculateMmax() {
  return std::abs((rPoint->y - lPoint->y) / (rPoint->x - lPoint->x));
}

double OneDimensionalOpt::calculateR() {
  return m * (rPoint->x - lPoint->x) +
         (std::pow(rPoint->y - lPoint->y, 2) / (m * (rPoint->x - lPoint->x))) -
         2 * (rPoint->y - lPoint->y);
}

void OneDimensionalOpt::OrdinaryPrepearing() {
  res.x = currX;
  res.y = a;
  res.z = function(currX, a);
  set.insert(Point(a, res.z));
  double tmp = function(currX, b);
  set.insert(Point(b, tmp));
  if (res.z > tmp) {
    res.y = b;
    res.z = tmp;
  }
  lPoint = set.begin();
  rPoint = set.begin();
  rPoint++;
  Rmax = calculateR();
  smallestInterval = std::abs(a - b);
}

void OneDimensionalOpt::findMmax() {
  Mmax = -1;
  lPoint = set.begin();
  rPoint = set.begin();
  rPoint++;
  while (rPoint != set.end()) {
    double M = calculateMmax();
    if (M > Mmax) {
      Mmax = M;
    }
    rPoint++;
    lPoint++;
  }
}

void OneDimensionalOpt::calculatem() {
  if (Mmax > 0) {
    m = Mmax * r_p;
  } else if (Mmax == 0) {
    m = 1.0;
  }
}

void OneDimensionalOpt::newFunction(double _a, double _b, double _currx,
                                    double (*f)(double _x, double _y)) {
  a = _a;
  b = _b;
  currX = _currx;
  function = f;
}

void OneDimensionalOpt::findRmax() {
  Rmax = -200000000;
  lPoint = set.begin();
  rPoint = set.begin();
  rPointRmax = set.begin();
  lPointRmax = set.begin();
  rPoint++;
  while (rPoint != set.end()) {
    double R = calculateR();
    double s = rPoint->x - lPoint->x;
    if (R > Rmax) {
      Rmax = R;
      rPointRmax = rPoint;
      lPointRmax = lPoint;
    }
    if (s < smallestInterval) {
      smallestInterval = s;
    }
    rPoint++;
    lPoint++;
  }
}

double OneDimensionalOpt::calculateNewPoint() {
  return (0.5) * (rPointRmax->x + lPointRmax->x) -
         ((rPointRmax->y - lPointRmax->y) / (2.0 * m));
}

void OneDimensionalOpt::calculate() {
  OrdinaryPrepearing();
  int n = 2;
  while (smallestInterval > eps) {
    findMmax();
    calculatem();
    findRmax();
    n++;
    double newX = calculateNewPoint();
    double tmp = function(currX, newX);
    set.insert(Point(newX, tmp));
    if (res.z > tmp) {
      res.y = newX;
      res.z = tmp;
    }
  }
  set.clear();
}

double GlobalOptimazer::calculateR() {
  return m * (rP->x - lP->x) +
         (std::pow((rP->z - lP->z), 2) / (m * (rP->x - lP->x))) -
         2 * (rP->z - lP->z);
}

double GlobalOptimazer::calculateMmax() {
  return std::abs((rP->z - lP->z) / (rP->x - lP->x));
}

void GlobalOptimazer::calculatem() {
  if (Mmax > 0) {
    m = r_p * Mmax;
  } else {
    m = 1;
  }
}

void GlobalOptimazer::findMmax() {
  Mmax = -1;
  rP = set.begin();
  rP++;
  lP = set.begin();
  while (rP != set.end()) {
    double currM = calculateMmax();
    if (currM > Mmax) Mmax = currM;
    rP++;
    lP++;
  }
}

void GlobalOptimazer::setRPoints() {
  rP = set.begin();
  rP++;
  lP = set.begin();
  while (rP != set.end()) {
    double R = calculateR();
    setR.insert(RPoint(R, rP->x, rP->z, lP->x, lP->z));
    rP++;
    lP++;
  }
}

double GlobalOptimazer::calculateNewPoint() {
  return (0.5) * (RP->x + RP->prevX) - ((RP->z - RP->prevZ) / (2 * m));
}

void GlobalOptimazer::OrdinaryCalculation() {
  res = {0, 0, 0};
  result ress;
  OneDimensionalOpt opt(c, d, a, function, eps, Nmax);
  opt.calculate();
  ress = opt.res;
  set.insert(GlobalPoint(ress.x, ress.y, ress.z));
  res = ress;
  opt.newFunction(c, d, b, function);
  opt.calculate();
  ress = opt.res;
  set.insert(GlobalPoint(ress.x, ress.y, ress.z));
  if (ress.z < res.z) res = ress;
  do {
    setR.clear();
    findMmax();
    calculatem();
    setRPoints();
    RP = setR.begin();
    double newX = calculateNewPoint();
    opt.newFunction(c, d, newX, function);
    opt.calculate();
    ress = opt.res;
    set.insert(GlobalPoint(ress.x, ress.y, ress.z));
    if (ress.z < res.z) res = ress;
  } while (RP->x - RP->prevX >= eps);
}

void GlobalOptimazer::globalCalculation() {
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size < 2) {
    OrdinaryCalculation();
  } else {
    MPI_Status status;
    res = {0, 0, 0};
    result ress;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
      int k;
      if (std::abs(a - b) <= 0.0001 || std::abs(c - d) <= 0.0001) {
        k = size + 1;
      } else {
        k = size;
      }
      double segmentLen = (b - a) / (k - 1);
      for (int i = 0; i < size - 1; ++i) {
        double Xf = a + i * segmentLen;
        MPI_Send(&Xf, 1, MPI_DOUBLE, i + 1, 1, MPI_COMM_WORLD);
      }
      OneDimensionalOpt tmp(c, d, b, function, eps);
      tmp.calculate();
      ress = tmp.res;
      set.insert(GlobalPoint(ress.x, ress.y, ress.z));
      res = ress;
      if (k != size) {
        tmp.newFunction(c, d, a + segmentLen * size, function);
        tmp.calculate();
        ress = tmp.res;
        set.insert(GlobalPoint(ress.x, ress.y, ress.z));
        if (ress.z < res.z) res = ress;
      }
      for (int i = 0; i < size - 1; ++i) {
        MPI_Recv(&ress, 3, MPI_DOUBLE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD,
                 &status);
        if (ress.z < res.z) res = ress;
        set.insert(GlobalPoint(ress.x, ress.y, ress.z));
      }
      bool f = true;
      while (f) {
        setR.clear();
        findMmax();
        calculatem();
        setRPoints();
        RP = setR.begin();
        for (int i = 0; i < size - 1; ++i) {
          double newX = calculateNewPoint();
          MPI_Send(&newX, 1, MPI_DOUBLE, i + 1, 1, MPI_COMM_WORLD);
          if (RP->x - RP->prevX <= eps) {
            f = false;
          }
          RP++;
        }
        for (int i = 0; i < size - 1; ++i) {
          MPI_Recv(&ress, 3, MPI_DOUBLE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD,
                   &status);
          if (ress.z < res.z) res = ress;
          set.insert(GlobalPoint(ress.x, ress.y, ress.z));
        }
      }
      for (int i = 0; i < size - 1; ++i) {
        double v = eps * 0.001;
        MPI_Send(&v, 1, MPI_DOUBLE, i + 1, 1, MPI_COMM_WORLD);
      }
    } else {
      double npoint = -1;
      do {
        MPI_Recv(&npoint, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
        if (npoint == eps * 0.001) {
          break;
        }
        OneDimensionalOpt tmp(c, d, npoint, function, eps, Nmax);
        tmp.calculate();
        ress = tmp.res;
        MPI_Send(&ress, 3, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
      } while (true);
    }
  }
}

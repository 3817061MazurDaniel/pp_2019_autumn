// Copyright 2019 Golubev Vladislav
#include <mpi.h>
#include <utility>
#include <list>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>
#include "../../../modules/task_3/golubev_v_global_optimization_search_area/global_optimization_search_area.h"


Optimization::Optimization(int _dim, double _a1, double _b1,
  std::function<double(double*)> _pFunc, double _eps,
  double _a2, double _b2) {
  dim = _dim;
  a1 = _a1;
  b1 = _b1;
  pFunc = _pFunc;
  eps = _eps;
  a2 = _a2;
  b2 = _b2;
}

double ABS(double x) {
  return (x >= 0) ? x : -x;
}

double Optimization::getM_XY(int idx, const std::vector<double>& vec) {
  return ABS(getFuncValue_XY(vec[idx]) - getFuncValue_XY(vec[idx - 1])) / (vec[idx] - vec[idx - 1]);
}

double Optimization::get_m_XY(double M, double r) {
  return (M == 0) ? 1 : r * M;
}

double Optimization::getR_XY(int idx, double m, const std::vector<double>& vec) {
  double funcValues = getFuncValue_XY(vec[idx]) - getFuncValue_XY(vec[idx - 1]);
  double koef = m * (vec[idx] - vec[idx - 1]);
  return koef + (funcValues * funcValues) / koef - 2 *
    (getFuncValue_XY(vec[idx]) + getFuncValue_XY(vec[idx - 1]));
}

double Optimization::getNewX_XY(int t, double m, const std::vector<double>& vec) {
  double funcValues = getFuncValue_XY(vec[t]) - getFuncValue_XY(vec[t - 1]);
  return (vec[t] + vec[t - 1]) / 2 - funcValues / (2 * m);
}

double Optimization::getFuncValue_XY(double x) {
  double* arg = &x;
  return pFunc(arg);
}

double Optimization::SequentialGlobalSearchXY(size_t maxIteration) {
  std::vector<double> x(maxIteration + 1, -1e+300);
  size_t nIteration = 1;
  int t = 1;
  double r = 2;
  double R, tmp;

  x[0] = a1;
  x[1] = b1;
  double M = getM_XY(1, x);
  double m = get_m_XY(M, r);
  x[2] = getNewX_XY(1, m, x);
  ++nIteration;

  while (nIteration < maxIteration) {
    sort(x.begin(), x.begin() + nIteration + 1);

    M = getM_XY(1, x);
    for (size_t i = 2; i <= nIteration; ++i) {
      M = std::max(M, getM_XY(i, x));
    }
    m = get_m_XY(M, r);
    R = getR_XY(1, m, x);
    t = 1;

    for (size_t i = 2; i <= nIteration; ++i) {
      tmp = getR_XY(i, m, x);
      if (R < tmp) {
        R = tmp;
        t = i;
      }
    }
    x[nIteration + 1] = getNewX_XY(t, m, x);
    ++nIteration;
    if (x[t] - x[t - 1] < eps) {
      break;
    }
  }
  return x[t];
}

double Optimization::ParallelGlobalSearchXY(size_t maxIteration) {
  int procNum;
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  if (procNum == 1) {
    return SequentialGlobalSearchXY(maxIteration);
  }
  int procRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  std::vector<double> result(procNum);

  double h = (b1 - a1) / procNum;
  double a = a1 + h * procRank;
  double b = a + h;
  double tmp = 0;

  Optimization localOpt(1, a, b, pFunc, eps);
  double localRes = localOpt.SequentialGlobalSearchXY(maxIteration);
  MPI_Gather(&localRes, 1, MPI_DOUBLE, &result[0], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  if (procRank == 0) {
    double rootFValue = getFuncValue_XY(result[0]);
    for (int i = 1; i < procNum; ++i) {
      tmp = getFuncValue_XY(result[i]);
      if (tmp < rootFValue) {
        std::swap(result[i], result[0]);
        rootFValue = tmp;
      }
    }
  }
  return result[0];
}



int n1, nexp, l, iq, iu[10], iv[10];
void Optimization::node(int is) {
  int n, i, j, k1, k2, iff;
  n = n1 + 1;
  if (is == 0) {
    l = n1;
    for (i = 0; i < n; i++) { iu[i] = -1; iv[i] = -1; }
  } else if (is == (nexp - 1)) {
    l = n1;
    iu[0] = 1;
    iv[0] = 1;
    for (i = 1; i < n; i++) { iu[i] = -1; iv[i] = -1; }
    iv[n1] = 1;
  } else {
    iff = nexp;
    k1 = -1;
    for (i = 0; i < n; i++) {
      iff = iff / 2;
      if (is >= iff) {
        if ((is == iff) && (is != 1)) { l = i; iq = -1; }
        is = is - iff;
        k2 = 1;
      } else {
        k2 = -1;
        if ((is == (iff - 1)) && (is != 0)) { l = i; iq = 1; }
      }
      j = -k1 * k2;
      iv[i] = j;
      iu[i] = j;
      k1 = k2;
    }
    iv[l] = iv[l] * iq;
    iv[n1] = -iv[n1];
  }
}

void Optimization::mapd(double x, double* y, int m, int n) {
  double f, mne, dr;
  float p, r;
  int iw[11];
  int it, is, i, j/*, k*/;
  p = 0.0;
  n1 = n - 1;
  for (nexp = 1, i = 0; i < n; nexp *= 2, i++) {
  }
  f = x; r = 0.5; it = 0; dr = nexp;
  for (mne = 1, i = 0; i < m; mne *= dr, i++) {
  }
  for (i = 0; i < n; i++) { iw[i] = 1; y[i] = 0.0; }
  for (j = 0; j < m; j++) {
    iq = 0;
    if (x == 1.0) {
      is = nexp - 1; f = 0.0;
    } else {
      f = f * nexp;
      is = f;
      f = f - is;
    }
    i = is;
    node(i);
    i = iu[0];
    iu[0] = iu[it];
    iu[it] = i;
    i = iv[0];
    iv[0] = iv[it];
    iv[it] = i;
    if (l == 0) l = it;
    else if (l == it) l = 0;
    /*if ((iq > 0) || ((iq == 0) && (is == 0))) k = l;
    else if (iq < 0) k = (it == n1) ? 0 : n1;*/
    r = r * 0.5;
    it = l;
    for (i = 0; i < n; i++) {
      iu[i] = iu[i] * iw[i];
      iw[i] = -iv[i] * iw[i];
      p = r * iu[i];
      p = p + y[i];
      y[i] = p;
    }
  }
}


double Optimization::getM_XYZ(int idx, const std::vector<double>& vec) {
  return ABS(getFuncValue_XYZ(vec[idx]) - getFuncValue_XYZ(vec[idx - 1])) / (vec[idx] - vec[idx - 1]);
}

double Optimization::get_m_XYZ(double M, double r) {
  return (M == 0) ? 1 : r * M;
}

double Optimization::getR_XYZ(int idx, double m, const std::vector<double>& vec) {
  double funcValues = getFuncValue_XYZ(vec[idx]) - getFuncValue_XYZ(vec[idx - 1]);
  double koef = m * (vec[idx] - vec[idx - 1]);
  return koef + (funcValues * funcValues) / koef - 2 *
    (getFuncValue_XYZ(vec[idx]) + getFuncValue_XYZ(vec[idx - 1]));
}

double Optimization::getNewX_XYZ(int t, double m, const std::vector<double>& vec, double r) {
  double funcValues = getFuncValue_XYZ(vec[t]) - getFuncValue_XYZ(vec[t - 1]);
  return (vec[t] + vec[t - 1]) / 2 - funcValues / (2 * m);
}

double Optimization::getFuncValue_XYZ(double x) {
  double* arg = new double[2];
  mapd(x, arg, 20, 2);
  double ro = std::max((b1 - a1), (b2 - a2));
  arg[0] = arg[0] * ro + (a1 + b1) / 2;
  arg[1] = arg[1] * ro + (a2 + b2) / 2;
  return pFunc(arg);
}

double* Optimization::SequentialGlobalSearchXYZ(size_t maxIteration) {
  std::vector<double> x(maxIteration + 1, -1e+300);
  size_t nIteration = 1;
  int t = 1;
  double r = 2;
  double R, tmp;

  x[0] = 0;
  x[1] = 1;
  double M = getM_XYZ(1, x);

  double m = get_m_XYZ(M, r);

  x[2] = getNewX_XYZ(1, m, x, r);

  ++nIteration;

  while (nIteration < maxIteration) {
    sort(x.begin(), x.begin() + nIteration + 1);

    M = getM_XYZ(1, x);

    for (size_t i = 2; i <= nIteration; ++i) {
      M = std::max(M, getM_XYZ(i, x));
    }
    m = get_m_XYZ(M, r);

    R = getR_XYZ(1, m, x);
    t = 1;

    for (size_t i = 2; i <= nIteration; ++i) {
      tmp = getR_XYZ(i, m, x);
      if (R < tmp) {
        R = tmp;
        t = i;
      }
    }
    x[nIteration + 1] = getNewX_XYZ(t, m, x, r);
    ++nIteration;
    if (x[t] - x[t - 1] < eps) {
      break;
    }
  }
  double* result = new double[2];
  mapd(x[t], result, 20, 2);
  return result;
}

double* Optimization::ParallelGlobalSearchXYZ(size_t maxIteration) {
  int procNum, procRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  if (procNum == 1) {
    return SequentialGlobalSearchXYZ(maxIteration);
  }
  double h = (b1 - a1) / procNum;
  double new_a1 = a1 + h * procRank;
  double new_b1 = new_a1 + h;

  double tmp = 0;

  std::vector<double> result(2 * procNum);
  Optimization localOpt(2, new_a1, new_b1, pFunc, eps, a2, b2);
  double* localRes = new double[2];
  localRes = localOpt.SequentialGlobalSearchXYZ(maxIteration);
  MPI_Gather(&localRes[0], 2, MPI_DOUBLE, &result[0], 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  double* res = new double[2];
  if (procRank == 0) {
    double* arg = new double[2];

    arg[0] = res[0] = result[0];
    arg[1] = res[1] = result[1];

    double rootFValue = pFunc(arg);
    for (int i = 2; i < 2 * procNum; i += 2) {
      arg[0] = result[i];
      arg[1] = result[i + 1];
      tmp = pFunc(arg);
      if (tmp < rootFValue) {
        res[0] = arg[0];
        res[1] = arg[1];
        rootFValue = tmp;
      }
    }
  }
  return res;
}

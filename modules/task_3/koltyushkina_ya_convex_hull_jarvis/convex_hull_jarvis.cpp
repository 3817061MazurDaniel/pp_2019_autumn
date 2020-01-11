// Copyright 2019 Koltyushkina Yanina

#include <mpi.h>
#include <iostream>
#include <random>
#include <cmath>
#include <stdexcept>
#include "../../../modules/task_3/koltyushkina_ya_convex_hull_jarvis/convex_hull_jarvis.h"

double** RandomHull(int size) {
  if (size <= 0)
    throw "Negativ size";
  double** arr = new double*[size];
  for (int i = 0; i < size; i++)
    arr[i] = new double[2];
  std::mt19937 seed;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < 2; j++) {
      arr[i][j] = seed() % 100;
    }
  }
  return arr;
}

bool PointMore(int ind, double* mid, double* fi, double* r) {
  if (fi[ind] > mid[0]) {
    return true;
  } else if (fi[ind] == mid[0]) {
    if (r[ind] > mid[1])
      return true;
  }
  return false;
}

bool PointLess(int ind, double* mid, double* fi, double* r) {
  if (fi[ind] < mid[0]) {
    return true;
  } else if (fi[ind] == mid[0]) {
    if (r[ind] < mid[1])
      return true;
  }
  return false;
}

double** QuickSort(double** arr, int first, int last, double* fi, double* r) {
  double* mid = new double[2];
  double temp;
  int f = first, l = last;
  mid[0] = fi[(f + l) / 2];
  mid[1] = r[(f + l) / 2];
  do {
    while (PointLess(f, mid, fi, r))
      f++;
    while (PointMore(l, mid, fi, r))
      l--;

    if (f <= l) {
      temp = r[f];
      r[f] = r[l];
      r[l] = temp;
      temp = fi[f];
      fi[f] = fi[l];
      fi[l] = temp;

      temp = arr[f][0];
      arr[f][0] = arr[l][0];
      arr[l][0] = temp;
      temp = arr[f][1];
      arr[f][1] = arr[l][1];
      arr[l][1] = temp;

      f++;
      l--;
    }
  } while (f < l);
  if (first < l)
    QuickSort(arr, first, l, fi, r);
  if (f < last)
    QuickSort(arr, f, last, fi, r);
  return arr;
}

double cosvec(double* p1, double* p2, double* p3) {
  double ax = p2[0] - p1[0];
  double ay = p2[1] - p1[1];
  double bx = p3[0] - p1[0];
  double by = p3[1] - p1[1];
  return ((ax*bx + ay * by) / (sqrt(ax*ax + ay * ay) * sqrt(bx*bx + by * by)));
}

double** ConvexHull(double** arr, const int nump) {
  double** nowres = new double*[nump + 1];
  double** result = new double*[nump + 1];
  for (int i = 0; i < nump + 1; i++) {
    nowres[i] = new double[2];
    result[i] = new double[2];
  }
  nowres[0][0] = 0;
  if (nump == 1) {
    result[0][0] = 1;
    result[1] = arr[0];
    return result;
  } else if (nump == 2) {
    result[0][0] = 2;
    result[1] = arr[0];
    result[2] = arr[1];
  } else {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int delta;
    int ost;
    int f = 0;
    ost = nump % size;
    if ((nump < size) || (nump / size <= 2)) {
      ost = 0;
      f = -1;
      size = 1;
    }
    delta = nump / size;

    double* OO = new double[2];
    OO[0] = arr[0][0];
    OO[1] = arr[0][1];
    int m = 0;
    for (int i = 1; i < nump; i++) {
      if (arr[i][0] < OO[0]) {
        OO[0] = arr[i][0];
        OO[1] = arr[i][1];
        m = i;
      } else {
        if (arr[i][0] == OO[0]) {
          if (arr[i][1] < OO[1]) {
            OO[0] = arr[i][0];
            OO[1] = arr[i][1];
            m = i;
          }
        }
      }
    }

    double* r = new double[nump];
    double* fi = new double[nump];
    double* tmp = arr[0];
    arr[0] = arr[m];
    arr[m] = tmp;
    m = 0;
    for (int i = 1; i < nump; i++)
      for (int j = 0; j < 2; j++)
        arr[i][j] = arr[i][j] - OO[j];

    if (rank == 0) {
      for (int i = 1; i < delta + ost; i++) {
        r[i] = pow((arr[i][0] * arr[i][0]) + (arr[i][1] * arr[i][1]), 0.5);
        fi[i] = atan(arr[i][1] / arr[i][0]);
      }
    } else {
      if (f == 0) {
        for (int i = delta * rank + ost; i < delta*rank + delta + ost; i++) {
          r[i] = pow((arr[i][0] * arr[i][0]) + (arr[i][1] * arr[i][1]), 0.5);
          fi[i] = atan(arr[i][1] / arr[i][0]);
        }
      }
    }
    if (size > 1) {
      MPI_Bcast(&r[1], delta + ost - 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(&fi[1], delta + ost - 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    for (int i = 1; i < size; i++) {
      MPI_Bcast(&fi[delta * i + ost], delta, MPI_DOUBLE, i, MPI_COMM_WORLD);
      MPI_Bcast(&r[delta*i + ost], delta, MPI_DOUBLE, i, MPI_COMM_WORLD);
    }
    arr[0][0] = 0;
    arr[0][1] = 0;

    int lockind;

    MPI_Barrier(MPI_COMM_WORLD);
    QuickSort(arr, 1, nump - 1, fi, r);
    int ind;
    if (rank == 0) {
      int smind = 1;
      if (fi[1] == fi[nump - 1]) {
        smind = nump - 1;
      } else if (fi[1] == fi[2]) {
        smind = 2;
        while (fi[smind] == fi[smind + 1]) {
          smind += 1;
        }
      }
      ind = smind;
    }

    MPI_Bcast(&ind, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (ind == nump - 1) {
      result[0][0] = 2;
      result[1][0] = arr[0][0] + OO[0];
      result[1][1] = arr[0][1] + OO[1];
      result[2][0] = arr[nump - 1][0] + OO[0];
      result[2][1] = arr[nump - 1][1] + OO[1];
      return result;
    }
    int pcount = nump;
    int minind = ind;
    if (rank == 0) {
      int smind = nump - 1;
      int endind = nump;
      while (fi[smind] == fi[smind - 1]) {
        smind--;
      }
      if (smind != nump - 1) {
        endind = smind + 1;
      }
      pcount = endind;
    }
    MPI_Bcast(&pcount, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (f == -1) {
      ost = 0;
      size = 1;
      arr[pcount - 1][0] = arr[nump - 1][0];
      arr[pcount - 1][1] = arr[nump - 1][1];
      nowres[1] = arr[0];
      nowres[2] = arr[ind];
      nowres[0][0] = 2;
      delta = (pcount - ind - 1);
      int sizeres = 2;
      int flag = 0;
      while (flag != 1) {
        double* last = new double[2];
        double* beforelast = new double[2];
        last = nowres[sizeres];
        beforelast = nowres[sizeres - 1];
        int minind = 0;
        double mincos = cosvec(last, beforelast, arr[0]);
        double cos0 = mincos;
        double nowcos;

        for (int j = ind + 1; j < pcount; j++) {
          nowcos = cosvec(last, beforelast, arr[j]);
          if ((nowcos <= mincos) && (nowcos != cos0)) {
            mincos = nowcos;
            minind = j;
          }
        }
        if (minind == 0) {
          flag = 1;
          break;
        } else {
          sizeres++;
          nowres[sizeres] = arr[minind];
          delta = (pcount - minind - 1) / size;
          ost = (pcount - minind - 1) % size;
          ind = minind;
        }
      }
      result[0][0] = sizeres;
      for (int i = 0; i < sizeres; i++) {
        result[i + 1][0] = nowres[i + 1][0] + OO[0];
        result[i + 1][1] = nowres[i + 1][1] + OO[1];
      }
      return result;
    }
    arr[pcount - 1][0] = arr[nump - 1][0];
    arr[pcount - 1][1] = arr[nump - 1][1];

    nowres[1] = (arr[0]);
    nowres[2] = (arr[ind]);
    int sizeres = 2;

    delta = (pcount - ind - 1) / size;
    ost = (pcount - ind - 1) % size;
    double* arrmincos = new double[size];
    int* arrminind = new int[size];

    double* last = new double[2];
    double* beforelast = new double[2];

    while (delta > 1) {
      last = nowres[sizeres];
      beforelast = nowres[sizeres - 1];
      minind = 0;
      double mincos = cosvec(last, beforelast, arr[0]);
      double cos0 = mincos;
      double nowcos;
      if (rank == 0) {
        int endlockind;
        if (ind + delta + ost + 1 > nump) {
          endlockind = nump;
        } else {
          endlockind = ind + delta + ost + 1;
          for (int j = ind + 1; j < endlockind; j++) {
            nowcos = cosvec(last, beforelast, arr[j]);
            if ((nowcos <= mincos) && (nowcos != cos0)) {
              mincos = nowcos;
              minind = j;
            }
          }
        }
      } else {
        if (f == 0) {
          lockind = ind + delta * rank + ost + 1;
          for (int j = lockind; j < lockind + delta; j++) {
            if (j == lockind) {
              mincos = cosvec(last, beforelast, arr[j]);
            } else {
              nowcos = cosvec(last, beforelast, arr[j]);
              if ((nowcos <= mincos) && (nowcos != cos0)) {
                mincos = nowcos;
                minind = j;
              }
            }
          }
        }
      }

      MPI_Gather(&minind, 1, MPI_INT, arrminind, 1, MPI_INT, 0, MPI_COMM_WORLD);
      MPI_Gather(&mincos, 1, MPI_DOUBLE, arrmincos, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

      if (rank == 0) {
        minind = arrminind[0];
        mincos = arrmincos[0];
        for (int i = 0; i < size; i++) {
          if ((arrmincos[i] <= mincos) && (arrmincos[i] != cos0)) {
            mincos = arrmincos[i];
            minind = arrminind[i];
          }
        }
      }

      MPI_Bcast(&minind, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

      ind = minind;
      sizeres++;
      nowres[sizeres] = arr[minind];
      delta = (pcount - minind - 1) / size;
      ost = (pcount - minind - 1) % size;
    }
    if (rank == 0) {
      int flag = 0;
      while (flag != 1) {
        double* last = new double[2];
        double* beforelast = new double[2];
        last = nowres[sizeres];
        beforelast = nowres[sizeres - 1];
        minind = 0;
        double mincos = cosvec(last, beforelast, arr[0]);
        double cos0 = mincos;
        double nowcos;

        for (int j = ind + 1; j < pcount; j++) {
          nowcos = cosvec(last, beforelast, arr[j]);
          if ((nowcos <= mincos) && (nowcos != cos0)) {
            mincos = nowcos;
            minind = j;
          }
        }
        if (minind == 0) {
          flag = 1;
          break;
        } else {
          sizeres++;
          nowres[sizeres] = arr[minind];
          delta = (pcount - minind - 1) / size;
          ost = (pcount - minind - 1) % size;
          ind = minind;
        }
      }
      result[0][0] = sizeres;
      for (int i = 0; i < sizeres; i++) {
        result[i + 1][0] = nowres[i + 1][0] + OO[0];
        result[i + 1][1] = nowres[i + 1][1] + OO[1];
      }
    }
  }
  return result;
}

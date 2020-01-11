// Copyright 2019 Silenko Dmitrii

#include <mpi.h>
#include <iostream>
#include <random>
#include <cmath>
#include <stack>
#include <stdexcept>
#include "../../../modules/task_3/silenko_d_Convex_Hull_Graham/Convex_Hull_Graham.h"

double** getRandomMas(int count) {
  if (count <= 0)
    throw "Wrong count";
  double** mas = new double*[count];
  for (int i = 0; i < count; i++)
    mas[i] = new double[2];
  std::mt19937 seed;
  for (int i = 0; i < count; i++) {
    for (int j = 0; j < 2; j++) {
      mas[i][j] = seed() % 100;
    }
  }
  return mas;
}

bool SravnenieMore(int num, double* _mid, double* fi, double* r) {
  if (fi[num] > _mid[0]) {
    return true;
  } else if (fi[num] == _mid[0]) {
    if (r[num] > _mid[1])
      return true;
  }
  return false;
}

bool SravnenieLess(int num, double* _mid, double* fi, double* r) {
  if (fi[num] < _mid[0]) {
    return true;
  } else if (fi[num] == _mid[0]) {
    if (r[num] < _mid[1])
      return true;
  }
  return false;
}

double** QuickSort(double** mas, int first, int last, double* fi, double* r) {
  double* mid = new double[2];
  double tmp;
  int f = first, l = last;
  mid[0] = fi[(f + l) / 2];
  mid[1] = r[(f + l) / 2];
  do {
    while (SravnenieLess(f, mid, fi, r))
      f++;
    while (SravnenieMore(l, mid, fi, r))
      l--;

    if (f <= l) {
      tmp = r[f];
      r[f] = r[l];
      r[l] = tmp;
      tmp = fi[f];
      fi[f] = fi[l];
      fi[l] = tmp;

      tmp = mas[f][0];
      mas[f][0] = mas[l][0];
      mas[l][0] = tmp;
      tmp = mas[f][1];
      mas[f][1] = mas[l][1];
      mas[l][1] = tmp;

      f++;
      l--;
    }
  } while (f < l);
  if (first < l)
    QuickSort(mas, first, l, fi, r);
  if (f < last)
    QuickSort(mas, f, last, fi, r);
  return mas;
}

double det(double* p1, double* p2, double* p3, int size) {
  double diag1 = (p1[0] - p2[0]) * (p3[1] - p1[1]);
  double diag2 = (p3[0] - p1[0]) * (p1[1] - p2[1]);
  return (diag1 - diag2);
}

std::stack<double*> ConvSort(double** mas, const int count) {
  std::stack<double*> res;
  std::stack<double*> prom_res;
  if (count == 1) {
    res.push(mas[0]);
  } else if (count == 2) {
    res.push(mas[0]);
    res.push(mas[1]);
  } else {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    int delta;
    int ost;
    int f = 0;
    ost = count % size;

    if ((count < size) || (count / size < 3)) {
      ost = 0;
      f = -1;
      size = 1;
    }

    delta = count / size;

    double* c = new double[2];
    c[0] = mas[0][0];
    c[1] = mas[0][1];
    int m = 0;

    for (int i = 1; i < count; i++) {
      if (mas[i][0] < c[0]) {
        c[0] = mas[i][0];
        c[1] = mas[i][1];
        m = i;
      } else {
        if (mas[i][0] == c[0]) {
          if (mas[i][1] < c[1]) {
            c[0] = mas[i][0];
            c[1] = mas[i][1];
            m = i;
          }
        }
      }
    }

    double* r = new double[count];
    double* fi = new double[count];
    double* tmp = mas[0];
    mas[0] = mas[m];
    mas[m] = tmp;
    m = 0;

    for (int i = 1; i < count; i++)
      for (int j = 0; j < 2; j++)
        mas[i][j] = mas[i][j] - c[j];

    if (rank == 0) {
      for (int i = 1; i < delta + ost; i++) {
        r[i] = pow((mas[i][0] * mas[i][0]) + (mas[i][1] * mas[i][1]), 0.5);
        fi[i] = atan(mas[i][1] / mas[i][0]);
      }
    } else {
      if (f == 0) {
        for (int i = delta*rank+ost; i < delta*rank + delta + ost; i++) {
          r[i] = pow((mas[i][0] * mas[i][0]) + (mas[i][1] * mas[i][1]), 0.5);
          fi[i] = atan(mas[i][1] / mas[i][0]);
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

    mas[0][0] = 0;
    mas[0][1] = 0;
    MPI_Barrier(MPI_COMM_WORLD);

    QuickSort(mas, 1, count - 1, fi, r);

    if (rank == 0) {
      prom_res.push(mas[0]);
      prom_res.push(mas[1]);
      for (int i = 2; i < delta + ost; i++) {
        double* last;
        double* beforelast;
        while (1) {
          last = prom_res.top();
          prom_res.pop();
          beforelast = prom_res.top();
          prom_res.push(last);

          double d = det(last, beforelast, mas[i], size);
          if (d > 0) {
            prom_res.push(mas[i]);
            break;
          } else {
            if (prom_res.size() >= 3) {
              prom_res.pop();
            } else {
              prom_res.pop();
              prom_res.push(mas[i]);
              break;
            }
          }
        }
      }
      if (f == 0) {
        int s;
        for (int i = 1; i < size; i++) {
          MPI_Recv(&s, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
          double* buf = new double[2 * s];
          MPI_Recv(&buf[0], 2 * s, MPI_DOUBLE, i, 9, MPI_COMM_WORLD, &status);
          double* last;
          double* beforelast;
          int k = 1;
          int flag = 0;
          for (int j = 0; j < s; j++) {
            while (1) {
              last = prom_res.top();
              prom_res.pop();
              beforelast = prom_res.top();
              prom_res.push(last);
              double* tt = new double[2];
              tt[0] = buf[2 * s - (k + 1)];
              tt[1] = buf[2 * s - k];
              double d = det(last, beforelast, tt, size);
              if (d > 0) {
                flag++;
                prom_res.push(tt);
                break;
              } else {
                if (prom_res.size() >= 3) {
                  if (flag > 0)
                    flag--;
                  prom_res.pop();
                } else {
                  flag++;
                  prom_res.pop();
                  prom_res.push(tt);
                  break;
                }
              }
              delete[] tt;
            }
            k += 2;
            if (flag >= 2)
              break;
          }
          for (int j = 2 * s - k; j > 0; j -= 2) {
            double* tt = new double[2];
            tt[0] = buf[j - 1];
            tt[1] = buf[j];
            prom_res.push(tt);
          }
        }
      }

      int s = prom_res.size();
      for (int i = 0; i < s; i++) {
        double* temp = prom_res.top();
        temp[0] += c[0];
        temp[1] += c[1];
        res.push(temp);
        prom_res.pop();
        temp = NULL;
      }
    } else {
      if (f == 0) {
        prom_res.push(mas[delta*rank + ost]);
        prom_res.push(mas[delta*rank + ost + 1]);
        for (int i = delta * rank + ost + 2; i < delta*rank + ost + delta; i++) {
          double* last;
          double* beforelast;
          while (1) {
            last = prom_res.top();
            prom_res.pop();
            beforelast = prom_res.top();
            prom_res.push(last);

            double d = det(last, beforelast, mas[i], 1);
            if (d > 0) {
              prom_res.push(mas[i]);
              break;
            } else {
              if (prom_res.size() >= 3) {
                prom_res.pop();
              } else {
                prom_res.pop();
                prom_res.push(mas[i]);
                break;
              }
            }
          }
        }

        int s = prom_res.size();
        MPI_Send(&s, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        double* buf = new double[2 * prom_res.size()];
        for (int i = 0; i < 2 * s; i += 2) {
          tmp = prom_res.top();
          prom_res.pop();
          buf[i] = tmp[0];
          buf[i + 1] = tmp[1];
        }
        MPI_Send(&buf[0], 2 * s, MPI_DOUBLE, 0, 9, MPI_COMM_WORLD);
      }
    }
  }
  return res;
}

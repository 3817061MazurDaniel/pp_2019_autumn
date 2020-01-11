// Copyright 2019 Rezantsev Sergey
#ifndef MODULES_TASK_3_REZANTSEV_S_GLOBAL_OPT_GLOBAL_OPT_H_
#define MODULES_TASK_3_REZANTSEV_S_GLOBAL_OPT_GLOBAL_OPT_H_
#include <mpi.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>

class Point {
 public:
  double x;
  double y;
  Point(double _x, double _y);
  friend bool operator<(const Point& l, const Point& r) { return l.x < r.x; }
};

class GlobalPoint : public Point {
 public:
  double z;
  GlobalPoint(double _x, double _y, double _z = 0) : Point(_x, _y), z(_z) {}
  friend bool operator<(const GlobalPoint& l, const GlobalPoint& r) {
    return l.x < r.x;
  }
};

class RPoint {
 public:
  double R;
  double x;
  double z;
  double prevX;
  double prevZ;
  RPoint(double _R, double _x, double _z, double _prevX, double _prevZ)
      : R(_R), x(_x), z(_z), prevX(_prevX), prevZ(_prevZ) {}
  friend bool operator<(const RPoint& l, const RPoint& r) { return l.R > r.R; }
};

struct result {
  double x;
  double y;
  double z;
};

class OneDimensionalOpt {
 public:
  result res;
  double a;
  double b;
  double currX;
  double (*function)(double _x, double _y);
  double eps;
  int Nmax;
  std::set<Point> set;
  std::set<Point>::iterator lPoint;
  std::set<Point>::iterator rPoint;
  std::set<Point>::iterator rPointRmax;
  std::set<Point>::iterator lPointRmax;
  double r_p;
  double Rmax;
  double smallestInterval;
  double m;
  double Mmax;
  OneDimensionalOpt(double _a, double _b, double _currx,
                    double (*f)(double _x, double _y), double _eps = 0.001,
                    int _Nmax = 500, double _r_p = 2.0);
  double calculateR();
  double calculateMmax();
  double calculateNewPoint();
  void newFunction(double _a, double _b, double _currx,
                   double (*f)(double _x, double _y));
  void calculatem();
  void OrdinaryPrepearing();
  void calculate();
  void findMmax();
  void findRmax();
};

class GlobalOptimazer {
 public:
  result res;
  double a;
  double b;
  double c;
  double d;
  double (*function)(double _x, double _y);
  double eps;
  int Nmax;
  double r_p;
  double Rmax;
  double smallestInterval;
  double m;
  double Mmax;
  int size;
  int rank;
  std::set<GlobalPoint> set;
  std::set<GlobalPoint>::iterator lP;
  std::set<GlobalPoint>::iterator rP;
  std::set<RPoint> setR;
  std::set<RPoint>::iterator RP;
  result temp;
  GlobalOptimazer(double _a, double _b, double _c, double _d,
                  double (*f)(double _x, double _y), double _eps = 0.05,
                  int _Nmax = 500, double _r_p = 2.0)
      : a(_a),
        b(_b),
        c(_c),
        d(_d),
        function(f),
        eps(_eps),
        Nmax(_Nmax),
        r_p(_r_p) {}
  void globalCalculation();
  double calculateR();
  double calculateMmax();
  double calculateNewPoint();
  void calculatem();
  void findMmax();
  void OrdinaryCalculation();
  void setRPoints();
};

#endif  // MODULES_TASK_3_REZANTSEV_S_GLOBAL_OPT_GLOBAL_OPT_H_

// Copyright 2019 Tihomirova Mariya
#ifndef MODULES_TASK_3_TIHOMIROVA_M_GRAHAM_GRAHAM_H_
#define MODULES_TASK_3_TIHOMIROVA_M_GRAHAM_GRAHAM_H_
#include <mpi.h>
#include <vector>
#include <iostream>

class Point {
 public:
  double x;
  double y;
  Point();
  Point(double _x, double _y);
  Point& operator=(const Point& A) {
    if (this != &A) {
      x = A.x;
      y = A.y;
    }
    return *this;
  }
  bool operator==(const Point& A) {
    return ((x == A.x) && (y == A.y));
  }
  bool operator!=(const Point& A) {
    return ((x != A.x) && (y != A.y));
  }
  Point Minimym(const Point& A, const Point& B);
  ~Point();
};

class Vector {
 public:
  double X;
  double Y;
  Vector();
  Vector(Point A, Point B);
  Vector(double X_, double Y_);
  ~Vector();
};

double Modul(const Vector& Vec);

double Scalar(const Vector& Vec1, const Vector& Vec2);

double Cosin(const Vector& A, const Vector& B);

bool cw(double x1, double y1, double x2, double y2, double x3, double y3);

std::vector<Point> GrahamPar(const std::vector<Point>& Points);

std::vector<Point> GrahamSeq(const std::vector<Point>& Points);

Point SearchMinPoint(const std::vector<Point>& Points);

std::vector<Point> Sort(const std::vector<Point>& Points);

bool ravni(double x1, double y1, double x2, double y2);

std::vector<Point> GenPoints(int n);

#endif  // MODULES_TASK_3_TIHOMIROVA_M_GRAHAM_GRAHAM_H_

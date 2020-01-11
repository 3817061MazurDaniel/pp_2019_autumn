// Copyright 2019 Tihomirova Mariya
#include <mpi.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <random>
#include "../../../modules/task_3/tihomirova_m_graham/graham.h"

Point::Point() {
  x = 0;
  y = 0;
}

Point::Point(double _x, double _y) : x{ _x }, y{ _y } {
}

Point Point::Minimym(const Point& A, const Point& B) {
  if (A.y < B.y) {
    return A;
  } else {
    if ((A.y == B.y) && (A.x < B.x))
      return A;
  }
  return B;
}

Point::~Point() {
}

Vector::Vector() {
  X = 0;
  Y = 0;
}

Vector::Vector(Point A, Point B) {
  X = B.x - A.x;
  Y = B.y - A.y;
}

Vector::Vector(double X_, double Y_) {
  X = X_;
  Y = Y_;
}

double Modul(const Vector& Vec) {
  return (sqrt(Vec.X * Vec.X + Vec.Y * Vec.Y));
}

double Scalar(const Vector& Vec1, const Vector& Vec2) {
  return (Vec1.X * Vec2.X + Vec1.Y * Vec2.Y);
}

Vector::~Vector() {
}


double Cosin(const Vector& A, const Vector& B) {
  return (Scalar(A, B) / (Modul(A) * Modul(B)));
}

Point SearchMinPoint(const std::vector<Point>& Points) {
  Point Min = Points[0];
  for (size_t i = 1; i < Points.size(); i++) {
    Min = Min.Minimym(Min, Points[i]);
  }
  return Min;
}

std::vector<Point> GenPoints(int n) {
  if (n < 1)
    throw "few points";
  std::vector<Point> P(n);
  std::mt19937 gen;
  for (int i = 0; i < n; i++) {
    P[i].x = gen();
    P[i].y = gen();
  }
  return P;
}

std::vector<Point> Sort(const std::vector<Point>& Points) {
  std::vector<Point> P = Points;
  Point Min = SearchMinPoint(P);
  std::vector<double> Cos(P.size());
  Vector VecA(1, 0);
  for (size_t i = 0; i < P.size(); i++) {
    Vector VecB(Min, Points[i]);
    if (Min == Points[i]) {
      Cos[i] = 2;
    } else {
      Cos[i] = Cosin(VecA, VecB);
    }
  }
  for (size_t i = 0; i < P.size() - 1; i++) {
    for (size_t j = 0; j < P.size() - i - 1; j++) {
      if (Cos[j + 1] > Cos[j]) {
        Point temp = P[j + 1];
        P[j + 1] = P[j];
        P[j] = temp;
        double temp_cos = Cos[j + 1];
        Cos[j + 1] = Cos[j];
        Cos[j] = temp_cos;
      }
    }
  }
  for (size_t i = 0; i < P.size() - 1; i++) {
    if (fabs(Cos[i + 1] - Cos[i]) < 0.000001) {
      Vector Vec1(Min, P[i]);
      Vector Vec2(Min, P[i + 1]);
      if (Modul(Vec1) > Modul(Vec2)) {
        Point temp = P[i + 1];
        P[i + 1] = P[i];
        P[i] = temp;
      }
    }
  }
  return P;
}

bool cw(double x1, double y1, double x2, double y2, double x3, double y3) {
  return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1) < 0;
}

bool ravni(double x1, double y1, double x2, double y2) {
  return (fabs(x1 - x2) < 0.000001) && (fabs(y1 - y2) < 0.000001);
}

std::vector<Point> GrahamSeq(const std::vector<Point>& P) {
  std::vector<Point> Points = Sort(P);
  if (Points.size() < 2) {
    return Points;
  }
  std::vector<double> Res;
  Res.push_back(Points[0].x);
  Res.push_back(Points[0].y);
  Res.push_back(Points[1].x);
  Res.push_back(Points[1].y);
  if (Points.size() > 2) {
    for (size_t i = 2; i < Points.size(); i++) {
      while (cw(Res[Res.size() - 4], Res[Res.size() - 3], Res[Res.size() - 2],
        Res[Res.size() - 1], Points[i].x, Points[i].y)) {
        Res.pop_back();
        Res.pop_back();
      }
      Res.push_back(Points[i].x);
      Res.push_back(Points[i].y);
    }
  }
  std::vector<Point> resPoint(Res.size() / 2);
  for (size_t i = 0; i < Res.size(); i += 2) {
    resPoint[i / 2].x = Res[i];
    resPoint[i / 2].y = Res[i + 1];
  }
  return resPoint;
}

std::vector<Point> GrahamPar(const std::vector<Point>& Points) {
  std::vector<double> Res;
  std::vector<Point> P = Sort(Points);
  int n = P.size();
  MPI_Status status;
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int l_n = n / size;
  if (n >= size) {
    std::vector<double> Vec;
    for (size_t i = 0; i < P.size(); i++) {
      Vec.push_back(P[i].x);
      Vec.push_back(P[i].y);
    }
    std::vector<double> l_Vec(2 * l_n);
    MPI_Scatter(&Vec[0], 2 * l_n, MPI_DOUBLE, &l_Vec[0], 2 * l_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (rank == (size - 1)) {
      for (size_t i = size * l_n; i < P.size(); i++) {
        l_Vec.push_back(P[i].x);
        l_Vec.push_back(P[i].y);
      }
    }
    std::vector<Point> l_P(l_Vec.size() / 2);
    for (size_t i = 0; i < l_Vec.size(); i += 2) {
      l_P[i / 2].x = l_Vec[i];
      l_P[i / 2].y = l_Vec[i + 1];
    }
    std::vector<Point> l_resPoint = GrahamSeq(l_P);
    l_Vec.clear();
    for (size_t i = 0; i < l_resPoint.size(); i++) {
      l_Vec.push_back(l_resPoint[i].x);
      l_Vec.push_back(l_resPoint[i].y);
    }
    int vec_n = l_Vec.size();
    std::vector<int> size_Vec(size);
    int N = 0;
    MPI_Gather(&vec_n, 1, MPI_INT, &size_Vec[0], 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Reduce(&vec_n, &N, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    std::vector<double> Global_vec(N);
    if (rank == 0) {
      std::vector<int> address(size);
      address[0] = 0;
      for (int i = 1; i < size; i++) {
        address[i] = size_Vec[i - 1];
      }
      for (int i = 2; i < size; i++) {
        address[i] += address[i - 1];
      }
      for (int i = 0; i < vec_n; i++) {
        Global_vec[i] = l_Vec[i];
      }
      for (int i = 1; i < size; i++) {
        MPI_Recv(&Global_vec[address[i]], size_Vec[i], MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      }
    } else {
      MPI_Send(&l_Vec[0], vec_n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }
    if (rank == 0) {
      std::vector<Point> GlobalResPoint(Global_vec.size() / 2);
      for (size_t i = 0; i < Global_vec.size(); i += 2) {
        GlobalResPoint[i / 2].x = Global_vec[i];
        GlobalResPoint[i / 2].y = Global_vec[i + 1];
      }
      std::vector<Point> resPoint = GrahamSeq(GlobalResPoint);
      return resPoint;
    }
  } else {
    std::vector<Point> resPoint = GrahamSeq(P);
    return resPoint;
  }
  return P;
}

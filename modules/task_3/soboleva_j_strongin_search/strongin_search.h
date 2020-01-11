// Copyright 2019 Soboleva Julia
#ifndef MODULES_TASK_3_SOBOLEVA_J_STRONGIN_SEARCH_STRONGIN_SEARCH_H_
#define MODULES_TASK_3_SOBOLEVA_J_STRONGIN_SEARCH_STRONGIN_SEARCH_H_
#include <vector>

class VectorPointX {
 public:
  std::vector<double> g;  // g - function
  int  n, count;  // n - power, count - count of points
  std::vector<double> x, z;  // x - points, z - function in points
  double a, b;  // a - zero point, b - last point

  VectorPointX(double _a, double _b, int _n);
  VectorPointX(double _a, double _b, int _n,  std::vector<double> _g);

  void addPoint(double newx);
  double countZ(double newx);
};

std::vector<double> getRandVector(int n);
double getM(VectorPointX  u, double r);
std::vector<double> getR(VectorPointX  u, double r);
double stronginSearch(VectorPointX  u, double r, int eps);
double parStronginSearch(VectorPointX  u, double r, int eps);

#endif  // MODULES_TASK_3_SOBOLEVA_J_STRONGIN_SEARCH_STRONGIN_SEARCH_H_

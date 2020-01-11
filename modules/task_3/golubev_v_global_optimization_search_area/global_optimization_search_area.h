// Copyright 2019 Golubev Vladislav
#ifndef MODULES_TASK_3_GOLUBEV_V_GLOBAL_OPTIMIZATION_SEARCH_AREA_GLOBAL_OPTIMIZATION_SEARCH_AREA_H_
#define MODULES_TASK_3_GOLUBEV_V_GLOBAL_OPTIMIZATION_SEARCH_AREA_GLOBAL_OPTIMIZATION_SEARCH_AREA_H_
#include <functional>
#include <vector>

class Optimization {
  int dim;
  double a1;
  double a2;
  double b1;
  double b2;
  std::function<double(double*)> pFunc;
  double eps;

  double getM_XY(int idx, const std::vector<double>& vec);
  double get_m_XY(double M, double r);
  double getR_XY(int idx, double m, const std::vector<double>& vec);
  double getNewX_XY(int t, double m, const std::vector<double>& vec);
  double getFuncValue_XY(double x);

  void node(int is);
  double getM_XYZ(int idx, const std::vector<double>& vec);
  double get_m_XYZ(double M, double r);
  double getR_XYZ(int idx, double m, const std::vector<double>& vec);
  double getNewX_XYZ(int t, double m, const std::vector<double>& vec, double r);
  double getFuncValue_XYZ(double x);

 public :
  void mapd(double x, double* y, int m, int n = 2);
  Optimization(int _dim, double _a1, double _b1,
               std::function<double(double*)> _pFunc, double eps,
               double a2 = 0, double b2 = 0);
  double SequentialGlobalSearchXY(size_t maxIteration = 1000);
  double ParallelGlobalSearchXY(size_t maxIteration = 1000);
  double* SequentialGlobalSearchXYZ(size_t maxIteration = 1000);
  double* ParallelGlobalSearchXYZ(size_t maxIteration = 1000);
};

#endif  // MODULES_TASK_3_GOLUBEV_V_GLOBAL_OPTIMIZATION_SEARCH_AREA_GLOBAL_OPTIMIZATION_SEARCH_AREA_H_

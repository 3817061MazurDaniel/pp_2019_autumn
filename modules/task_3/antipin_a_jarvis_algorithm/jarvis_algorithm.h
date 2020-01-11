// Copyright 2019 Antipin Alexander
#ifndef MODULES_TASK_3_ANTIPIN_A_JARVIS_ALGORITHM_JARVIS_ALGORITHM_H_
#define MODULES_TASK_3_ANTIPIN_A_JARVIS_ALGORITHM_JARVIS_ALGORITHM_H_

#include <mpi.h>
#include <time.h>
#include <iostream>
#include <limits>
#include <vector>
#include <random>

class point {
    double x;
    double y;
 public:
    point();
    point(const point& p);
    point(const double x, const double y);
    ~point();
    void setX(const double x);
    void setY(const double y);
    double getX() const;
    double getY() const;
    bool operator==(const point& p) const;
    bool operator<=(const point& p) const;
    bool operator>(const point& p) const;
};

void getRandomFieldOfPoints(std::vector<point>* field, const int maxX, const int minX, const int minY, const int maxY);
bool isMaxRightPoint(const std::vector<point>& field, const int min, const int start, const int end);
void getSequentialSolution(const std::vector<point>& field, std::vector<int>* result);
void getParallelSolution(const std::vector<point>& field, std::vector<int>* result);

#endif  // MODULES_TASK_3_ANTIPIN_A_JARVIS_ALGORITHM_JARVIS_ALGORITHM_H_

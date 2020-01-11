// Copyright 2019 Astafeva Irina

#ifndef MODULES_TASK_3_ASTAFEVA_I_CONVEX_HULL_CONVEX_HULL_H_
#define MODULES_TASK_3_ASTAFEVA_I_CONVEX_HULL_CONVEX_HULL_H_

#include <mpi.h>
#include <vector>

void printImage(int ** image, int height, int width);
std::vector<int> searchConnectedComponents(int ** image, int height, int width);
int ** buildComponentConvexHull(int ** image, int height, int width);
int ** buildImageConvexHull(int ** image, int height, int width);

#endif  // MODULES_TASK_3_ASTAFEVA_I_CONVEX_HULL_CONVEX_HULL_H_

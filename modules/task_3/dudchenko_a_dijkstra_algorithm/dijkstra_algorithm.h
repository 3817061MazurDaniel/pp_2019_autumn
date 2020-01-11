// Copyright 2019 Dudchenko Anton

#ifndef MODULES_TASK_3_DUDCHENKO_A_DIJKSTRA_ALGORITHM_DIJKSTRA_ALGORITHM_H_
#define MODULES_TASK_3_DUDCHENKO_A_DIJKSTRA_ALGORITHM_DIJKSTRA_ALGORITHM_H_

#include <mpi.h>
#include <vector>

std::vector<int> generateGraph(size_t n);
std::vector<int> sequenceDijkstra(std::vector<int> graph, size_t source);
std::vector<int> parallelDijkstra(std::vector<int> graph, size_t source);

#endif  // MODULES_TASK_3_DUDCHENKO_A_DIJKSTRA_ALGORITHM_DIJKSTRA_ALGORITHM_H_

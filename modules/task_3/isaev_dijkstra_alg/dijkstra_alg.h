// Copyright 2019 Isaev Ilya
#ifndef MODULES_TASK_3_ISAEV_DIJKSTRA_ALG_DIJKSTRA_ALG_H_
#define MODULES_TASK_3_ISAEV_DIJKSTRA_ALG_DIJKSTRA_ALG_H_
#include <vector>

const int infinity = 999999999;
enum GraphType{UNDIRECTED, DIRECTED};

class Graph {
 private:
    std::vector<int> adjacency;
    GraphType type;
 public:
    explicit Graph(const int& vsize = 1, GraphType _type = UNDIRECTED);
    std::vector<int> getAdjacency() const {return adjacency;}
    GraphType getType() const {return type;}
    void setAdjacency(const std::vector<int> _adj);
};

std::vector<int> createRandomGraph(const int& n, GraphType type = UNDIRECTED);
int minDistanceVertex(const std::vector<int>& dist, const std::vector<int>& marker);
std::vector<int> dijkstraSequential(const Graph& graph, const int& src);
std::vector<int> dijkstraParallel(const Graph& graph, const int& src);
std::vector<int> getTransposeAdjacency(const std::vector<int>& mat);

#endif  // MODULES_TASK_3_ISAEV_DIJKSTRA_ALG_DIJKSTRA_ALG_H_

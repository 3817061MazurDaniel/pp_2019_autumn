// Copyright 2019 Dudchenko Anton

#include <vector>
#include <random>
#include <ctime>
#include <iostream>
#include <limits>
#include "../../../modules/task_3/dudchenko_a_dijkstra_algorithm/dijkstra_algorithm.h"

std::vector<int> generateGraph(size_t n) {
    int max_int = std::numeric_limits<int>::max();
    std::vector<int> graph(n*n);
    std::mt19937 engine;
    for (std::size_t i = 0; i < n; i++) {
        for (size_t j = 0; j <= i; j++) {
            if (i == j) {
                graph[i * n + j] = 0;
                continue;
            }
            int tmp1 = engine() % 10;
            if (tmp1 == 0) {
                tmp1 = max_int;
            }
            graph[j * n + i] = graph[i * n + j] = tmp1;
        }
    }
    return graph;
}

std::vector<int> sequenceDijkstra(std::vector<int> graph, size_t source) {
    int n = sqrt(static_cast<int>(graph.size()));
    int max_int = std::numeric_limits<int>::max();
    std::vector<int> distance(n, max_int);
    std::vector<int> visited(n, 0);
    distance[source] = 0;
    for (int i = 0; i < n - 1; i++) {
        int min_dist = max_int;
        int vertex = -1;
        for (size_t j = 0; j < distance.size(); j++) {
            if (distance[j] < min_dist && !visited[j]) {
                min_dist = distance[j];
                vertex = j;
            }
        }
        if (vertex != -1) {
            visited[vertex] = 1;
        } else {
            break;
        }
        for (int j = 0; j < n; j++) {
            if (!visited[j] && graph[j + vertex * n] != max_int
                && (distance[vertex] + graph[j + vertex * n] < distance[j])) {
                distance[j] = distance[vertex] + graph[j + vertex * n];
            }
        }
    }
    return distance;
}

std::vector<int> parallelDijkstra(std::vector<int> graph, size_t source) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int n = sqrt(static_cast<int>(graph.size()));
    int r = n % size;
    int d = n / size;
    int max_int = std::numeric_limits<int>::max();
    std::vector<int> result;
    std::vector<int> buffer(n * d);
    if (d > 0) {
        MPI_Scatter(&graph[r * n], d * n, MPI_INT, &buffer[0], d * n, MPI_INT, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        buffer.insert(buffer.begin(), graph.begin(), graph.begin() + r * n);
    }

    std::vector<bool> visited(rank == 0 ? d + r : d, false);
    std::vector<int> distance(rank == 0 ? d + r : d, max_int);
    int inRoot;
    if (rank == 0) {
        inRoot = r;
    } else {
        inRoot = 0;
    }
    for (int i = 0; i < d + inRoot; i++) {
        distance[i] = buffer[i * n + source];
        if (distance[i] == 0) {
            visited[i] = true;
        }
    }

    for (int i = 0; i < n - 1; i++) {
        int min_dist = max_int;
        int min_index = -1;
        for (size_t j = 0; j < distance.size(); j++) {
            if (distance[j] <= min_dist && !visited[j]) {
                min_dist = distance[j];
                min_index = j;
            }
        }
        int index = -1;
        if (min_index >= 0) {
            index = r - inRoot + min_index + rank * d;
        }

        int recv_index = -1;
        int recv_dist = max_int;
        int glob_index = index;
        int globmin_dist = min_dist;

        if (d > 0) {
            if (rank == 0) {
                for (int i = 1; i < size; i++) {
                    MPI_Recv(&recv_index, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    MPI_Recv(&recv_dist, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                    if (recv_dist < globmin_dist) {
                        globmin_dist = recv_dist;
                        glob_index = recv_index;
                    }
                }
                for (int i = 1; i < size; i++) {
                    MPI_Send(&glob_index, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
                    MPI_Send(&globmin_dist, 1, MPI_INT, i, 3, MPI_COMM_WORLD);
                }
            } else {
                MPI_Send(&index, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                MPI_Send(&min_dist, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
                MPI_Recv(&glob_index, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&globmin_dist, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        if (glob_index == -1) {
            break;
        }
        if (glob_index == index && min_index != -1) {
            visited[min_index] = true;
        }

        for (int i = 0; i < d + inRoot; i++) {
            if (buffer[glob_index + i * n] != max_int && !visited[i]
                && (globmin_dist + buffer[glob_index + i * n] < distance[i])) {
                distance[i] = globmin_dist + buffer[glob_index + i * n];
            }
        }
    }

    if (rank == 0) {
        result.insert(result.begin(), distance.begin(), distance.end());
        std::vector<int> recvRes(d);
        for (int i = 1; i < size; i++) {
            if (d > 0) {
                MPI_Recv(&recvRes[0], d, MPI_INT, i, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                result.insert(result.end(), recvRes.begin(), recvRes.end());
            }
        }
    } else {
        if (d > 0) {
            MPI_Send(&distance[0], d, MPI_INT, 0, 4, MPI_COMM_WORLD);
        }
    }

    return result;
}

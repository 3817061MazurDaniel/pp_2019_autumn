// Copyright Dudchenko Anton 2019
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <stdexcept>
#include "../../../modules/task_1/dudchenko_a_max_vector_value/max_vector_value.h"

std::vector<int> getVector(int size) {
    std::vector<int> randVec(size);
    std::mt19937 engine;
    for (int i = 0; i < size; i++) {
        randVec[i] = engine() % 10000;
    }
    return randVec;
}

int getMaxVectorValue(std::vector<int> vec) {
    int size, rank;
    int result = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int d = vec.size() / (size - 1);
    int r = vec.size() % size;

    if (vec.size() == 0) {
        throw std::runtime_error("Vector size must be nonzero");
    }

    if (rank == 0) {
        if (d > 0) {
            for (int thr = 1; thr < size; thr++)
                MPI_Send(&vec[r] + (thr - 1) * d, d, MPI_INT, thr, 0, MPI_COMM_WORLD);
        }
    } else {
        if (d > 0) {
            MPI_Status status;
            std::vector<int> subvec(d);
            MPI_Recv(&subvec[0], d, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            int max_val = *max_element(subvec.begin(), subvec.end());
            MPI_Send(&max_val, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }
    }

    if (rank == 0) {
        int thr_max;
        MPI_Status status;
        result = *max_element(vec.begin(), vec.begin() + r);
        if (d > 0) {
            for (int thr = 1; thr < size; thr++) {
                MPI_Recv(&thr_max, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
                result = std::max(result, thr_max);
            }
        }
    }
    return result;
}

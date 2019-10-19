// Copyright 2019 Makarova Viktoria

#include <mpi.h>
#include <random>
#include <vector>
#include <algorithm>
#include "./vector_min.h"


int minElemSeq(std::vector<int> ref, int length) {
    return *std::min_element(ref.begin(), ref.end());
}

int minElem(std::vector<int> ref, int length) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int delta = length / size, rem = length % size;

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(&ref[0] + proc * delta + rem,
                        delta,
                        MPI_INT,
                        proc,
                        0,
                        MPI_COMM_WORLD);
        }
    }

    std::vector<int> localVector(delta);
    if (rank == 0) {
        localVector = std::vector<int>(ref.begin(), ref.begin() + delta + rem);
    } else {
        MPI_Recv(&localVector[0],
                    delta,
                    MPI_INT,
                    0,
                    0,
                    MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE);
    }

    int globalMin, localMin = minElemSeq(localVector, localVector.size());
    MPI_Reduce(&localMin, &globalMin, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    return globalMin;
}

std::vector<int> randomVector(int length) {
    std::mt19937 engine;
    std::vector<int> randVector(length);
    engine.seed(length);
    for (int i = 0; i < length; i++) {
        randVector[i] = engine() % 100;
    }
    return randVector;
}

// Copyright 2019 Gribanov Mihail
#include <mpi.h>
#include <random>
#include <vector>
#include <algorithm>
#include <cmath>
#include "./vector_difference.h"

std::vector<int> getVector(int size) {
    std::random_device rd;
    std::vector<int> Vec(size);
    std::mt19937 engine(rd());
    for (int i = 0; i < size; i++) {
        Vec[i] = engine() % 100;
    }
    return Vec;
}

int getMaxDifferenceSeq(const std::vector <int> Vec) {
    int res = 0;
    if (Vec.size() != 0) {
        for (int i = 0; i < static_cast<int>(Vec.size()) - 1; i++) {
            int dif = abs(Vec[i] - Vec[i + 1]);
            if (dif > res)
                res = dif;
        }
    }
    return res;
}

int getMaxDifference(const std::vector <int> Vec) {
    int size, rank;
    int res = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int pairAmt = Vec.size() - 1;
    int delta = pairAmt / size;
    int rem = pairAmt % size;

    if (rank == 0) {
        if (delta > 0) {
            for (int proc = 1; proc < size; proc++) {
                MPI_Send(&Vec[rem] + proc * delta, delta + 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        if (delta > 0) {
            std::vector<int> subVec(delta + 1);
            MPI_Status status;
            MPI_Recv(&subVec[0], delta + 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            int maxDif = getMaxDifferenceSeq(subVec);
            MPI_Send(&maxDif, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    if (rank == 0) {
        int procRes;
        MPI_Status status;
        std::vector<int> subVec(Vec.begin(), Vec.begin() + rem + delta + 1);
        res = getMaxDifferenceSeq(subVec);
        if (delta > 0) {
            for (int proc = 1; proc < size; proc++) {
                MPI_Recv(&procRes, 1, MPI_INT, proc, 0, MPI_COMM_WORLD, &status);
                res = std::max(res, procRes);
            }
        }
    }
    return res;
}

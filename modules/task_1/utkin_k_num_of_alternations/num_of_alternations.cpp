// Copyright 2019 Utkin Konstantin
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/utkin_k_num_of_alternations/num_of_alternations.h"

std::vector<int> getRandomVector(int size) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        if (gen() % 2 == 1) {
            vec[i] = gen() % 100;
        } else {
            vec[i] = -1 * (gen() % 100);
        }
    }
    return vec;
}

int getSequentialOperations(std::vector<int> vec) {
    int numOfAlt = 0;
    const int size = vec.size();
    if (size > 1) {
        for (int i = 1; i < size; ++i) {
            if (vec[i - 1] * vec[i] < 0) {
                ++numOfAlt;
            }
        }
    }
    return numOfAlt;
}

int getParallelOperations(std::vector<int> globalVec) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = globalVec.size() / size;
    const int remain = globalVec.size() % size;

    std::vector<int> localVec(delta + 1);

    if (rank == 0) {
        for (int proc = 1; proc < size; ++proc) {
            MPI_Send(&globalVec[remain] + proc * delta - 1, delta + 1,
                MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
        localVec.resize(delta + remain);
        localVec = std::vector<int>(globalVec.begin(),
            globalVec.begin() + delta + remain);
    } else {
        MPI_Status status;
        MPI_Recv(&localVec[0], delta + 1, MPI_INT, 0, 0,
            MPI_COMM_WORLD, &status);
    }

    int globalSum = 0;
    int localSum = getSequentialOperations(localVec);
    MPI_Reduce(&localSum, &globalSum, 1, MPI_INT, MPI_SUM,
        0, MPI_COMM_WORLD);

    return globalSum;
}

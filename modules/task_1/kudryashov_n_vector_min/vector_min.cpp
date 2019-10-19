// Copyright 2019 Kudryashov Nikita
#include <mpi.h>
#include <vector>
#include <random>
#include "../../../modules/task_1/kudryashov_n_vector_min/vector_min.h"

std::vector<int> getRandomVector(unsigned int size) {
    std::vector<int> rand_vec(size);
    std::mt19937 int_gen;
    int_gen.seed(73);
    for (unsigned int i = 0; i < size; i++) {
        rand_vec[i] = int_gen() % 30;
    }
    return rand_vec;
}

int getVectorMinParallel(std::vector<int> in) {
    int size, rank, step, rest, min;
    int global_min;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    step = in.size()/size;
    rest = in.size() % size;
    min = in[rank * step];

    if (rank == (size - 1)) {
        for (int i = rank * step; i < (rank + 1) * step + rest; i++) {
            if (in[i] < min) {
                min = in[i];
            }
        }
    } else {
        for (int i = rank * step; i < (rank + 1) * step; i++) {
            if (in[i] < min) {
                min = in[i];
            }
        }
    }

    MPI_Reduce(&min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    return global_min;
}

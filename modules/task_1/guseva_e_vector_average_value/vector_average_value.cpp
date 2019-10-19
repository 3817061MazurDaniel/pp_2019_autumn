// Copyright 2019 Guseva Catherine

#include <mpi.h>
#include <iostream>
#include <random>
#include <vector>
#include "../../../modules/task_1/guseva_e_vector_average_value/vector_average_value.h"

std::vector<int> getRandVector(int size) {
    std::mt19937 val;
    std::vector<int> vector(size);
    if (size < 1) {
        throw "ERROR: Wrong size!";
    }
    for (int i = 0; i < size; i++) {
        vector[i] = val() % 10;
    }

    return vector;
}

int parallelMidValueVector(const std::vector <int> vector, int vector_size) {
    int local_sum = 0;
    int global_sum = 0;
    int comm_size, rank;
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (vector_size < comm_size) {
        return sumVector(vector)/vector_size;
    }

    int interval = vector_size / comm_size;
    int ending = vector_size % comm_size;

    std::vector<int>local_vector(interval);

    if (rank == 0) {
        if (interval != 0) {
            for (int proc = 1; proc < comm_size; proc++) {
                MPI_Send(&vector[0] + proc * interval + ending, interval, MPI_INT, proc, 0, MPI_COMM_WORLD);
            }
        }
        local_vector = std::vector<int>(vector.begin(), vector.begin() + interval + ending);
    } else if (interval != 0) {
        MPI_Recv(&local_vector[0], interval, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    local_sum = sumVector(local_vector);

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return global_sum / vector_size;
}

int sumVector(const std::vector<int> vector) {
    int sum = 0;
    int size = vector.size();
    for (int i = 0; i < size; i++) {
        sum += vector[i];
    }
    return sum;
}

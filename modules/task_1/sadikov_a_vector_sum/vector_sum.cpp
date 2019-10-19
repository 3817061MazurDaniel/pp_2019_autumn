// Copyright 2019 Sadikov Artem
#include <mpi.h>
#include <random>
#include <algorithm>
#include <ctime>
#include <vector>
#include "../../../modules/task_1/sadikov_a_vector_sum/vector_sum.h"

std::vector<int> getRandomVector(int length) {
    if (length < 1)
        throw "WRONG_LEN";

    std::vector<int> vec(length);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < length; i++) {
        vec[i] = gen() % 100;
    }

    return vec;
}

int getSum(std::vector<int> vec) {
    const int len = vec.size();
    int sum = 0;

    for (int i = 0; i < len; i++)
        sum += vec[i];

    return sum;
}

int getSumMPI(std::vector<int> vec, int len) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (size > len)
        return getSum(vec);

    int delta = len / size;
    int delta_step = len % size;

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(&vec[0] + proc * delta + delta_step, delta, MPI_INT,
                     proc, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> local_vec(delta);

    if (rank == 0) {
        local_vec = std::vector<int>(vec.begin(),
                                     vec.begin() + delta + delta_step);
    } else {
        MPI_Status st;
        MPI_Recv(&local_vec[0], delta, MPI_INT, 0, 0,
                 MPI_COMM_WORLD, &st);
    }

    int local_max = getSum(local_vec);
    int result = 0;
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce(&local_max, &result, 1, MPI_INT, MPI_SUM,
                0, MPI_COMM_WORLD);

    return result;
}

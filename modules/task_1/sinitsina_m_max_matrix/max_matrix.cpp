// Copyright 2019 Maria Sinitsina
#include <mpi.h>
#include <random>
#include <ctime>
#include <numeric>
#include <vector>
#include <algorithm>
#include <functional>
#include "../../../modules/task_1/sinitsina_m_max_matrix/max_matrix.h"

std::vector<int> RandomMatrix(int row, int col) {
    if (row <= 0) {
        throw - 111;
    }
    if (col <= 0) {
        throw - 111;
    }
    std::mt19937 gen;
    std::vector<int> M(row*col);
    std::default_random_engine rand_val;
    gen.seed(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < row*col; i++) {
        M[i] = gen() % 10;
    }
    return M;
}

int MaxMatrix(const std::vector<int>& vect, int row, int col) {
    if (row <= 0) {
        throw - 111;
    }
    if (col <= 0) {
        throw - 111;
    }
    int res = 0;
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int delta = row * col / size;
    int ost = row * col % size;

    int tmp;
    if (delta > 1) {
        if (rank == 0) {
            for (int i = delta + ost; i <= row * col - delta; i += delta) {
                MPI_Send(&vect[i], delta, MPI_INT, (i - ost) / delta, 0, MPI_COMM_WORLD);
            }
        }
    }
    MPI_Status status;
    std::vector <int> vect1(delta, 0);

    if (rank == 0) {
        tmp = *max_element(vect.begin(), vect.begin() + delta + ost);
    } else {
        if (delta > 1) {
            MPI_Recv(&vect1[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            tmp = std::max(tmp, *max_element(vect1.begin(), vect1.begin() + delta));
        }
    }
    MPI_Reduce(&tmp, &res, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    return res;
}

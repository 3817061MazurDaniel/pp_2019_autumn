// Copyright 2019 Suslov Egor
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/suslov_e_chislo_cheredovaniy/chislo_cheredovaniy.h"

std::vector<int> getRandomVector(int size_v) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vect(size_v);
    for (int  i = 0; i < size_v; i++) { vect[i] = gen() % 100 - 50; }
    return vect;
}

int getChisloCheredovaniy(std::vector<int> vector, int count_size_vector) {
    if (count_size_vector < 2) {
        return 0;
    }
    int chislo_cheredovaniy = 0;
    for (int c = 1; c < count_size_vector; c++) {
        if (vector[c] * vector[ c - 1 ] < 0) {
             chislo_cheredovaniy++;
        }
    }
    return chislo_cheredovaniy;
}

int getParallelOperations(std::vector<int> global_vec, int count_size_vector) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (count_size_vector < 100) {
        if (rank == 0) {
            return getChisloCheredovaniy(global_vec, count_size_vector);
        } else {
            return 0;
        }
    }
    const int full = count_size_vector / size;
    const int ostatok_elem = count_size_vector % size;
    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(&global_vec[ostatok_elem] + proc * full, full, MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }
    std::vector<int> local_vec(full);
    if (rank == 0) {
        local_vec.resize(full + ostatok_elem);
        local_vec = std::vector<int>(global_vec.begin(), global_vec.begin() + full + ostatok_elem);
    } else {
        MPI_Status status;
        MPI_Recv(&local_vec[0], full, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    int global_chislo_cheredovaniy = 0;
    int local_chislo_cheredovaniy = 0;
    if (rank == 0) {
        local_chislo_cheredovaniy += getChisloCheredovaniy(local_vec, local_vec.size());
        std::vector<int> v(size);
        for (int n = 1; n < size; n++) {
            v[n] = global_vec[ostatok_elem - 1 + n * full];
            MPI_Send(&v[n], 1, MPI_INT, n, 0, MPI_COMM_WORLD);
        }
    } else {
        int v;
        MPI_Status status;
        MPI_Recv(&v, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        local_chislo_cheredovaniy += getChisloCheredovaniy(local_vec, local_vec.size());
        if (v*local_vec[0] < 0) {
            local_chislo_cheredovaniy += 1;
        }
    }
    MPI_Reduce(&local_chislo_cheredovaniy, &global_chislo_cheredovaniy, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        return global_chislo_cheredovaniy;
}





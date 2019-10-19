// Copyright 2019 Arisova Anasrasiia
#include <mpi.h>
#include <vector>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/arisova_a_rectangle_integration/rectangle_integration.h"

double getSequentialIntegration(std::vector<double> vec, double(*F)(double), double l_h) {
    double t_sum = 0.0;
    int N = vec.size() - 1;
    for (int i = 0; i < N; i++) {
       t_sum += F((vec[i]+vec[i + 1])/2.0) * l_h;
    }
    return t_sum;
}

double getParallelIntegration(std::vector<double> vec, double(*F)(double), double a, double b, int N) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double>global_vector(N + 1);
    double h = (b - a) / (static_cast<double> (N));
    for (int i = 0; i < N; i++) {
        global_vector[i] = a + i * h;
    }
    global_vector[N] = b;

     int l_N = (N + 1) / size;
     int delta = (N + 1) % size;


    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(&global_vector[0] + delta + proc * l_N, l_N,
                        MPI_DOUBLE, proc, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<double> local_vector(l_N);
    if (rank == 0) {
        local_vector = std::vector<double>(global_vector.begin(),
                                     global_vector.begin() + l_N + delta);
    } else {
        MPI_Status status;
        MPI_Recv(&local_vector[0], l_N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }

    double g_sum = 0.0;
    double l_sum = getSequentialIntegration(local_vector, F, h);
    MPI_Op op_code;
    op_code = MPI_SUM;
    MPI_Reduce(&l_sum, &g_sum, 1, MPI_DOUBLE, op_code, 0, MPI_COMM_WORLD);
    return g_sum;
}

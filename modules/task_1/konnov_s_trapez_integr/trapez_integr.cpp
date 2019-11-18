// Copyright 2019 Konnov Sergey
#include "../../../modules/task_1/konnov_s_trapez_integr/trapez_integr.h"
#include <mpi.h>
#include <functional>
#include <iostream>
#include <algorithm>
#include <cmath>

double GetTrapezIntegrParallel(double l, double r, int n, const std::function<double(double)>& f) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (size == 1 || n < size)
        return GetTrapezIntegrSequential(l, r, n, f);

    int portion = n / size, begin = portion + n % size;
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(&begin, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            begin += portion;
        }
        portion = n / size + n % size;
        begin = 0;
    } else {
        MPI_Status status;
        MPI_Recv(&begin, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    double local_integral = 0, global_integral = 0;
    double left = l + static_cast<double>(begin) * (r - l) / static_cast<double>(n);
    for (int i = 0; i < portion; i++) {
        if (std::abs(left - l) < 1e-9)
            local_integral += f(left) / 2;
        else
            local_integral += f(left);
        left += (r - l) / static_cast<double>(n);
    }
    if (std::abs(left - r) < 1e-9)
        local_integral += f(left)/2;

    MPI_Reduce(&local_integral, &global_integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    global_integral *= ((r - l) / static_cast<double>(n));
    return global_integral;
}

double GetTrapezIntegrSequential(double l, double r, int n, const std::function<double(double)>& f) {
    double left = l;
    double integral = 0;
    for (int i = 0; i < n; i++) {
        if (std::abs(left - l) < 1e-9)
            integral += f(left) / 2;
        else
            integral += f(left);
        left += (r - l) / static_cast<double>(n);
    }
    if (std::abs(left - r) < 1e-9)
        integral += f(left) / 2;
    integral *= ((r - l) / static_cast<double>(n));
    return integral;
}

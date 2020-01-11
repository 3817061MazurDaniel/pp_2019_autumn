// Copyright 2019 Trenina Elizaveta


#include <mpi.h>
#include "../../../modules/task_1/trenina_e_rect_integral/rect_intg.h"


double get_l_rect_intg_sequential(double l, double r, int n, double(*f)(double)) {
    if (n <= 0) throw - 1;

    double h = (r - l) / static_cast<double>(n);
    double integral = 0.0;
    double x;
    for (int i = 0; i < n; i++) {
        x = l + (static_cast<double>(i)) * h;
        integral += f(x);
    }
    integral *= h;
    return integral;
}

double get_midpoint_intg_sequential(double l, double r, int n, double(*f)(double)) {
    if (n <= 0) throw - 1;
    double h = (r - l) / static_cast<double>(n);
    double integral = 0.0;
    double x;
    for (int i = 0; i < n; i++) {
        x = l + (static_cast<double>(i) + 0.5) * h;
        integral += f(x);
    }
    integral *= h;
    return integral;
}

double get_midpoint_intg_parallel(double l, double r, int n, double(*f)(double)) {
    if (n <= 0) throw - 1;

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double h = (r - l) / static_cast<double>(n);
    double local_sum = 0.0;
    double global_sum = 0.0;
    double x;

    for (int i = rank; i < n; i += size) {
        x = l + (static_cast<double>(i)+0.5) * h;
        local_sum += f(x);
    }
    local_sum *= h;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_sum;
}

double get_l_rect_intg_parallel(double l, double r, int n, double(*f)(double)) {
    if (n <= 0) throw - 1;

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double h = (r - l) / static_cast<double>(n);
    double local_sum = 0.0;
    double global_sum = 0.0;
    double x;

    for (int i = rank; i < n; i+=size) {
        x = l + static_cast<double>(i) * h;
        local_sum += f(x);
    }
    local_sum *= h;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_sum;
}



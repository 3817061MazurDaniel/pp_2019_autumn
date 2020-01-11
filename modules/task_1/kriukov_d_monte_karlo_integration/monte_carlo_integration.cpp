// Copyright 2019 Kriukov Dmitry
#include <mpi.h>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/kriukov_d_monte_karlo_integration/monte_carlo_integration.h"


double monteCarloIntegration(double lower_limit, double upper_limit, double(*pfunc)(double), int point_count) {
    if (point_count < 0)
        throw(1);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::mt19937 gen;
    time_t curr_time = static_cast<time_t>(rank);
    gen.seed(static_cast<unsigned int>(time(&curr_time)));
    std::uniform_real_distribution<> urd(0, 1);

    double delta = (upper_limit - lower_limit) / size;
    int local_count = point_count / size;
    double local_sum = 0.0;
    double global_sum = 0.0;
    for (int i = 0; i < local_count; i++) {
        local_sum += pfunc(lower_limit + delta * rank + urd(gen)*delta);
    }
    local_sum = local_sum * delta / local_count;

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_sum;
}


// Copyright 2019 Evdokimov Artem
#include <mpi.h>
#include <stdlib.h>
#include <vector>
#include <random>
#include <ctime>
#include <iostream>
#include "../../../modules/task_1/evdokimov_a_integration_monte_carlo/integration_monte_carlo.h"

double getResultOfIntegral(double upper_limit, double lower_limit, double count_of_dots, double(*function)(double)) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (count_of_dots <= 0)
        throw "count of dots cant be negative";
    if (lower_limit >= upper_limit)
        throw "error of limits";

    double min_function = function(lower_limit);
    double delta = (upper_limit - lower_limit) / size;
    std::vector<double> area(size);
    for (int i = 0; i < size; i++) {
        area[i] = lower_limit + delta * i;
    }

    lower_limit = area[rank];
    double max_function = function(lower_limit + delta);
    double *x = new double[static_cast<int>(count_of_dots)];
    double *y = new double[static_cast<int>(count_of_dots)];
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::uniform_real_distribution<> urd1(lower_limit, upper_limit);
    std::uniform_real_distribution<> urd2(min_function, max_function);
    int count_of_hits = 0;
    for (int ind = 0; ind < count_of_dots; ind++) {
        x[ind] = urd1(gen);
        y[ind] = urd2(gen);
        if (y[ind] < function(x[ind]))
           count_of_hits++;
    }

    double result = count_of_hits * (delta) * (max_function - min_function) / count_of_dots;
    double main_result = 0.0;
    MPI_Reduce(&result, &main_result, 1,
        MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    return main_result;
}

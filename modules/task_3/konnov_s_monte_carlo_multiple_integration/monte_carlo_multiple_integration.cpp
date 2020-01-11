// Copyright 2019 Konnov Sergey
#include "../../../modules/task_3/konnov_s_monte_carlo_multiple_integration/monte_carlo_multiple_integration.h"
#include <mpi.h>
#include <random>
#include <ctime>
#include <iostream>
#include <vector>

double monteCarloMultipleIntegraion(const std::vector<double>& lower_limits,
                                    const std::vector<double>& upper_limits,
                                    int count_of_dots,
                                    const std::function<double(const std::vector<double>&)>& f,
                                    int seed) {
    if (lower_limits.empty() || upper_limits.empty())
        throw "count of limits must be postive";
    if (lower_limits.size() != upper_limits.size())
        throw "count of lower and upper limits must be equal";
    if (count_of_dots <= 0)
        throw "count of dots must be positive";
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int count_of_dimensions = static_cast<int>(lower_limits.size());
    double count_of_dots_proc = count_of_dots/size + (rank < count_of_dots%size?1:0);

    std::mt19937 mt;
    if (seed == -1) {
        mt = std::mt19937(time(0));
    } else {
        mt = std::mt19937(seed + rank);
    }
    std::vector<std::uniform_real_distribution<double>> rand(count_of_dimensions);
    for (int i = 0; i < count_of_dimensions; i++) {
        rand[i] = std::uniform_real_distribution<double>(lower_limits[i], upper_limits[i]);
    }

    double ans = 0.;
    std::vector<double> tmp(count_of_dimensions);
    for (int i = 0; i < count_of_dots_proc; i++) {
        for (int j = 0; j < count_of_dimensions; j++) {
            tmp[j] = rand[j](mt);
        }
        ans += f(tmp);
    }

    double global_ans = 0;
    MPI_Reduce(&ans, &global_ans, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        for (int i = 0 ; i < count_of_dimensions; i++) {
            global_ans *= (upper_limits[i] - lower_limits[i]);
        }
        global_ans /= count_of_dots;
    }
    return global_ans;
}

double monteCarloMultipleIntegraionSequentional(const std::vector<double>& lower_limits,
                                                const std::vector<double>& upper_limits,
                                                int count_of_dots,
                                                const std::function<double(const std::vector<double>&)>& f,
                                                int seed) {
    if (lower_limits.empty() || upper_limits.empty())
        throw "count of limits must be postive";
    if (lower_limits.size() != upper_limits.size())
        throw "count of lower and upper limits must be equal";
    if (count_of_dots <= 0)
        throw "count of dots must be positive";
    int count_of_dimensions = static_cast<int>(lower_limits.size());
    std::mt19937 mt;
    if (seed == -1) {
        mt = std::mt19937(time(0));
    } else {
        mt = std::mt19937(seed);
    }
    std::vector<std::uniform_real_distribution<double>> rand(count_of_dimensions);
    double ans = 0.;
    for (int i = 0; i < count_of_dimensions; i++)
        rand[i] = std::uniform_real_distribution<double>(lower_limits[i], upper_limits[i]);
    std::vector<double> tmp(count_of_dimensions);
    for (int i = 0; i < count_of_dots; i++) {
        for (int j = 0; j < count_of_dimensions; j++)
            tmp[j] = rand[j](mt);
        ans += f(tmp);
    }
    for (int i = 0; i < count_of_dimensions; i++) {
        ans *= (upper_limits[i]-lower_limits[i]);
    }
    ans /= count_of_dots;
    return ans;
}

// Copyright 2019 Korobeinikov Aleksei
#include <mpi.h>
#include <random>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include "../../../modules/task_3/korobeinikov_a_calculation_of_integrals/calculation_of_integrals.h"

double ParallelVersion(double (*func)(std::vector<double>), std::vector <std::pair<double, double>> v,
                      std::vector <int> distr) {
    int numprocs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = v.size();
    std::vector<int> ni(n);
    std::vector<double> hi(n);
    std::vector<std::pair<double, double >> ranges(n);
    int num_elem;
    if (rank == 0) {
        num_elem = 1;
        for (int i = 0; i < n; ++i) {
            ni[i] = distr[i];
            hi[i] = (v[i].second - v[i].first) / ni[i];
            if (i != n - 1) {
                num_elem *= ni[i];
            }
            ranges[i] = v[i];
        }
    }
    MPI_Bcast(&num_elem, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&ni[0], n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&hi[0], n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&ranges[0], 2*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    int ost = num_elem % numprocs;
    int count = num_elem / numprocs + (rank < ost ? 1 : 0);
    std::vector<std::vector<double>> params(count);
    int suf = 0;
    if (rank < ost) {
        suf = rank * count;
    } else {
        suf = ost * (count + 1) + (rank - ost) * count;
    }
    for (int i = 0; i < count; ++i) {
        int number = suf + i;
        for (int j = 0; j < n - 1; ++j) {
            int cef = number % ni[j];
            params[i].push_back(ranges[j].first + cef * hi[j] + hi[j]/2);
            number /= ni[j];
        }
    }
    double sum = 0.0;
    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < ni[n - 1]; ++j) {
            params[i].push_back(ranges[n-1].first + j * hi[n-1] + hi[n-1] / 2);
            sum += func(params[i]);
            params[i].pop_back();
        }
    }
    double gsum = 0.0;
    MPI_Reduce(&sum, &gsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    for (int i = 0; i < n; ++i) {
        gsum *= hi[i];
    }
    return gsum;
}

double SequentialVersion(double (*func)(std::vector<double>), std::vector <std::pair<double, double>> v,
    std::vector <int> distr) {
    int count = distr.size();
    std::vector<double> h(count);
    int64_t number = 1;
    for (int i = 0; i < count; ++i) {
        h[i] = (v[i].second - v[i].first) / distr[i];
        number *= distr[i];
    }
    std::vector <double> params(count);
    double sum = 0.0;
    for (int i = 0; i < number; ++i) {
        int x = i;
        for (int j = 0; j < count; ++j) {
            int cef = x % distr[j];
            params[j] = v[j].first + cef * h[j] + h[j] / 2;
            x /= distr[j];
        }
        sum += func(params);
    }
    for (int i = 0; i < count; ++i) {
        sum *= h[i];
    }
    return sum;
}

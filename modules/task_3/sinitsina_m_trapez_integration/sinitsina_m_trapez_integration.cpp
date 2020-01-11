// Copyright 2019 Sinitsina Maria
#include <vector>
#include "../../../modules/task_3/sinitsina_m_trapez_integration/sinitsina_m_trapez_integration.h"

double ParallelIntegr(double(*func)(std::vector<double>), std::vector <double> x, std::vector <double> y, const int n) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (x.size() != y.size() || n <= 0) {
        throw "Wrong data";
    }
    int dimension = x.size();

    std::vector<double> h(dimension);
    std::vector<double> x1(dimension);
    std::vector<double> y1(dimension);

    int num = pow(n, dimension - 1);

    int n1 = n;
    if (rank == 0) {
        for (int i = 0; i < dimension; ++i) {
            h[i] = (y[i] - x[i]) / n;
            x1[i] = x[i];
            y1[i] = y[i];
        }
    }

    MPI_Bcast(&num, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n1, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&h[0], dimension, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&x1[0], dimension, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&y1[0], dimension, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int ost = num % size;
    int tmp = 0;
    int number = num / size;
    if (rank < ost) {
        number += 1;
        tmp = rank * number;
    } else {
        tmp = rank * number + ost;
    }

    std::vector<std::vector<double>> segments(number);
    for (int i = 0; i < number; ++i) {
        int N = tmp + i;
        for (int j = 0; j < dimension - 1; ++j) {
            segments[i].push_back(x1[j] + h[j] * (N % n + 0.5));
        }
    }

    double preres = 0.0;
    for (int i = 0; i < number; ++i) {
        for (int j = 0; j < n; ++j) {
            segments[i].push_back(x1[dimension - 1] + (j + 0.5) * h[dimension - 1]);
            preres += func(segments[i]);
            segments[i].pop_back();
        }
    }

    double result = 0.0;
    MPI_Reduce(&preres, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    for (int i = 0; i < dimension; ++i) {
        result *= h[i];
    }
    return result;
}

double SequentialIntegr(double(*func)(std::vector<double>), std::vector <double> x,
    std::vector <double> y, const int n) {
    if (x.size() != y.size() || n <= 0) {
        throw "wrong data";
    }
    int dimension = x.size();
    std::vector<double> h(dimension);

    for (int i = 0; i < dimension; ++i) {
        h[i] = (y[i] - x[i]) / static_cast<double>(n);
    }
    std::vector <double> segments(dimension);
    double result = 0.0;
    int num = pow(n, dimension);
    for (int i = 0; i < num; ++i) {
        for (int j = 0; j < dimension; ++j) {
            segments[j] = x[j] + h[j] * (i % n + 0.5);
        }
        result += func(segments);
    }
    for (int i = 0; i < dimension; ++i) {
        result *= h[i];
    }
    return result;
}

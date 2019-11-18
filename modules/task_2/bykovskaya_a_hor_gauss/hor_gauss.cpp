// Copyright 2019 Bykovskaya Alina
#include <mpi.h>
#include <random>
#include <iostream>
#include <ctime>
#include <vector>
#include <stdexcept>
#include "../../../modules/task_2/bykovskaya_a_hor_gauss/hor_gauss.h"

static int offset = 0;

std::vector<double> getRandomMatrix(int rows, int cols, double min,
    double max) {
    std::mt19937 gen;
    gen.seed((unsigned)time(0) + ++offset);
    std::uniform_real_distribution<> dis(min, max);
    std::vector<double> vec(rows * cols);
    for (int i = 0; i < rows * cols; ++i)
        vec[i] = dis(gen);
    return vec;
}

std::vector<double> parallelGauss(const std::vector<double> &a, int rows, int cols) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = rows / size;
    const int rem = rows % size;
    int tmp = delta + (rank < rem ? 1 : 0);
    std::vector<double> loc(tmp * cols);

    if (loc.size() == 0)
        loc.resize(1);

    std::vector<int> sendcounts(size);
    std::vector<int> displs(size);

    displs[0] = 0;
    for (int i = 0; i < size; ++i) {
        sendcounts[i] = (delta + (i < rem ? 1 : 0)) * cols;
        if (i > 0)
            displs[i] = (displs[i - 1] + sendcounts[i - 1]);
    }

    MPI_Scatterv(a.data(), sendcounts.data(), displs.data(),
        MPI_DOUBLE, &loc[0], tmp * cols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    std::vector<double> pivRow(cols);

    for (int i = 0; i < displs[rank] / cols; ++i) {
        int root = 0;
        int sum = 0;
        for (int j = 0; j < size; ++j, ++root) {
            sum += sendcounts[j] / cols;
            if (i < sum) {
                root = j; break;
            }
        }
        MPI_Bcast(&pivRow[0], cols, MPI_DOUBLE, root, MPI_COMM_WORLD);

        for (int j = 0; j < sendcounts[rank] / cols; j++) {
            double s = pivRow[i] / loc[j * cols + i];
            for (int k = i; k < cols; ++k)
                loc[j * cols + k] = s * loc[j * cols + k] - pivRow[k];
        }
    }

    for (int i = 0; i < sendcounts[rank] / cols; ++i) {
        for (int j = 0; j < cols; ++j)
            pivRow[j] = loc[i * cols + j];
        MPI_Bcast(&pivRow[0], cols, MPI_DOUBLE, rank, MPI_COMM_WORLD);
        for (int j = i + 1; j < sendcounts[rank] / cols; j++) {
            double s = pivRow[displs[rank] / cols + i] / loc[j * cols + i + displs[rank] / cols];
            for (int k = i + displs[rank] / cols; k < cols; ++k)
                loc[j * cols + k] = s * loc[j * cols + k] - pivRow[k];
        }
    }

    int root = 0;
    for (int i = displs[rank] / cols + sendcounts[rank] / cols; i < rows; ++i) {
        int sum = 0;
        for (int j = 0; j < size; ++j, ++root) {
            sum += sendcounts[j] / cols;
            if (i < sum) {
                root = j; break;
            }
        }
        MPI_Bcast(&pivRow[0], cols, MPI_DOUBLE, root, MPI_COMM_WORLD);
    }

    std::vector<double> res(0);
    if (rank == 0) res.resize(rows * cols);
    MPI_Gatherv(loc.data(), tmp * cols, MPI_DOUBLE, res.data(),
        sendcounts.data(), displs.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

    std::vector<double> x(0);
    if (rank == 0) {
        x.resize(rows);
        for (int i = rows - 1; i >= 0; --i) {
            double b = res[i * cols + cols - 1];
            for (int j = rows - 1; j >= i + 1; --j)
                b -= res[i * cols + j] * x[j];
            x[i] = b / res[i * cols + i];
        }
    }

    return x;
}

std::vector<double> Gauss(const std::vector<double> &b, int rows, int cols) {
    std::vector <double> a(b);
    for  (int i = 0; i < rows; ++i) {
        for (int j = i + 1; j < rows; ++j) {
            double b = a[i * cols  + i] / a[j * cols + i];
            for (int k = i; k < cols; ++k)
                a[j * cols + k] = a[j * cols + k] * b - a[i * cols + k];
        }
    }
    std::vector<double> x(rows);
    for (int i = rows - 1; i >= 0; --i) {
        double b = a[i * cols + cols - 1];
        for (int j = rows - 1; j >= i + 1; --j)
            b -= a[i * cols + j] * x[j];
        x[i] = b / a[i * cols + i];
    }
    return x;
}

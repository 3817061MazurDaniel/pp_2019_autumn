// Copyright 2019 Korobeinikov Aleksei
#include <mpi.h>
#include <random>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <vector>
#include "./Jacoby_method.h"

double scalar(const std::vector<double>& a, const std::vector<double>& b) {
    if (a.size() != b.size()) {
        throw - 1;
    }
    double ans = 0.0;
    for (unsigned int i = 0; i < a.size(); i++) {
        ans += a[i] * b[i];
    }
    return ans;
}

double getNormForAns(const std::vector <double>& A,
    const std::vector<double> x,
    const std::vector <double>& b) {
    double ans = 0.0;
    int n = b.size();
    for (int i = 0; i < n; i++) {
        std::vector<double> tmp = std::vector<double>(A.begin() + i * n,
            A.begin() + (i + 1) * n);
        ans += fabs(scalar(tmp, x) - b[i]);
    }
    return ans;
}

std::vector<double> Jacobi_Solve(const std::vector <double>& A,
    std::vector <double> x,
    double eps,
    int n, int rank, int part, int numprocs) {
    std::vector <double> x_old(n);
    std::vector <double> x_new(n);
    int i, iter = 0, zero_num;
    double d_norm, d_val;
    std::vector <int> displs(numprocs);
    std::vector <int> sendcnts(numprocs);
    MPI_Scan(&part, &zero_num, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    zero_num -= part;
    MPI_Allgather(&part, 1, MPI_INT, &sendcnts[0], 1,
        MPI_INT, MPI_COMM_WORLD);
    displs[0] = 0;
    for (i = 1; i < numprocs; i++)
        displs[i] = displs[i - 1] + sendcnts[i - 1];
    do {
        for (int i = 0; i < n; ++i) {
            x_old[i] = x[i];
        }
        for (int i = 0; i < part; ++i) {
            double sum = 0;
            for (int j = 0; j < n; j++) {
                if (j != i + zero_num)
                    sum += A[i * (n + 1) + j] * x_old[j];
            }
            x[i + zero_num] = (A[i * (n + 1) + n] - sum) /
                A[i * (n + 1) + i + zero_num];
        }
        MPI_Allgatherv(&x[0] + zero_num, part, MPI_DOUBLE, &x_new[0],
            &sendcnts[0], &displs[0], MPI_DOUBLE,
            MPI_COMM_WORLD);
        x = x_new;
        d_norm = 0;
        if (rank == 0) {
            for (i = 0; i < n; i++) {
                d_val = fabs(x[i] - x_old[i]);
                if (d_norm < d_val) d_norm = d_val;
            }
        }
        MPI_Bcast(&d_norm, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        iter++;
    } while (eps < d_norm);
    return x;
}
std::vector <double> GetSolveOfTask(std::vector<double> matrix,
    std::vector <double> b, int Size, double eps) {
    double Error;
    std::vector <double> A(10);
    int n;
    int numprocs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        n = Size;
        Error = eps;
        A = std::vector <double>(n * (n + 1));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                A[i * (n + 1) + j] = matrix[i * n + j];
            }
            A[i * (n + 1) + n] = b[i];
        }
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&Error, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    std::vector <double> x(n);
    MPI_Bcast(&x[0], n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    int ost = n % numprocs;
    int part = n / numprocs + (rank < ost ? 1 : 0);
    std::vector <double> A_part(part * (n + 1) + (part > 0 ? 0 : 1));
    std::vector <int> displs(numprocs);
    std::vector <int> sendcnts(numprocs);
    int part_size = (n + 1) * part;
    MPI_Gather(&part_size, 1, MPI_INT, &sendcnts[0], 1,
        MPI_INT, 0, MPI_COMM_WORLD);
    displs[0] = 0;
    for (int i = 1; i < numprocs; i++)
        displs[i] = displs[i - 1] + sendcnts[i - 1];
    MPI_Scatterv(&A[0], &sendcnts[0], &displs[0], MPI_DOUBLE,
        &A_part[0], part_size, MPI_DOUBLE, 0,
        MPI_COMM_WORLD);
    x = Jacobi_Solve(A_part, x, Error, n, rank, part, numprocs);
    return x;
}

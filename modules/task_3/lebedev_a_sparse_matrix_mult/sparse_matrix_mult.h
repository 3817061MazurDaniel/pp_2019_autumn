// Copyright 2019 Lebedev Alexander
#ifndef MODULES_TASK_3_LEBEDEV_A_SPARSE_MATRIX_MULT_SPARSE_MATRIX_MULT_H_
#define MODULES_TASK_3_LEBEDEV_A_SPARSE_MATRIX_MULT_SPARSE_MATRIX_MULT_H_

#include <mpi.h>
#include <vector>
#include <ctime>
#include <random>

struct ccsMatrix {
    int                 n;
    std::vector<double> value;
    std::vector<int>    index;
    std::vector<int>    offset;

    ccsMatrix() = default;

    ccsMatrix(int nn, std::vector<double> val, std::vector<int> ind, std::vector<int> offs);

    void init(int nn, std::vector<double> val, std::vector<int> ind, std::vector<int> offs);

    void init(const ccsMatrix& copy);

    void clear() noexcept;

    void transpanent();

    friend bool operator ==(const ccsMatrix& x, const ccsMatrix& y);

    ~ccsMatrix() = default;
};

ccsMatrix generateRandomMatrix(int n, int lowerBound = -1000., int upperBound = 1000.);

ccsMatrix sparseMatrixMultSequential(ccsMatrix A,
                                     ccsMatrix B,
                                     double eps = 0.0000001);

ccsMatrix sparseMatrixMultParallel(ccsMatrix A,
                                    ccsMatrix B,
                                    double eps = 0.0000001,
                                    MPI_Comm = MPI_COMM_WORLD);

void printMatr(const ccsMatrix& check);

#endif  // MODULES_TASK_3_LEBEDEV_A_SPARSE_MATRIX_MULT_SPARSE_MATRIX_MULT_H_

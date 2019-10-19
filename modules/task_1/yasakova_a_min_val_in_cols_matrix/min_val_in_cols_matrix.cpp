// Copyright 2019 Yasakova Anastasia
#include <mpi.h>
#include <vector>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <random>
#include "../../../modules/task_1/yasakova_a_min_val_in_cols_matrix/min_val_in_cols_matrix.h"


std::vector <int> getRandomMatrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        throw - 1;
    }
    std::vector <int> Matrix(rows * cols);
    std::default_random_engine random;
    random.seed(static_cast<unsigned int>(std::time(0)));
    for (int i = 0; i < rows * cols; i++) {
        Matrix[i] = random() % (rows * cols) - rows;
    }
    return Matrix;
}

std::vector <int> ParallelMinInColsMatrix(const std::vector <int>& mtx, int rows, int cols) {
    if (static_cast<int>(mtx.size()) != rows * cols) {
        throw - 1;
    }
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int amount = cols / size;
    int rest = cols % size;
    std::vector<int> rank_v(amount * rows);
    std::vector<int> a(cols * rows);
    if (rank == 0) {
        a = TransposeMatrix(mtx, rows, cols);
        if (amount > 0) {
            for (int proc = 1; proc < size; proc++) {
                MPI_Send(&a[(amount * proc + rest) * rows], amount * rows, MPI_INT, proc, 1, MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Status status;
        if (amount > 0) {
           MPI_Recv(&rank_v[0], amount * rows, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        }
    }
    std::vector<int> res;
    if (rank == 0) {
        MPI_Status status;
        int min_el = rows * cols;
        std::vector<int> min(rows);
        rank_v = std::vector<int>(a.begin(), a.begin() + rest * rows + amount * rows);
        for (int i = 0; i < amount + rest; i++) {
            for (int j = 0; j < rows; j++) {
                min_el = std::min(rank_v[i * rows + j], min_el);
            }
            res.push_back(min_el);
            min_el = rows * cols;
        }
        if (amount > 0) {
            for (int proc = 1; proc < size; proc++) {
                MPI_Recv(&min[0], amount, MPI_INT, proc, 2, MPI_COMM_WORLD, &status);
                res.insert(res.end(), min.begin(), min.begin() + amount);
            }
        }
    } else if (amount > 0) {
        int min_el = rows * cols;
        for (int i = 0; i < amount; i++) {
            for (int j = 0; j < rows; j++) {
                 min_el = std::min(rank_v[i * rows + j], min_el);
            }
            res.push_back(min_el);
            min_el = rows * cols;
        }
        MPI_Send(&res[0], amount, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }
    return res;
}

std::vector <int> TransposeMatrix(const std::vector <int>& mtx, int rows, int cols) {
    std::vector <int> a(rows * cols);
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            a[j + i * rows] = mtx[i + j * cols];
        }
    }
    return a;
}

std::vector <int> SequentialMinInColsMatrix(const std::vector <int>& a, int rows, int cols) {
    std::vector <int> res(cols, rows * cols + 1);
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            res[i] = std::min(a[i + j * cols], res[i]);
        }
    }
    return res;
}

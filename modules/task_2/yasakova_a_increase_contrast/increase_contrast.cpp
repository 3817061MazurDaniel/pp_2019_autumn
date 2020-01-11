// Copyright 2019 Yasakova Anastasia
#include <mpi.h>
#include <vector>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <random>
#include "../../../modules/task_2/yasakova_a_increase_contrast/increase_contrast.h"


std::vector <int> getImage(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
         throw - 1;
    }
    std::vector <int> Matrix(rows * cols);
    std::default_random_engine random;
    random.seed(static_cast<unsigned int>(std::time(0)));
    for (int i = 0; i < rows * cols; i++) {
        Matrix[i] = random() % 256;
    }
    return Matrix;
}

std::vector <int> ParallelIncreaseContrast(const std::vector <int>& mtx, int rows, int cols, int correct) {
    if (static_cast<int>(mtx.size()) != rows * cols || correct < 1) {
        throw - 1;
    }
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int amount = rows / size;
    int rest = rows % size;
    std::vector <int> res(rows * cols);
    std::vector<int> rank_v((amount + 1) * cols);
    int Middle = 0;
    int mid = 0;
    if (rank == 0) {
        if (amount > 0) {
            for (int proc = 1; proc < size; proc++) {
                if (proc < rest) {
                    MPI_Send(&mtx[(amount * proc + proc) * cols],
                        (amount + 1) * cols, MPI_INT, proc, 1, MPI_COMM_WORLD);
                } else {
                    MPI_Send(&mtx[(amount * proc + rest) * cols], amount * cols, MPI_INT, proc, 2, MPI_COMM_WORLD);
                }
            }
        }
    } else if (amount > 0) {
        MPI_Status status;
        if (rank < rest) {
            MPI_Recv(&rank_v[0], (amount + 1) * cols, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            amount++;
        } else {
            MPI_Recv(&rank_v[0], amount * cols, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
        }
    }
    int rest1 = rest;
    if (rank == 0) {
        MPI_Status status;
        if (amount > 0) {
            if (rest > 0) {
                rest1 = 1;
            } else {
                rest1 = 0;
            }
        }
        rank_v = std::vector<int>(mtx.begin(), mtx.begin() + (rest1 + amount) * cols);
        for (int i = 0; i < (rest1 + amount) * cols; i++) {
            Middle+=rank_v[i];
        }
        if (amount > 0) {
            for (int proc = 1; proc < size; proc++) {
                MPI_Recv(&mid, 1, MPI_INT, proc, 3, MPI_COMM_WORLD, &status);
                Middle += mid;
            }
        }
        Middle = Middle / (cols * rows);
    } else if (amount > 0) {
        for (int i = 0; i < amount * cols; i++) {
            Middle += rank_v[i];
        }
        MPI_Send(&Middle, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
    }
    MPI_Bcast(&Middle, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int amount_ = 256 / size;
    int rest_ = 256 % size;
    std::vector <int> palette(256);
    float correction = 1.0 + static_cast<float>(correct) / 100;
    std::vector <int> a(amount_);
    if (rank == 0) {
        MPI_Status status;
        for (int i = 0; i < amount_ + rest_; i++) {
            int delta = i -  Middle;
            palette[i] = Middle + correction * delta;
            if (palette[i] > 255) {
                palette[i] = 255;
            } else if (palette[i] < 0) {
                palette[i] = 0;
            }
        }
        if (amount_ > 0) {
            for (int proc = 1; proc < size; proc++) {
                MPI_Recv(&palette[amount_ + rest_ + amount_ * (proc - 1)], amount_, MPI_INT,
                    proc, 3, MPI_COMM_WORLD, &status);
            }
        }
    } else if (amount_ > 0) {
        for (int i = 0; i < amount_; i++) {
            int delta = i + amount_ + rest_ + amount_ * (rank - 1) - Middle;
            a[i] = Middle + correction * delta;
            if (a[i] > 255) {
                a[i] = 255;
            }
            if (a[i] < 0) {
                a[i] = 0;
            }
        }
        MPI_Send(&a[0], amount_, MPI_INT, 0, 3, MPI_COMM_WORLD);
    }
    MPI_Bcast(&palette[0], 256, MPI_INT, 0, MPI_COMM_WORLD);
    std::vector <int> b(amount * cols);
    if (rank == 0) {
        MPI_Status status;
        for (int i = 0; i < (amount + rest1) * cols; i++) {
            res[i] = palette[mtx[i]];
        }
        if (amount > 0) {
            for (int proc = 1; proc < size; proc++) {
                if (proc < rest) {
                    MPI_Recv(&res[(amount * proc + proc) * cols], (amount + 1) * cols, MPI_INT,
                        proc, 4, MPI_COMM_WORLD, &status);
                } else {
                    MPI_Recv(&res[(amount * proc  + rest) * cols], amount * cols, MPI_INT,
                        proc, 4, MPI_COMM_WORLD, &status);
                }
            }
        }
    } else if (amount > 0) {
        for (int i = 0; i < amount * cols; i++) {
            b[i] = palette[rank_v[i]];
        }
        MPI_Send(&b[0], amount * cols, MPI_INT, 0, 4, MPI_COMM_WORLD);
    }
    return res;
}

std::vector <int> SequentialIncreaseContrast(const std::vector <int>& mtx, int rows, int cols, int correct) {
    if (static_cast<int>(mtx.size()) != rows * cols || correct < 1) {
        throw - 1;
    }
    std::vector <int> res(rows * cols);
    int Middle = 0;
    for (int i = 0; i < rows * cols;  i++) {
        Middle += mtx[i];
    }
    Middle = Middle / (rows * cols);
    float correction = 1.0 + static_cast<float>(correct) / 100;
    std::vector <int> palette(256);
    for (int i = 0; i < 256; i++) {
        int delta = i - Middle;
        palette[i] = Middle + correction * delta;
        if (palette[i] > 255) {
            palette[i] = 255;
        }
        if (palette[i] < 0) {
            palette[i] = 0;
        }
    }
    for (int i = 0; i < rows * cols; i++) {
        res[i] = palette[mtx[i]];
    }
    return res;
}

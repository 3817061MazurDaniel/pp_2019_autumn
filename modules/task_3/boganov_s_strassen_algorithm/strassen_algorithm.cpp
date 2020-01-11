  // Copyright 2019 Boganov Sergey
#include <mpi.h>
#include <vector>
#include <random>
#include <string>
#include <ctime>
#include <iostream>
#include "../../../modules/task_3/boganov_s_strassen_algorithm/strassen_algorithm.h"

std::vector<double> getRandomMatrix(int n, double min_rand, double max_rand) {
    std::uniform_real_distribution<double> unif(min_rand, max_rand);
    std::mt19937 rand_engine(static_cast<int>(time(0)));

    std::vector<double> matrix(n * n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[j + i * n] = unif(rand_engine);
        }
    }

    return matrix;
}

std::vector<double> getSequentialSquareMatrixAddition(int n, const std::vector<double> &A,
const std::vector<double> &B) {
    if (n * n != A.size() || n * n != B.size()) {
        throw static_cast<std::string>("Ожидаются квадратные матрицы одинаковой размерности");
    }
    std::vector<double> C(n * n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[j + i * n] = A[j + i * n] + B[j + i * n];
        }
    }

    return C;
}

std::vector<double> getSequentialSquareMatrixSubtraction(int n, const std::vector<double> &A,
const std::vector<double> &B) {
    if (n * n != A.size() || n * n != B.size()) {
        throw static_cast<std::string>("Ожидаются квадратные матрицы одинаковой размерности");
    }
    std::vector<double> C(n * n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[j + i * n] = A[j + i * n] - B[j + i * n];
        }
    }

    return C;
}

std::vector<double> getSequentialSquareMatrixMultiplication(int n, const std::vector<double> &A,
const std::vector<double> &B) {
    if (n * n != A.size() || n * n != B.size()) {
        throw static_cast<std::string>("Ожидаются квадратные матрицы одинаковой размерности");
    }
    std::vector<double> C(n * n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[j + i * n] += A[k + i * n] * B[j + k * n];
            }
        }
    }

    return C;
}

int getNewSize(int n) {
    int res = 1, temp = n;
    while (temp >>= 1) {
        res <<= 1;
    }
    if (n != res) {
        res <<= 1;
    }
    return res;
}

std::vector<double> getExpandSquareMatrix(int n, const std::vector<double> &A) {
    if (n * n != A.size()) {
        throw static_cast<std::string>("Матрица должна быть квадратной");
    }

    int new_size = getNewSize(n);
    std::vector<double> res(new_size * new_size, 0.0);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            res[j + i * new_size] = A[j + i * n];
        }
    }
    return res;
}

void getFourMatrixBlocks(int n_A, const std::vector<double> &A, std::vector<double> *A11,
std::vector<double> *A12, std::vector<double> *A21, std::vector<double> *A22) {
    if (n_A * n_A != A.size()) {
        throw static_cast<std::string>("Матрица должна быть квадратной");
    }
    if (n_A & (n_A - 1)) {
        throw static_cast<std::string>("N не является степенью двойки");
    }

    int new_size = n_A / 2;

    (*A11).resize(new_size * new_size);
    (*A12).resize(new_size * new_size);
    (*A21).resize(new_size * new_size);
    (*A22).resize(new_size * new_size);

    for (int i = 0; i < new_size; i++) {
        for (int j = 0; j < new_size; j++) {
            (*A11)[j + i * new_size] = A[j + i * n_A];
            (*A12)[j + i * new_size] = A[(new_size + j) + i * n_A];
            (*A21)[j + i * new_size] = A[j + (i + new_size) * n_A];
            (*A22)[j + i * new_size] = A[(new_size + j) + (i + new_size) * n_A];
        }
    }
}

std::vector<double> getJoinBlocksIntoMatrix(int n_block, const std::vector<double> &A11,
const std::vector<double> &A12, const std::vector<double> &A21, const std::vector<double> &A22) {
    if (n_block * n_block != A11.size() || n_block * n_block != A12.size()
    || n_block * n_block != A21.size() || n_block * n_block != A22.size()) {
        throw static_cast<std::string>("Блоки должны быть квадратными матрицами одного размера");
    }

    int n = n_block * 2;

    std::vector<double> A(n * n);

    for (int i = 0; i < n_block; i++) {
        for (int j = 0; j < n_block; j++) {
            A[j + i * n] = A11[j + i * n_block];
            A[(n_block + j) + i * n] = A12[j + i * n_block];
            A[j + (i + n_block) * n] = A21[j + i * n_block];
            A[(n_block + j) + (i + n_block) * n] = A22[j + i * n_block];
        }
    }

    return A;
}

std::vector<double> getSequentialStrassenMatrixMultiplicationAlgorithm(int n, const std::vector<double> &A,
const std::vector<double> &B) {
    if (n <= 64) {
        return getSequentialSquareMatrixMultiplication(n, A, B);
    }

    std::vector<double> A11, A12, A21, A22, B11, B12, B21, B22;

    getFourMatrixBlocks(n, A, &A11, &A12, &A21, &A22);
    getFourMatrixBlocks(n, B, &B11, &B12, &B21, &B22);

    n /= 2;

    std::vector<double> P1, P2, P3, P4, P5, P6, P7;

    P1 = getSequentialStrassenMatrixMultiplicationAlgorithm(n, getSequentialSquareMatrixAddition(n, A11, A22),
    getSequentialSquareMatrixAddition(n, B11, B22));

    P2 = getSequentialStrassenMatrixMultiplicationAlgorithm(n, getSequentialSquareMatrixAddition(n, A21, A22), B11);

    P3 = getSequentialStrassenMatrixMultiplicationAlgorithm(n, A11, getSequentialSquareMatrixSubtraction(n, B12, B22));

    P4 = getSequentialStrassenMatrixMultiplicationAlgorithm(n, A22, getSequentialSquareMatrixSubtraction(n, B21, B11));

    P5 = getSequentialStrassenMatrixMultiplicationAlgorithm(n, getSequentialSquareMatrixAddition(n, A11, A12), B22);

    P6 = getSequentialStrassenMatrixMultiplicationAlgorithm(n, getSequentialSquareMatrixSubtraction(n, A21, A11),
    getSequentialSquareMatrixAddition(n, B11, B12));

    P7 = getSequentialStrassenMatrixMultiplicationAlgorithm(n, getSequentialSquareMatrixSubtraction(n, A12, A22),
    getSequentialSquareMatrixAddition(n, B21, B22));

    std::vector<double> C11, C12, C21, C22;

    C11 = getSequentialSquareMatrixAddition(n, getSequentialSquareMatrixAddition(n, P1, P4),
    getSequentialSquareMatrixSubtraction(n, P7, P5));

    C12 = getSequentialSquareMatrixAddition(n, P3, P5);

    C21 = getSequentialSquareMatrixAddition(n, P2, P4);

    C22 = getSequentialSquareMatrixAddition(n, getSequentialSquareMatrixSubtraction(n, P1, P2),
    getSequentialSquareMatrixAddition(n, P3, P6));

    return getJoinBlocksIntoMatrix(n, C11, C12, C21, C22);
}

std::vector<double> getSequentialStrassenMatrixMultiplication(int n, const std::vector<double> &A,
const std::vector<double> &B) {
    if (n * n != A.size() || n * n != B.size()) {
        throw static_cast<std::string>("Ожидаются квадратные матрицы одинаковой размерности");
    }
    if (n & (n - 1)) {
        int new_size = getNewSize(n);
        std::vector<double> new_A = getExpandSquareMatrix(n, A), new_B = getExpandSquareMatrix(n, B);
        return getSequentialStrassenMatrixMultiplicationAlgorithm(new_size, new_A, new_B);
    } else {
        return getSequentialStrassenMatrixMultiplicationAlgorithm(n, A, B);
    }
}

std::vector<double> coef_P1(const std::vector<double> &A11, const std::vector<double> &A12,
const std::vector<double> &A21, const std::vector<double> &A22, const std::vector<double> &B11,
const std::vector<double> &B12, const std::vector<double> &B21, const std::vector<double> &B22,
int n) {
    return getSequentialStrassenMatrixMultiplicationAlgorithm(n, getSequentialSquareMatrixAddition(n, A11, A22),
    getSequentialSquareMatrixAddition(n, B11, B22));
}

std::vector<double> coef_P2(const std::vector<double> &A11, const std::vector<double> &A12,
const std::vector<double> &A21, const std::vector<double> &A22, const std::vector<double> &B11,
const std::vector<double> &B12, const std::vector<double> &B21, const std::vector<double> &B22,
int n) {
    return getSequentialStrassenMatrixMultiplicationAlgorithm(n, getSequentialSquareMatrixAddition(n, A21, A22), B11);
}

std::vector<double> coef_P3(const std::vector<double> &A11, const std::vector<double> &A12,
const std::vector<double> &A21, const std::vector<double> &A22, const std::vector<double> &B11,
const std::vector<double> &B12, const std::vector<double> &B21, const std::vector<double> &B22,
int n) {
    return getSequentialStrassenMatrixMultiplicationAlgorithm(n, A11,
    getSequentialSquareMatrixSubtraction(n, B12, B22));
}

std::vector<double> coef_P4(const std::vector<double> &A11, const std::vector<double> &A12,
const std::vector<double> &A21, const std::vector<double> &A22, const std::vector<double> &B11,
const std::vector<double> &B12, const std::vector<double> &B21, const std::vector<double> &B22,
int n) {
    return getSequentialStrassenMatrixMultiplicationAlgorithm(n, A22,
    getSequentialSquareMatrixSubtraction(n, B21, B11));
}

std::vector<double> coef_P5(const std::vector<double> &A11, const std::vector<double> &A12,
const std::vector<double> &A21, const std::vector<double> &A22, const std::vector<double> &B11,
const std::vector<double> &B12, const std::vector<double> &B21, const std::vector<double> &B22,
int n) {
    return getSequentialStrassenMatrixMultiplicationAlgorithm(n,
    getSequentialSquareMatrixAddition(n, A11, A12), B22);
}

std::vector<double> coef_P6(const std::vector<double> &A11, const std::vector<double> &A12,
const std::vector<double> &A21, const std::vector<double> &A22, const std::vector<double> &B11,
const std::vector<double> &B12, const std::vector<double> &B21, const std::vector<double> &B22,
int n) {
    return getSequentialStrassenMatrixMultiplicationAlgorithm(n,
    getSequentialSquareMatrixSubtraction(n, A21, A11),
    getSequentialSquareMatrixAddition(n, B11, B12));
}

std::vector<double> coef_P7(const std::vector<double> &A11, const std::vector<double> &A12,
const std::vector<double> &A21, const std::vector<double> &A22, const std::vector<double> &B11,
const std::vector<double> &B12, const std::vector<double> &B21, const std::vector<double> &B22,
int n) {
    return getSequentialStrassenMatrixMultiplicationAlgorithm(n,
    getSequentialSquareMatrixSubtraction(n, A12, A22),
    getSequentialSquareMatrixAddition(n, B21, B22));
}

std::vector<double> (*fP[])(const std::vector<double> &A11, const std::vector<double> &A12,
const std::vector<double> &A21, const std::vector<double> &A22, const std::vector<double> &B11,
const std::vector<double> &B12, const std::vector<double> &B21, const std::vector<double> &B22,
int n) = {coef_P1, coef_P2, coef_P3, coef_P4, coef_P5, coef_P6, coef_P7};

std::vector<double> getParallelStrassenMatrixMultiplicationAlgorithm(int n, const std::vector<double> &A,
const std::vector<double> &B, int size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    if (n <= 64) {
        if (rank == 0) {
            return getSequentialSquareMatrixMultiplication(n, A, B);
        } else {
            return A;
        }
    }

    std::vector<double> A11, A12, A21, A22, B11, B12, B21, B22;
    int m = n / 2;

    if (rank == 0) {
        getFourMatrixBlocks(n, A, &A11, &A12, &A21, &A22);
        getFourMatrixBlocks(n, B, &B11, &B12, &B21, &B22);
    } else {
        A11.resize(m * m);
        A12.resize(m * m);
        A21.resize(m * m);
        A22.resize(m * m);
        B11.resize(m * m);
        B12.resize(m * m);
        B21.resize(m * m);
        B22.resize(m * m);
    }

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(&A11[0], m * m, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(&A12[0], m * m, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(&A21[0], m * m, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(&A22[0], m * m, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(&B11[0], m * m, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(&B12[0], m * m, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(&B21[0], m * m, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(&B22[0], m * m, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&A11[0], m * m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&A12[0], m * m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&A21[0], m * m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&A22[0], m * m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&B11[0], m * m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&B12[0], m * m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&B21[0], m * m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&B22[0], m * m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }

    n /= 2;

    std::vector<std::vector<double>> P(7, std::vector<double>(n * n, 0));

    std::vector<int> count_elem(size, 0);

    for (int i = 0; i < 7; i++) {
        count_elem[i % size]++;
    }

    std::vector<int> start_index(size + 1, 0);

    for (int i = 1; i < size + 1; i++) {
        start_index[i] = start_index[i - 1] + count_elem[i - 1];
    }

    for (int j = start_index[rank]; j < start_index[rank + 1]; j++) {
        P[j] = fP[j](A11, A12, A21, A22, B11, B12, B21, B22, n);
    }

    if (rank == 0) {
        int s = count_elem[0];
        for (int i = 1; i < size; i++) {
            for (int j = 0; j < count_elem[i]; j++) {
                MPI_Recv(&P[s][0], n * n, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
                s++;
            }
        }
    } else {
        for (int i = 0; i < count_elem[rank]; i++) {
            MPI_Send(&P[start_index[rank] + i][0], n * n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
    }

    if (rank == 0) {
        std::vector<double> C11, C12, C21, C22;

        C11 = getSequentialSquareMatrixAddition(n, getSequentialSquareMatrixAddition(n, P[0], P[3]),
        getSequentialSquareMatrixSubtraction(n, P[6], P[4]));

        C12 = getSequentialSquareMatrixAddition(n, P[2], P[4]);

        C21 = getSequentialSquareMatrixAddition(n, P[1], P[3]);

        C22 = getSequentialSquareMatrixAddition(n, getSequentialSquareMatrixSubtraction(n, P[0], P[1]),
        getSequentialSquareMatrixAddition(n, P[2], P[5]));

        return getJoinBlocksIntoMatrix(n, C11, C12, C21, C22);
    } else {
        return A;
    }
}

std::vector<double> getParallelStrassenMatrixMultiplication(int n, const std::vector<double> &A,
const std::vector<double> &B) {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int proc_size = size, err_code = -1;

    if (size > 7) {
        size = 7;
        if (rank >= size) {
            MPI_Status status;

            MPI_Recv(&err_code, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            if (err_code == 1) {
                throw static_cast<std::string>("Ожидаются квадратные матрицы одинаковой размерности");
            }

            return A;
        }
    }

    if (rank == 0) {
        if (n * n != A.size() || n * n != B.size()) {
            err_code = 1;
        }
        for (int i = 1; i < proc_size; i++) {
            MPI_Send(&err_code, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Status status;
        MPI_Recv(&err_code, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    if (err_code == 1) {
        throw static_cast<std::string>("Ожидаются квадратные матрицы одинаковой размерности");
    }

    if (n & (n - 1)) {
        int new_size;
        std::vector<double> new_A, new_B;
        MPI_Status status;

        if (rank == 0) {
            new_size = getNewSize(n);
            new_A = getExpandSquareMatrix(n, A);
            new_B = getExpandSquareMatrix(n, B);
            for (int i = 1; i < size; i++) {
                MPI_Send(&new_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        } else {
            MPI_Recv(&new_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

            new_A.resize(new_size * new_size);
            new_B.resize(new_size * new_size);
        }

        if (rank == 0) {
            for (int i = 1; i < size; i++) {
                MPI_Send(&new_A[0], new_size * new_size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            }
        } else {
            MPI_Recv(&new_A[0], new_size * new_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        }

        if (rank == 0) {
            for (int i = 1; i < size; i++) {
                MPI_Send(&new_B[0], new_size * new_size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            }
        } else {
            MPI_Recv(&new_B[0], new_size * new_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        }

        return getParallelStrassenMatrixMultiplicationAlgorithm(new_size, new_A, new_B, size);
    } else {
        return getParallelStrassenMatrixMultiplicationAlgorithm(n, A, B, size);
    }
}

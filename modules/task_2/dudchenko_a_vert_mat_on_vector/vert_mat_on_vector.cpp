// Copyright 2019 Dudchenko Anton

#include <../../../modules/task_2/dudchenko_a_vert_mat_on_vector/vert_mat_on_vector.h>
#include <mpi.h>
#include <random>
#include <vector>

std::vector<int> getRandVector(std::size_t size) {
    std::vector<int> randVec(size);
    std::mt19937 engine;
    for (std::size_t i = 0; i < size; i++) {
        randVec[i] = engine() % 10;
    }
    return randVec;
}

std::vector<int> getRandMatrix(std::size_t rows, std::size_t columns) {
    std::vector<int> randMat(rows * columns);
    std::mt19937 engine;
    for (std::size_t i = 0; i < rows * columns; i++) {
            randMat[i] = engine() % 10;
    }
    return randMat;
}

std::vector<int> multiMatOnVec(std::vector<int> mat, std::size_t rows, std::size_t columns, std::vector<int> vec) {
    if (columns != vec.size()) {
        throw "Number of columns is not equal vector size";
    }

    int size;
    int rank;

    std::vector<int> res(rows);
    std::vector<int> transpMat(rows * columns);
    std::vector<int> resultMat(rows * columns);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int d = rows * columns / size;
    int r = rows * columns % size;

    std::vector<int> recv_vec(d);
    int ind = 0;
    for (std::size_t i = 0; i < columns; i++) {
        for (std::size_t j = 0; j < rows; j++) {
            transpMat[ind++] = mat[j * columns + i];
        }
    }

    MPI_Scatter(&transpMat[r], d, MPI_INT, &recv_vec[0], d, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < d; i++) {
        int index = (rank * d + i + r) / rows;
        recv_vec[i] *= vec[index];
    }

    MPI_Gather(&recv_vec[0], d, MPI_INT, &resultMat[r], d, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < r; i++) {
            resultMat[i] = transpMat[i] * vec[i / rows];
        }

        for (std::size_t i = 0; i < columns * rows; i++) {
            res[i % rows] += resultMat[i];
        }
    }

    return res;
}

std::vector<int> multiMatOnVecSeq(std::vector<int> mat, std::size_t rows, std::size_t columns, std::vector<int> vec) {
    if (columns != vec.size()) {
        throw "Number of columns is not equal vector size";
    }

    std::vector<int> result(rows);

    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < columns; j++) {
            result[i] += vec[j] * mat[i * columns + j];
        }
    }

    return result;
}

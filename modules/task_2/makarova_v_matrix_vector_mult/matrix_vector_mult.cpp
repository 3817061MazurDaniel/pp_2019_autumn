// Copyright 2019 Makarova Viktoria

#include <mpi.h>
#include <vector>
#include <random>
#include "../../../modules/task_2/makarova_v_matrix_vector_mult/matrix_vector_mult.h"

std::vector<int> getRandomVector(int n) {
    std::mt19937 engine;
    std::vector<int> a(n);
    engine.seed(n);
    int k = 1;
    for (int i = 0; i < n; i++) {
        k = -k;
        a[i] = engine() % 100;
        a[i] = a[i] * k;
    }
    return a;
}

std::vector<int> getRandomMatrix(int rows, int columns) {
    if (rows <= 0 || columns <= 0) {
        throw -1;
    }

    std::vector<int> result(rows * columns);

    std::mt19937 engine;
    engine.seed(rows * columns);

    for (int i = 0; i < rows * columns; i++) {
        int temp = engine() % 100 * (i + 1);
        result.push_back(temp);
    }

    return result;
}

std::vector<int> multiply(std::vector<int> vector,
                          std::vector<int> matrix,
                          int cols, int rows) {
    if ((signed)vector.size() != cols) {
        throw -1;
    }

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int delta = rows / size *cols;
    std::vector<int> buffer(delta);

    MPI_Scatter(&matrix[0], delta, MPI_INT,
                &buffer[0], delta, MPI_INT,
                0, MPI_COMM_WORLD);

    std::vector<int> mResult = multiply_seq(vector, buffer, cols, delta / cols);
    std::vector<int> result(rows);

    MPI_Gather(&mResult[0], delta / cols, MPI_INT,
               &result[0], delta / cols, MPI_INT,
               0, MPI_COMM_WORLD);
    return result;
}

std::vector<int> multiply_seq(std::vector<int> vector,
                              std::vector<int> matrix,
                              int cols, int rows) {
    if ((signed)vector.size() != cols) {
        throw -1;
    }

    std::vector<int> result(rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i] += vector[j] * matrix[i * cols + j];
        }
    }
    return result;
}

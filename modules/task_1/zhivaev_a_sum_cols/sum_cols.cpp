// Copyright 2019 Zhivaev Artem
#include "../../../modules/task_1/zhivaev_a_sum_cols/sum_cols.h"
#include <mpi.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <numeric>

using std::vector;
using std::copy;
using std::transform;
using std::plus;
using std::invalid_argument;
using std::fill;
using std::accumulate;

vector<int> getRandomMatrix(int rows, int cols) {
    vector<int> matrix(rows * cols);
    int counter = 0;
    for (int& i : matrix) {
        i = ++counter;
        counter = counter % 1024;
    }
    return matrix;
}

vector<int> sumColumnsOneProc(const int* matrix, int rows, int cols) {
    if (rows == 0 || cols == 0) {
        return vector<int>(0);
    }
    if (rows < 0 || cols < 0) {
        throw invalid_argument("Negative args");
    }
    vector<int> result(cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j] += matrix[i * cols + j];
        }
    }
    return result;
}

int* transponse(int* matrix, int rows, int cols) {
    int* result = new int[cols * rows];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j * rows + i] = matrix[i * cols + j];
        }
    }
    return result;
}

vector<int> sumColumns(int* matrix, int rows, int cols) {
    if (rows == 0 || cols == 0) {
        return vector<int>(0);
    }
    if (rows < 0 || cols < 0) {
        throw invalid_argument("Negative args");
    }

    int root = 0;

    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        matrix = transponse(matrix, rows, cols);
    } else {
        matrix = nullptr;
    }

    int tmp = rows; rows = cols; cols = tmp;

    vector<int> result(rows);

    int delta = rows / size;
    int remainder = rows % size;
    int localDelta = delta;
    if (rank < remainder) localDelta++;
    int localMatrixSize = localDelta * cols;

    int* localMatrix = new int[localMatrixSize];
    int* localResult = new int[localDelta];

    int* localMatricesSizes = nullptr;
    int* localDeltas = nullptr;
    int* displs = nullptr;

    if (rank == root) {
        localMatricesSizes = new int[size];
        displs = new int[size];
        localDeltas = new int[size];
        fill(localMatricesSizes, localMatricesSizes + size, delta * cols);
        fill(localDeltas, localDeltas + size, delta);
        for (int i = 0; i < remainder; i++) {
            localDeltas[i] += 1;
            localMatricesSizes[i] += cols;
        }
        displs[0] = 0;
        for (int i = 1; i < size; i++) {
            displs[i] = displs[i - 1] + localMatricesSizes[i - 1];
        }
    }

    MPI_Scatterv(matrix, localMatricesSizes, displs, MPI_INT,
                 localMatrix, localMatrixSize, MPI_INT, root, MPI_COMM_WORLD);

    fill(localResult, localResult + delta, 0);
    for (int i = 0; i < localDelta; i++) {
        localResult[i] = accumulate(localMatrix + i * cols, localMatrix + (i + 1) * cols, 0);
    }

    if (rank == root) {
        for (int i = 1; i < size; i++) {
            displs[i] = displs[i - 1] + localDeltas[i - 1];
        }
    }

    MPI_Gatherv(localResult, localDelta, MPI_INT, result.data(), localDeltas, displs, MPI_INT, root, MPI_COMM_WORLD);

    delete[] localMatrix;
    delete[] localResult;
    delete[] matrix;
    delete[] localMatricesSizes;
    delete[] localDeltas;

    return result;
}


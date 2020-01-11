// Copyright 2019 Obolenskiy Arseniy
#include <mpi.h>
#include <algorithm>
#include <ctime>
#include <numeric>
#include <random>
#include <stdexcept>
#include <vector>
#include "../../../modules/task_3/obolenskiy_a_gaussian_image_filtering/gaussian_image_filtering.h"

static int offset = 0;
static const int gaussianKernelSize = 3;
static int gaussianKernel[] = {
    1, 2, 1,
    2, 4, 2,
    1, 2, 1
};
static int gaussianKernelNorm = std::accumulate(
    std::begin(gaussianKernel), std::end(gaussianKernel), 0
);

std::vector <Pixel> getRandomMatrix(int rows, int cols) {
    std::mt19937 gen;
    gen.seed((unsigned)time(0) + ++offset);
    std::uniform_int_distribution<int> dis(0, 255);
    std::vector <Pixel> result(rows * cols);
    for (int i = 0; i < rows * cols; ++i) {
        result[i] = {static_cast<uint8_t>(dis(gen)), static_cast<uint8_t>(dis(gen)), static_cast<uint8_t>(dis(gen))};
    }
    return result;
}

std::vector <Pixel> solveSequential(std::vector <Pixel> a, int rows, int cols) {
    std::vector <Pixel> oldA(a);
    if (rows * cols != static_cast<int>(a.size())) {
        throw std::runtime_error("Matrix sizes does not match");
    }
    if (a.size() == 0) {
        throw std::runtime_error("Image is empty");
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            double r = 0, g = 0, b = 0;
            int gaussianIndex = 0;
            for (int l = -gaussianKernelSize / 2; l <= gaussianKernelSize / 2; ++l) {
                for (int k = -gaussianKernelSize / 2; k <= gaussianKernelSize / 2; ++k) {
                    int idX = std::max(0, std::min(i + l, rows - 1));
                    int idY = std::max(0, std::min(j + k, cols - 1));
                    Pixel p = oldA[idX * cols + idY];
                    r += gaussianKernel[gaussianIndex] * p.r;
                    g += gaussianKernel[gaussianIndex] * p.g;
                    b += gaussianKernel[gaussianIndex] * p.b;
                    ++gaussianIndex;
                }
            }
            r /= gaussianKernelNorm;
            g /= gaussianKernelNorm;
            b /= gaussianKernelNorm;
            Pixel &p = a[i * cols + j];
            p.r = std::max(0, std::min(static_cast<int>(r), 255));
            p.g = std::max(0, std::min(static_cast<int>(g), 255));
            p.b = std::max(0, std::min(static_cast<int>(b), 255));
        }
    }
    return a;
}

std::vector <Pixel> solveParallel(std::vector <Pixel> a, int rows, int cols) {
    int code = 0;
    if (rows * cols != static_cast<int>(a.size())) {
        code = 1;
    } else if (a.size() == 0) {
        code = 2;
    }
    MPI_Bcast(&code, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (code == 1) {
        throw std::runtime_error("Matrix sizes does not match");
    }
    if (code == 2) {
        throw std::runtime_error("Image is empty");
    }

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    size = std::min(size, rows);
    const int delta = rows / size;
    const int rem = rows % size;

    int total_rows = rows;
    if (rank >= size) {
        total_rows = rows = 0;
    }

    if (rows > 2) {
        if (rank == 0) {
            int rowStart = delta + ((rem > 0) ? 1 : 0);
            rows = rowStart + (rank + 1 != size ? 1 : 0);
            for (int i = 1; i < size; ++i) {
                int rowsCount = delta + ((i < rem) ? 1 : 0);
                if (rowsCount == 0) continue;
                int begin = (rowStart - 1) * cols;
                int bufferSize = (rowsCount + 1 + (i + 1 != size ? 1 : 0)) * cols;
                MPI_Send(a.data() + begin, bufferSize, MPI_INT, i, 1, MPI_COMM_WORLD);
                rowStart += rowsCount;
            }
        } else {
            int rowsCount = delta + ((rank < rem) ? 1 : 0);
            if (rowsCount > 0) {
                rows = rowsCount + 1 + (rank + 1 != size ? 1 : 0);
                int bufferSize = rows * cols;
                a.resize(bufferSize);
                MPI_Status status;
                MPI_Recv(a.data(), bufferSize, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            } else {
                total_rows = rows = 0;
            }
        }
    } else if (rank > 0) {
        total_rows = rows = 0;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    std::vector <Pixel> oldA(rows * cols);
    for (int i = 0; i < rows * cols; ++i) {
        oldA[i] = a[i];
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            double r = 0, g = 0, b = 0;
            int gaussianIndex = 0;
            for (int l = -gaussianKernelSize / 2; l <= gaussianKernelSize / 2; ++l) {
                for (int k = -gaussianKernelSize / 2; k <= gaussianKernelSize / 2; ++k) {
                    int idX = std::max(0, std::min(i + l, rows - 1));
                    int idY = std::max(0, std::min(j + k, cols - 1));
                    Pixel p = oldA[idX * cols + idY];
                    r += gaussianKernel[gaussianIndex] * p.r;
                    g += gaussianKernel[gaussianIndex] * p.g;
                    b += gaussianKernel[gaussianIndex] * p.b;
                    ++gaussianIndex;
                }
            }
            r /= gaussianKernelNorm;
            g /= gaussianKernelNorm;
            b /= gaussianKernelNorm;
            Pixel &p = a[i * cols + j];
            p.r = std::max(0, std::min(static_cast<int>(r), 255));
            p.g = std::max(0, std::min(static_cast<int>(g), 255));
            p.b = std::max(0, std::min(static_cast<int>(b), 255));
        }
    }

    if (total_rows > 2) {
        if (rank != 0) {
            int rowsCount = delta + ((rank < rem) ? 1 : 0);
            if (rowsCount > 0) {
                MPI_Send(a.data() + 1 * cols, rowsCount * cols, MPI_INT, 0, 2, MPI_COMM_WORLD);
            }
        } else {
            int rowStart = delta + ((rem > 0) ? 1 : 0);
            for (int i = 1; i < size; ++i) {
                int rowsCount = delta + ((i < rem) ? 1 : 0);
                if (rowsCount == 0) continue;
                MPI_Status status;
                MPI_Recv(a.data() + rowStart * cols, rowsCount * cols, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
                rowStart += rowsCount;
            }
        }
    }
    return a;
}

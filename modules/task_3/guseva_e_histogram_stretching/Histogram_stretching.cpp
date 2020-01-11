// Copyright 2019 Guseva Catherine
#include "../../../modules/task_3/guseva_e_histogram_stretching/Histogram_stretching.h"
#include <mpi.h>
#include <vector>
#include <iostream>
#include <random>

std::vector<int> getRandomImage(int cols, int rows) {
    if (cols <= 0 || rows <= 0) {
        throw "I dont want a lot for Christmas";
    }
    std::mt19937 gen;
    std::vector<int> image(cols*rows);

    for (int i = 0; i < cols; i++) {
        image[i] = static_cast<int>(gen() % 256);
    }

    return image;
}

std::vector<int> HistogramSequential(std::vector<int> src, int cols, int rows) {
    if (static_cast<int>(src.size()) != cols * rows) {
        throw "I dont care about the presents";
    }

    std::vector<int> result(cols * rows);
    float a = 0, b = 0;
    int min = src[0], max = 0;

    for (int i = 0; i < cols*rows; i++) {
        if (src[i] < min) {
            min = src[i];
        }
        if (src[i] > max) {
            max = src[i];
        }
    }

    b = static_cast<float>(255) / (max - min);
    a = static_cast<float>(b * (min * (-1)));

    for (int i = 0; i < cols*rows; i++) {
        result[i] = static_cast<int>(a + b * src[i]);
        if (result[i] > 255) {
            result[i] = 255;
        }
        if (result[i] < 0) {
            result[i] = 0;
        }
    }

    return result;
}

std::vector<int> HistogramParallel(std::vector<int> src, int cols, int rows) {
    if (static_cast<int>(src.size()) != cols * rows) {
        throw "Underneath the Christmas tree";
    }

    int size, rank;
    std::vector<int> globalResult;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rows < size || size == 1) {
        globalResult = HistogramSequential(src, cols, rows);
        return globalResult;
    }

    std::vector<int> minVector(size);
    std::vector<int> maxVector(size);

    int interval = (cols*rows) / size;
    int ending = (cols*rows) % size;

    int* recvCount = new int[size];
    int* displs = new int[size];

    int beginPV = 0;
    int endPV = 0;

    for (int i = 0; i < size; i++) {
        int begin = interval * i;
        int len = interval;
        if (i == size - 1) {
            len += ending;
        }
        recvCount[i] = len;
        displs[i] = i * ((i == size - 1) ? recvCount[i - 1] : len);
        if (i == rank) {
            beginPV = begin;
            endPV = len;
        }
    }

    globalResult = std::vector<int>(src.begin() + beginPV, src.begin() + beginPV + endPV);

    int max_value = getMax(globalResult);
    int min_value = getMin(globalResult);

    int loc_max = 0;
    int loc_min = 0;

    MPI_Allreduce(&max_value, &loc_max, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(&min_value, &loc_min, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

    changePixle(&globalResult, loc_max, loc_min);

    MPI_Gatherv(&globalResult[0], endPV, MPI_INT, &src[0], recvCount, displs, MPI_INT, 0, MPI_COMM_WORLD);

    delete[] recvCount;
    delete[] displs;
    recvCount = nullptr;
    displs = nullptr;

    return src;
}

int getMax(std::vector<int> src) {
    int max = 0;

    for (int i = 0; i < static_cast<int>(src.size()); i++) {
        if (max < src[i]) {
            max = src[i];
        }
    }

    return max;
}

int getMin(std::vector<int> src) {
    int min = src[0];

    for (int i = 0; i < static_cast<int>(src.size()); i++) {
        if (min > src[i]) {
            min = src[i];
        }
    }

    return min;
}

void changePixle(std::vector<int>* temp, int max_value, int min_value) {
    float b = static_cast<float>(255) / (max_value - min_value);
    float a = static_cast<float>(b * (min_value * (-1)));

    for (int i = 0; i < static_cast<int>(temp->size()); i++) {
        (*temp)[i] = static_cast<int>(a + b * (*temp)[i]);
        if ((*temp)[i] > 255) {
            (*temp)[i] = 255;
        }
        if ((*temp)[i] < 0) {
            (*temp)[i] = 0;
        }
    }
}

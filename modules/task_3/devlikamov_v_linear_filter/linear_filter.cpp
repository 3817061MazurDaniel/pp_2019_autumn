// Copyright 2019 Devlikamov V
#include <../../../modules/task_3/devlikamov_v_linear_filter/linear_filter.h>
#include <mpi.h>
#include <iostream>
#include <random>
#include <ctime>
#include <numeric>
#include <vector>
#include <algorithm>
#include <functional>

GaussianBlurs::GaussianBlurs(rgb* input, int rows, int cols) {
    _cols = cols;
    _rows = rows;
    source = new rgb[cols*rows];
    int k = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            source[k] = input[k];
            ++k;
        }
    }
    result = new rgb[cols*rows];
}

rgb* GaussianBlurs::GetResult() {
    return result;
}

rgb GaussianBlurs::Processing(int x, int y) {
    rgb pixel;
    for (int i = 0; i < 3; ++i) {
        int sum = 0;

        for (int j = -1; j <= 1; ++j) {
            for (int k = -1; k <= 1; ++k) {
                int x_idx = x + j;
                int y_idx = y + k;

                if (x_idx < 0 || x_idx > _rows - 1) {
                    x_idx = x;
                }
                if (y_idx < 0 || y_idx > _cols  - 1) {
                    y_idx = y;
                }
                if (x_idx*_cols + y_idx >= _cols*_rows) {
                    x_idx = x;
                    y_idx = y;
                }
                if (i == 0) {
                    sum += source[x_idx*_cols + y_idx].red * static_cast<int>(Mask[j+1][k+1]);
                } else if (i == 1) {
                    sum += source[x_idx*_cols + y_idx].green * static_cast<int>(Mask[j+1][k+1]);
                } else {
                    sum += source[x_idx*_cols + y_idx].blue * static_cast<int>(Mask[j+1][k+1]);
                }
            }
        }
        int newValue = sum / sumOfElementsInMask;
        if (i == 0) {
            pixel.red = newValue;
        } else if (i == 1) {
            pixel.green = newValue;
        } else {
            pixel.blue = newValue;
        }
    }
    return pixel;
}

void GaussianBlurs::process() {
    int k = 0;
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            result[k] = Processing(i, j);
            ++k;
        }
    }
}

rgb* createMatrix(int n, int m) {
    rgb* a = new rgb[n*m];
    int k = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            a[k].red = 128;
            a[k].green = 128;
            a[k].blue = 128;
            ++k;
        }
    }
    return a;
}

rgb* createMatrix1(int n, int m) {
    rgb* a = new rgb[n*m];
    int k = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            a[k].red = (i + j) % 256;
            a[k].green = (i + j) % 256;
            a[k].blue = (i + j) % 256;
            ++k;
        }
    }
    return a;
}

rgb* linear_filter_with_gauss(int n, int m, int code) {
    int rank, numberOfTasks, imgRows, imgCols;
    int *send_counts, *rows_per_process, *displacements;
    rgb *result;
    rgb *source;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfTasks);

    send_counts = new int[numberOfTasks];
    displacements = new int[numberOfTasks];
    rows_per_process = new int[numberOfTasks];
    source = new rgb[n*m];
    result = new rgb[n*m];

    if (rank == 0) {
        if (code == 0) {
            source = createMatrix(n, m);
        } else {
            source = createMatrix1(n, m);
        }
        imgCols = m;
        imgRows = n;
        result = new rgb[n*m];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                result[i*m + j].red = 0;
                result[i*m + j].green = 0;
                result[i*m + j].blue = 0;
            }
        }
        for (int i = 0; i < numberOfTasks; i++) {
            send_counts[i] = displacements[i] = 0;
            rows_per_process[i] = 0;
        }
        int colSize = std::max(imgRows / numberOfTasks, 1);
        for (int i = 0; i < std::min(imgRows, numberOfTasks - 1); i++) {
            rows_per_process[i] = colSize;
            if (i == 0) {
                displacements[i] = 0;
            } else {
                displacements[i] = displacements[i-1] + send_counts[i-1];
            }
            send_counts[i] = colSize * imgCols*3;
        }
        if (numberOfTasks == 1) {
            rows_per_process[0] = colSize;
            displacements[0] = 0;
            send_counts[0] = colSize * imgCols*3;
        } else {
            rows_per_process[std::min(imgRows, numberOfTasks) - 1] = std::max(0,
            n - colSize*(std::min(imgRows, numberOfTasks) - 1));
            displacements[std::min(imgRows, numberOfTasks) - 1] = displacements[std::min(imgRows, numberOfTasks) - 2] +
            send_counts[std::min(imgRows, numberOfTasks) - 2];
            send_counts[std::min(imgRows, numberOfTasks) - 1] =
            rows_per_process[std::min(imgRows, numberOfTasks) - 1]*imgCols*3;
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatter(rows_per_process, 1, MPI_INT, &imgRows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&imgCols, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    rgb *slice;
    slice = new rgb[imgRows*imgCols];
    MPI_Scatterv(source, send_counts, displacements, MPI_BYTE,
                 slice, imgRows * imgCols*3, MPI_BYTE, 0, MPI_COMM_WORLD);

    if (0 <= rank && rank < numberOfTasks) {
        GaussianBlurs* gb = new GaussianBlurs(slice, imgRows, imgCols);
        gb->process();
        slice = gb->GetResult();
        free(gb);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gatherv(slice, imgRows * imgCols*3, MPI_BYTE,
           result, send_counts, displacements, MPI_BYTE, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        delete [] send_counts;
        delete [] displacements;
        delete [] rows_per_process;
        delete [] slice;
    }
    return result;
}

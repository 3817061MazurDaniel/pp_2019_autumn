// Copyright 2019 Astafeva Irina

#include <mpi.h>
#include <random>
#include <ctime>
#include <algorithm>
#include <vector>
#include "../../../modules/task_1/astafeva_i_max_val_in_row/max_val_in_row.h"

std::vector<int> getRandomMatrix(int rows, int columns) {
    if ((rows < 0) || (columns < 0)) throw - 1;
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    int size = rows * columns;
    std::vector<int> matrix(size);
    for (int i = 0; i < size; i++) {
        matrix[i] = gen() % 101;
    }
    return matrix;
}

std::vector<int> getMaxInRows(const std::vector<int> matrix, int rows, int columns) {
    if ((rows <= 0) || (columns <= 0)) throw - 1;
    std::vector<int> maxs(rows);
    int j = 0;
    for (int i = 0; i < rows * columns; i += columns) {
        int maxInRow = *std::max_element(&matrix[i], &matrix[i] + columns);
        maxs[j] = maxInRow;
        j++;
    }
    return maxs;
}

std::vector<int> getMaxInRowsPar(const std::vector<int> matrix, int rows, int columns) {
    if ((rows <= 0) || (columns <= 0)) throw - 1;
    if (rows == 1) {
        std::vector<int> result;
        result.push_back(*std::max_element(&matrix[0], &matrix[0] + columns));
        return result;
    }
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> recvcounts(size);
    std::vector<int> displs(size);
    int s = size;
    if (size > rows) {
        s = rows;
    }
    const int delta_rows = rows / s;
    const int rem_rows = rows % s;
    int start_row = delta_rows;
    if (rem_rows != 0) {
        start_row++;
    }
    recvcounts[0] = start_row;
    displs[0] = 0;
    if ((rank == 0)&&(size > 1)) {
        for (int proc = 1; proc < s; proc++) {
            if (proc < rem_rows) {
                MPI_Send(&matrix[start_row * columns], (delta_rows + 1) * columns, MPI_INT, proc, 0, MPI_COMM_WORLD);
                recvcounts[proc] = delta_rows + 1;
                displs[proc] = start_row;
                start_row += delta_rows;
                start_row++;
            } else {
                MPI_Send(&matrix[start_row * columns], delta_rows * columns, MPI_INT, proc, 0, MPI_COMM_WORLD);
                recvcounts[proc] = delta_rows;
                displs[proc] = start_row;
                start_row += delta_rows;
            }
        }
        if (s != size) {
            for (int proc = s; proc < size; proc++) {
                recvcounts[proc] = 0;
                displs[proc] = start_row - 1;
            }
        }
    }

    std::vector<int> local;
    int local_rows = 0;
    if (rank == 0) {
        if (rem_rows != 0) {
            local_rows = delta_rows + 1;
        } else {
            local_rows = delta_rows;
        }
        local.resize(local_rows * columns);
        local = std::vector<int>(&matrix[0], &matrix[0] + local_rows * columns);
    } else {
        if (rank < rem_rows) {
            local_rows = delta_rows + 1;
        } else {
            local_rows = delta_rows;
        }
        local.resize(local_rows * columns);
        if (rank < s) {
            MPI_Status status;
            MPI_Recv(&local[0], local_rows * columns, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        }
    }
    std::vector<int> global_result(rows);
    std::vector<int> local_result = getMaxInRows(local, local_rows, columns);

    MPI_Gatherv(&local_result[0], local_rows, MPI_INT, &global_result[0],
        &recvcounts[0], &displs[0], MPI_INT, 0, MPI_COMM_WORLD);
    return global_result;
}

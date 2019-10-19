// Copyright 2019 Korobeinikov Aleksei
#include <mpi.h>
#include <random>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <vector>
#include "../../../modules/task_1/korobeinikov_a_max_in_matrix/max_in_matrix.h"

std::vector <int> GetMatrix(int rows, int columns) {
    if (rows < 1 || columns < 1) {
        throw - 1;
    }
    std::vector <int> result(rows * columns);
    std::mt19937 gen(time(0));
    std::uniform_int_distribution<int> ran(-1000, 1000);
    for (int i = 0; i < rows * columns; ++i) {
        result[i] = ran(gen);
    }
    return result;
}

std::vector <int> TranspositionOfMatrix(std::vector <int> vec, int rows) {
    int columns = vec.size() / rows;
    std::vector <int> ans(rows*columns);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            ans[j * rows + i] = vec[i * columns + j];
        }
    }
    return ans;
}

std::vector <int> GetMaxValueInColumnOfMatrix(std::vector<int> matrixx,
    int rows, int columns) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = columns / size;
    int ost = columns % size;
    int count = delta + (rank < ost ? 1 : 0);
    std::vector <int> result;
    std::vector<int> buf(count * rows);
    int cur = delta;
    if (rank == 0) {
        cur = count;
        std::vector <int> matrix = TranspositionOfMatrix(matrixx, rows);
        for (int i = 1; i < size; ++i) {
            int other = delta + (i < ost ? 1 : 0);
            if (other > 0) {
                MPI_Send(&matrix[0] + cur * rows, other * rows,
                    MPI_INT, i, 1, MPI_COMM_WORLD);
                cur += delta + (i < ost ? 1 : 0);
            }
        }
        buf = std::vector<int>(matrix.begin(), matrix.begin() + count * rows);
    } else {
        MPI_Status status;
        if (count > 0) {
            MPI_Recv(&buf[0], count * rows, MPI_INT, 0, 1,
                MPI_COMM_WORLD, &status);
        }
    }

    if (rank == 0) {
        for (int i = 0; i < count; ++i) {
            result.push_back(*max_element(buf.begin() + i * rows,
                buf.begin() + (i + 1) * rows));
        }
        std::vector<int> rescol(delta + 1);
        MPI_Status status;
        for (int i = 1; i < size; ++i) {
            int other = delta + (i < ost ? 1 : 0);
            if (other > 0) {
                MPI_Recv(&rescol[0], other, MPI_INT, i, 2,
                    MPI_COMM_WORLD, &status);
            }
            result.insert(result.end(), rescol.begin(),
                rescol.begin()  + other);
        }

    } else {
        if (count > 0) {
            std::vector <int> tmp(count);
            for (int i = 0; i < count; ++i) {
                tmp[i] = *max_element(buf.begin() + i * rows,
                    buf.begin() + (i + 1) * rows);
            }
            MPI_Send(&tmp[0], count, MPI_INT, 0, 2, MPI_COMM_WORLD);
        }
    }
    return result;
}

std::vector <int> CheckResult(std::vector<int> matrix,
    int rows, int columns) {
    std::vector <int> ans(columns);
    for (int i = 0; i < columns; ++i) {
        ans[i] = matrix[i];
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            ans[j] = std::max(ans[j], matrix[i*columns + j]);
        }
    }
    return ans;
}

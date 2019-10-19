// Copyright 2019 Zhivaev Artem
#include "../../../modules/task_1/zhivaev_a_sum_cols/sum_cols.h"
#include <mpi.h>
#include <vector>
#include <algorithm>

using std::vector;
using std::copy;

vector<vector<int>> getRandomMatrix(const int rows,
                                    const int cols) {
    vector<vector<int>> res(rows);
    for (vector<int>& i : res) {
        i.resize(cols);
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            res[i][j] = i + j;
        }
    }

    return res;
}

vector<int> summColumnsOneProc(vector<vector<int>> matrix) {
    vector<int> result(matrix[0].size());

    for (const vector<int>& i : matrix) {
        for (unsigned int j = 0; j < i.size(); ++j) {
            result[j] += i[j];
        }
    }

    return result;
}

vector<int> summColumns(vector<vector<int>> matrix) {
    int rank, size;
    int rows, cols, delta;
    const int root = 0;

    vector<int> result, localSum;
    vector<vector<int>> locals;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == root) {
        rows = matrix.size();
        cols = matrix[0].size();

        delta = rows / size;
        if (delta * size != rows) {
            delta += 1;
            vector<int> nullVec(cols);
            for (int i = rows; i < delta * size; ++i) {
                matrix.push_back(nullVec);
            }
            rows = matrix.size();
        }
    }

    MPI_Bcast(&rows, 1, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&cols, 1, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&delta, 1, MPI_INT, root, MPI_COMM_WORLD);

    localSum.resize(cols);
    result.resize(cols);

    locals.resize(delta);
    for (vector<int>& i : locals) {
        i.resize(cols);
    }

    if (rank == root) {
        for (int i = 0; i < delta; ++i) {
            copy(matrix[i].begin(), matrix[i].end(),
                 locals[i].begin());
        }

        for (int i = 1; i < size; ++i) {
            for (int j = delta * i; j < delta * (i + 1); ++j) {
                MPI_Send(matrix[j].data(), cols, MPI_INT,
                         i, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Status status;
        for (int i = 0; i < delta; ++i) {
            MPI_Recv(locals[i].data(), cols, MPI_INT,
                     root, 0, MPI_COMM_WORLD, &status);
        }
    }

    for (const vector<int>& i : locals) {
        for (int j = 0; j < cols; ++j) {
            localSum[j] += i[j];
        }
    }

    MPI_Reduce(localSum.data(), result.data(), cols, MPI_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);


    return result;
}


// Copyright 2019 Kondrina Tatyana

#include <mpi.h>

#include <valarray>
#include <ctime>
#include <random>

#include "../../../modules/task_1/kondrina_t_matrix_rows_sum/matrix_rows_sum.h"

std::valarray<int> rowsSumSeq(std::valarray<int> const matrix,
    int const columns, int const rows) {
        std::valarray<int> result(rows);

        for (int currRow = 0; currRow < rows; ++currRow) {
            result[currRow] = matrix[ std::slice(currRow * columns , columns, 1) ].sum();
        }

        return result;
}

std::valarray<int> rowsSum(std::valarray<int> const matrix,
    int const columns, int const rows) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int delta = columns / size,
        remainder = columns % size;
    std::valarray<int> results(rows);

    for (int currRow = 0; currRow < rows; ++currRow) {
        if (rank == 0 && delta > 0) {
            for (int process = 1; process < size; ++process) {
                MPI_Send(&matrix[0] + process * delta + currRow * columns + remainder,
                         delta,
                         MPI_INT,
                         process,
                         currRow,
                         MPI_COMM_WORLD);
            }
        }

        std::valarray<int> buffer(delta + remainder);
        int rowSum = 0;

        if (rank == 0) {
            buffer = matrix[ std::slice(currRow * columns , delta + remainder, 1) ];
        } else {
            MPI_Recv(&buffer[0],
                     delta,
                     MPI_INT,
                     0,
                     currRow,
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
        }

        rowSum = buffer.sum();

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Reduce(&rowSum,
                   &results[currRow],
                   1,
                   MPI_INT,
                   MPI_SUM,
                   0,
                   MPI_COMM_WORLD);
    }

    return results;
}

std::valarray<int> randomMatrix(int const columns, int const rows) {
    std::srand((unsigned)std::time(NULL));

    std::valarray<int> matrix(columns * rows);
    matrix = matrix.apply([](int element) -> int {
                                return element += std::rand() % 100;
                         });

    return matrix;
}

bool valarraysEquality(std::valarray<int> _leftArr, std::valarray<int> _rightArr) {
    if (_leftArr.size() != _rightArr.size()) {
        return false;
    }

    bool check = true;
        for (int element = 0; element < static_cast<int>(_leftArr.size()); ++element) {
            _leftArr[element] == _rightArr[element] ? check = true : check = false;
        }

    return check;
}

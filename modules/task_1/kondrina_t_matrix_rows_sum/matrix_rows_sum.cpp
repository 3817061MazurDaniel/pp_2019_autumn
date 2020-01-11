// Copyright 2019 Kondrina Tatyana

#include "../../../modules/task_1/kondrina_t_matrix_rows_sum/matrix_rows_sum.h"

#include <mpi.h>

#include <algorithm>
#include <ctime>
#include <numeric>
#include <random>
#include <vector>

std::vector<int> rowsSumSeq(std::vector<int> const matrix, int const columns,
                            int const rows) {
  std::vector<int> result;

  for (int currRow = 0; currRow < rows; ++currRow) {
    result.push_back(
        std::accumulate(matrix.begin() + (currRow * columns),
                        matrix.begin() + (currRow * columns) + columns, 0));
  }

  return result;
}

std::vector<int> rowsSum(std::vector<int> const matrix, int const columns,
                         int const rows) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int delta = (rows / size) * columns, remainder = (rows % size) * columns;
  std::vector<int> local_sums;
  std::vector<int> buffer(delta);

  if (delta != 0) {
    if (rank == 0) {
      MPI_Scatter(&matrix[remainder], delta, MPI_INT, &buffer[0], delta,
                  MPI_INT, 0, MPI_COMM_WORLD);
    } else {
      MPI_Scatter(nullptr, 0, MPI_INT, &buffer[0], delta, MPI_INT, 0,
                  MPI_COMM_WORLD);
    }
  }

  if (rank == 0 && remainder != 0) {
    buffer.insert(buffer.begin(), matrix.begin(), matrix.begin() + remainder);
  }

  int index;
  rank == 0 ? index = (delta + remainder) / columns : index = delta / columns;
  local_sums = rowsSumSeq(buffer, columns, index);

  if (rank == 0) {
    buffer.clear();
    buffer.resize(rows - (remainder / columns));
  }

  if (delta != 0) {
    rank == 0 ? index = remainder / columns : index = 0;
    MPI_Gather(&local_sums[index], delta / columns, MPI_INT, &buffer[0],
               delta / columns, MPI_INT, 0, MPI_COMM_WORLD);
  }

  if (rank == 0 && remainder != 0) {
    buffer.insert(buffer.begin(), local_sums.begin(),
                  local_sums.begin() + (remainder / columns));
  }

  return buffer;
}

std::vector<int> randomMatrix(int const columns, int const rows) {
  std::srand((unsigned)std::time(NULL));

  std::vector<int> matrix(columns * rows);
  std::generate(matrix.begin(), matrix.end(),
                []() { return std::rand() % 100 - 50; });

  return matrix;
}

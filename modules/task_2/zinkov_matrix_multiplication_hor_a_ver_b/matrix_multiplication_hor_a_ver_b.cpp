// Copyright 2019 Zinkov Artem
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <numeric>
#include "../../../modules/task_2/zinkov_matrix_multiplication_hor_a_ver_b/matrix_multiplication_hor_a_ver_b.h"

std::vector<int> getRandomMatrix(int matrix_side_size) {
  if (matrix_side_size < 1) {
    throw "WRONG_LEN";
  }

  const int size_matrix = matrix_side_size * matrix_side_size;
  std::vector<int> matrix(size_matrix);

  std::mt19937 gen{
    std::random_device()()
  };
  std::uniform_int_distribution<int> uid(-10, 10);
  std::generate(matrix.begin(), matrix.end(), [&uid, &gen]() -> int {
    return uid(gen);
  });

  return matrix;
}

std::vector<int> getMatrixMultiplication(std::vector<int> matrix_a, std::vector<int> matrix_b, int matrix_side_size) {
  std::vector<int> matrix_c(matrix_side_size * matrix_side_size);
  for (int i = 0; i < matrix_side_size; i++) {
    for (int j = 0; j < matrix_side_size; j++) {
      matrix_c[i * matrix_side_size + j] = 0;
      for (int k = 0; k < matrix_side_size; k++) {
        matrix_c[i * matrix_side_size + j] = matrix_c[i * matrix_side_size + j]
        + matrix_a[i * matrix_side_size + k] * matrix_b[k * matrix_side_size + j];
      }
    }
  }
  return matrix_c;
}

std::vector<int> getMatrixMultiplicationParellel(std::vector<int> matrix_a,
  std::vector<int> matrix_b, int matrix_side_size) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int length_small_block = matrix_side_size / size;
  const int length_big_block = matrix_side_size / size + 1;
  const int residue_size = matrix_side_size % size;
  int lenght_curr_block;

  std::vector<int> resulting_matrix(matrix_side_size * matrix_side_size);
  std::vector<int> local_vec_a, local_vec_b, local_vec_b1, local_vec_c;

  if (matrix_side_size / size < 1) {
    if (rank == 0) {
      resulting_matrix = getMatrixMultiplication(matrix_a, matrix_b, matrix_side_size);
    }
    return resulting_matrix;
  }
  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Datatype MPI_SMALL_BLOCK, MPI_BIG_BLOCK;

  MPI_Type_vector(matrix_side_size, length_small_block, matrix_side_size, MPI_INT, &MPI_SMALL_BLOCK);
  MPI_Type_commit(&MPI_SMALL_BLOCK);
  MPI_Type_vector(matrix_side_size, length_big_block, matrix_side_size, MPI_INT, &MPI_BIG_BLOCK);
  MPI_Type_commit(&MPI_BIG_BLOCK);

  if (rank < residue_size) {
    local_vec_a.resize(matrix_side_size * length_big_block);
    local_vec_c.resize(matrix_side_size * length_big_block);
  } else {
    local_vec_a.resize(matrix_side_size * length_small_block);
    local_vec_c.resize(matrix_side_size * length_small_block);
  }

  if (residue_size == 0) {
    local_vec_b.resize(matrix_side_size * length_small_block);
    if (rank == 0) {
      local_vec_b1.resize(matrix_side_size * length_small_block);
    }
  } else {
    local_vec_b.resize(matrix_side_size * length_big_block);
    if (rank == 0) {
      local_vec_b1.resize(matrix_side_size * length_big_block);
    }
  }

  int* sendcounts = new int[size];
  int* displs = new int[size];

  for (int i = 0; i < size; i++) {
    displs[i] = 0;
    if (i < residue_size) {
      sendcounts[i] = length_big_block * matrix_side_size;
    } else {
      sendcounts[i] = length_small_block * matrix_side_size;
    }
    if (i > 0) {
      displs[i] = displs[i - 1] + sendcounts[i - 1];
    }
  }

  if (rank < residue_size) {
    lenght_curr_block = length_big_block;
  } else {
    lenght_curr_block = length_small_block;
  }

  MPI_Scatterv(matrix_a.data(), sendcounts, displs, MPI_INT,
    &local_vec_a.front(), sendcounts[rank], MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    for (int i = 0; i < matrix_side_size; i++) {
      for (int j = 0; j < lenght_curr_block; j++) {
        local_vec_b[i * lenght_curr_block + j] = matrix_b[j + matrix_side_size * i];
      }
    }

    for (int i = 1; i < residue_size; i++) {
      MPI_Send(matrix_b.data() + i * length_big_block, 1, MPI_BIG_BLOCK, i, 0, MPI_COMM_WORLD);
    }

    int f = residue_size;
    if (residue_size == 0) {
      f = 1;
    }

    for (int i = f; i < size ; i++) {
      MPI_Send(matrix_b.data() + residue_size * length_big_block + (i - residue_size)
        * length_small_block, 1, MPI_SMALL_BLOCK, i, 0, MPI_COMM_WORLD);
    }
  } else {
    MPI_Status status;
    if (rank < matrix_side_size % size) {
      MPI_Recv(&local_vec_b.front(), matrix_side_size * length_big_block, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    } else {
      MPI_Recv(&local_vec_b.front(), matrix_side_size * length_small_block, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
  }

  int size_recv_block;
  int indent_matrix = 0;

  for (int i = 0; i < rank; i++) {
    if (i % size < residue_size) {
      lenght_curr_block = length_big_block;
    } else {
      lenght_curr_block = length_small_block;
    }
    indent_matrix += lenght_curr_block;
  }

  for (int i = 0; i < size; i++) {
    if ((rank  + i) % size < residue_size) {
      lenght_curr_block = length_big_block;
    } else {
      lenght_curr_block = length_small_block;
    }

    for (size_t j = 0; j < local_vec_a.size() / matrix_side_size; j++) {
      for (int l = 0; l < matrix_side_size; l++) {
        for (int k = 0; k < lenght_curr_block; k++) {
          local_vec_c[(indent_matrix + j * matrix_side_size + k) % local_vec_c.size()] =
            local_vec_c[(indent_matrix + j * matrix_side_size + k) % local_vec_c.size()] +
            local_vec_a[matrix_side_size * j + l] * local_vec_b[k + l * lenght_curr_block];
        }
      }
    }

    indent_matrix += lenght_curr_block;

    if (indent_matrix == matrix_side_size) {
      indent_matrix = 0;
    }

    if (i < size - 1) {
      MPI_Status status;
      MPI_Send(&lenght_curr_block, 1, MPI_INT, (rank + size - 1) % size, 0, MPI_COMM_WORLD);
      MPI_Recv(&size_recv_block, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD, &status);

      if (rank == 0) {
        local_vec_b1 = local_vec_b;
        MPI_Recv(&local_vec_b.front(), matrix_side_size * size_recv_block,
          MPI_INT, (rank + 1) % size, 1, MPI_COMM_WORLD, &status);
        MPI_Send(local_vec_b1.data(), matrix_side_size* lenght_curr_block,
          MPI_INT, (rank + size - 1) % size, 1, MPI_COMM_WORLD);
      } else {
        MPI_Send(local_vec_b.data(), matrix_side_size* lenght_curr_block,
          MPI_INT, (rank + size - 1) % size, 1, MPI_COMM_WORLD);
        MPI_Recv(&local_vec_b.front(), matrix_side_size* size_recv_block,
          MPI_INT, (rank + 1) % size, 1, MPI_COMM_WORLD, &status);
      }
    }
  }

  MPI_Gatherv(local_vec_c.data(), sendcounts[rank], MPI_INT, &resulting_matrix.front(),
    sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Type_free(&MPI_BIG_BLOCK);
  MPI_Type_free(&MPI_SMALL_BLOCK);
  return resulting_matrix;
}

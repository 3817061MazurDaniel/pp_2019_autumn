// Copyright 2019 Mazur Daniil
#include <mpi.h>
#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include <cmath>
#include "../../../modules/task_3/mazur_d_sobele/sobele.h"

std::vector<int> randomMatrix(int rows, int cols) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> startMatrix(cols * rows);
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      startMatrix[i * rows + j] = static_cast<int>(gen() % 256);

  return startMatrix;
}

int sobeleFilt(std::vector <int> matrix, int cols, int pos) {
  int xRes = 0;
  int yRes = 0;

  xRes = matrix[pos + cols - 1] + 2 * matrix[pos + cols] + matrix[pos + cols + 1];
  xRes = xRes - matrix[pos - cols - 1] - 2 * matrix[pos - cols] - matrix[pos - cols + 1];
  yRes = matrix[pos - cols + 1] + 2 * matrix[pos + 1] + matrix[pos + cols + 1];
  yRes = yRes - matrix[pos - cols - 1] - 2 * matrix[pos - 1] - matrix[pos + cols - 1];

  return static_cast<int> (sqrt(xRes * xRes + yRes * yRes));
}

std::vector<int> soloSobele(std::vector<int> matrix, int rows, int cols) {
  if ((rows < 3) || (cols < 3))
    return matrix;
  std::vector<int> finishMatrix(cols * rows);

  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j) {
      if ((i == 0) || (j == 0) || (i == rows - 1) || (j == cols - 1))
        finishMatrix[i * cols + j] = 0;
      else
        finishMatrix[i * cols + j] = sobeleFilt(matrix, cols, i* cols + j);
    }

  return finishMatrix;
}

std::vector<int> parSobele(std::vector<int> matrix, int rows, int cols) {
  int comm_size, rank;
  MPI_Status status;
  if ((rows < 3) || (cols < 3))
    return matrix;
  std::vector<int> parMatrix(cols * rows);

  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if ((rows < comm_size + 1) || (comm_size <= 2)) {
    return soloSobele(matrix, rows, cols);
  }


  MPI_Barrier(MPI_COMM_WORLD);
  int pack = 0;
  int pack2 = 0;
  int worker = comm_size - 1;

    if (rows % worker == 0) {
    pack = rows / worker;
    pack2 = pack;
  } else {
      pack = rows / worker;
      pack2 = (rows % worker) + (rows / worker);
  }

  /*if (rows % worker == 0) {
    pack = rows / worker;
    pack2 = pack;
  } else if ((rows % worker + rows / worker) < (worker - 1)) {
      pack2 = rows % worker + rows / worker;
      pack = rows / worker;
  } else if ((rows % worker + rows / worker) == (worker - 1))
    {
      pack = rows / worker;
      pack2 = worker - 1;
  } else {
      pack2 = rows % worker + rows / worker - (worker - 1);
      pack = rows / worker + 1;
  }*/
  // std::cout << '\n' << "for" << rows << "rows pack = " << pack << " pack2 = " << pack2 << '\n';
  // MPI_Barrier(MPI_COMM_WORLD);
  // return soloSobele(matrix, rows, cols);


  if (rank == 0) {
    for (int i = 1; i < comm_size; i++) {
      int firstPix;
      int pixCount;
      if (i == 1) {
        pixCount = cols * (pack + 1);
        firstPix = 0;
      } else if (i == comm_size - 1) {
        pixCount = cols * (pack2 + 1);
          firstPix =  cols * pack * (i - 1) - cols;
      } else {
        pixCount = cols * (pack + 2);
        firstPix = cols * pack * (i - 1) - cols;
      }

      MPI_Send(&matrix[firstPix], pixCount, MPI_INT, i, i*10, MPI_COMM_WORLD);
    }
  } else if (rank != 0) {
    int pixCount;

    std::vector<int> inMatrix(pack * cols + 2 * cols);
    std::vector<int> outMatrix(pack * cols);

    if (rank == 1) {
      inMatrix.resize(cols * (pack + 1));
      pixCount = cols * (pack + 1);

      MPI_Recv(&inMatrix[0], pixCount, MPI_INT, 0, rank * 10, MPI_COMM_WORLD, &status);
      for (int i = 0; i < pack; ++i)
        for (int j = 0; j < cols; ++j)
          if ((i == 0) || (j == 0) || (j == cols - 1))
            outMatrix[i * cols + j] = 0;
          else
            outMatrix[i * cols + j] = sobeleFilt(inMatrix, cols, i*cols + j);
      MPI_Send(&outMatrix[0], pack * cols, MPI_INT, 0, rank * 20, MPI_COMM_WORLD);

    } else if (rank == comm_size - 1) {
      inMatrix.resize(cols * (pack2 + 1));
      outMatrix.resize(cols * pack2);
      pixCount = cols * (pack2 + 1);

      MPI_Recv(&inMatrix[0], pixCount, MPI_INT, 0, rank * 10, MPI_COMM_WORLD, &status);
      for (int i = 1; i <= pack2; ++i)
        for (int j = 0; j < cols; ++j)
          if ((i == pack2) || (j == 0) || (j == cols - 1))
            outMatrix[(i - 1) * cols + j] = 0;
          else
            outMatrix[(i - 1) * cols + j] = sobeleFilt(inMatrix, cols, i * cols + j);

      MPI_Send(&outMatrix[0], pack2 * cols, MPI_INT, 0, rank * 20, MPI_COMM_WORLD);

    } else {
      pixCount = cols * (pack + 2);
      MPI_Recv(&inMatrix[0], pixCount, MPI_INT, 0, rank * 10, MPI_COMM_WORLD, &status);
      for (int i = 1; i <= pack; ++i)
        for (int j = 0; j < cols; ++j)
          if ((j == 0) || (j == cols - 1))
            outMatrix[(i - 1) * cols + j] = 0;
          else
            outMatrix[(i - 1)  * cols + j] = sobeleFilt(inMatrix, cols, i * cols + j);

      MPI_Send(&outMatrix[0], pack * cols, MPI_INT, 0, rank * 20, MPI_COMM_WORLD);
    }
  }

  if (rank == 0) {
    int start = 0;
    for (int i = 1; i < comm_size; ++i) {
      if (i != comm_size - 1) {
        start = cols * pack * (i - 1);
        MPI_Recv(&parMatrix[start], pack * cols, MPI_INT, i, i * 20, MPI_COMM_WORLD, &status);
      } else {
        int start = cols * pack * (i - 1);
        MPI_Recv(&parMatrix[start], pack2 * cols, MPI_INT, i, i*20, MPI_COMM_WORLD, &status);
      }
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);
  return parMatrix;
}

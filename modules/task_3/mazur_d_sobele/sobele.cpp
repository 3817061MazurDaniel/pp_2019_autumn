#include <mpi.h>
#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include <cmath>
#include "../../../modules/task_3/mazur_d_sobele/sobele.h"

std::vector<unsigned char> randomMatrix(int rows, int cols) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<unsigned char> startMatrix(cols * rows);
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      startMatrix[i * rows + j] = static_cast<unsigned char>(gen() % 256);

  return startMatrix;
}

unsigned char sobeleFilt(std::vector <unsigned char> matrix, int rows, int cols, int xPix, int yPix) {
  int xRes = 0;
  int yRes = 0;
  xRes = matrix[xPix + cols - 1] + 2 * matrix[xPix + cols] + matrix[xPix + cols + 1] - matrix[xPix - cols - 1] - 2 * matrix[xPix - cols] - matrix[xPix - cols + 1];
  yRes = matrix[xPix - cols + 1] + 2 * matrix[xPix + 1] + matrix[xPix + cols + 1] - matrix[xPix - cols - 1] + 2 * matrix[xPix - 1] + matrix[xPix - cols + 1];

    return static_cast<unsigned char> (sqrt(xRes * xRes + yRes * yRes));
}

std::vector<unsigned char> soloSobele(std::vector<unsigned char> matrix, int rows, int cols) {
  std::vector<unsigned char> finishMatrix(cols * rows);
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j) {
      if ((i == 0) || (j == 0) || (i = rows - 1) || (j = cols - 1))
        finishMatrix[i * cols + j] = 0;
      else
        finishMatrix[i * cols + j] = sobeleFilt(matrix, rows, cols, i, j);
    }
  return finishMatrix;
}

std::vector<unsigned char> parSobele(std::vector<unsigned char> matrix, int rows, int cols) {
  int comm_size, rank;
  
  MPI_Status status;

  std::vector<unsigned char> parMatrix(cols * rows);

  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if ((rows < comm_size)||(comm_size <= 2)) {

    return soloSobele(matrix, rows, cols);
  }

  int pack = rows / comm_size;
  int pack2 = rows % comm_size;
  std::vector<unsigned char> inMatrix(pack * cols + 2 * cols);
  std::vector<unsigned char> outMatrix(pack * cols);

  if (rank == 0) {

    for (int i = 1; i < comm_size; i++) {
      int firstPix;
      int pixCount;
      if (i == 1) {
        pixCount = cols * (pack + 1);
        firstPix = matrix[0];
      }
      else if (i == comm_size - 1) {
        pixCount = cols * (pack2 + 1);
          firstPix = matrix[0] + cols * pack * (i - 1) - cols;
      }
      else {
        pixCount = cols * (pack + 2);
        firstPix = matrix[0] + cols * pack * (i - 1) - cols;
      }

      MPI_Send(&matrix[0] + firstPix - 1, pixCount, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD);

      
    }
  }
  else if (rank != 0) {
    int pixCount;
    if (rank == 1) {
      
      inMatrix.resize(cols * (pack + 1));
      pixCount = cols * (pack + 1);
      
      MPI_Recv(&inMatrix, pixCount, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, &status);
      
      for (int i = 0; i < pack; ++i)
        for (int j = 0; j < cols; ++j)
          if ((i == 0) || (j == 0) || (j == cols - 1))
            outMatrix[i * cols + j] = 0;
          else
            outMatrix[i * cols + j] = sobeleFilt(inMatrix, pack, cols, j, i);

      MPI_Send(&outMatrix[0], pack * cols, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
    } else if (rank != comm_size - 1) {
      pixCount = cols * (pack + 2);
      MPI_Recv(&inMatrix, pixCount, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, &status);
      for (int i = 0; i < pack; ++i)
        for (int j = 0; j < cols; ++j)
          if ((j == 0) || (j == cols - 1))
            outMatrix[i * cols + j] = 0;
          else
            outMatrix[i * cols + j] = sobeleFilt(inMatrix, pack, cols, j, i);

      MPI_Send(&outMatrix[0], pack * cols, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else {
      inMatrix.resize(cols * (pack2 + 1));
      pixCount = cols * (pack2 + 1);
      MPI_Recv(&inMatrix[0], pixCount, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, &status);
      for (int i = 0; i < pack2; ++i)
        for (int j = 0; j < cols; ++j)
          if ((i == rows - 1) || (j == 0) || (j == cols - 1))
            outMatrix[i * cols + j] = 0;
          else
            outMatrix[i * cols + j] = sobeleFilt(inMatrix, pack2, cols, j, i);

      MPI_Send(&outMatrix[0], pack2 * cols, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
    }
  }
  if (rank == 0) {
    for (int i = 1; i < comm_size - 1; ++i) {
      if (i != comm_size - 1) {
        int start = cols * pack * (i - 1);
        MPI_Recv(&parMatrix[0] + start, pack * cols, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD, &status);
      }
      else {
        int start = cols * pack * (i - 1);
        MPI_Recv(&parMatrix[0] + start, pack * cols, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD, &status);
      }
    }

    return parMatrix;
  }

 }



#include <mpi.h>
#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include <cmath>
#include "../../../modules/task_3/mazur_d_sobele/sobele.h"

std::vector<unsigned int> randomMatrix(int rows, int cols) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<unsigned int> startMatrix(cols * rows);
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      startMatrix[i * rows + j] = static_cast<unsigned int>(gen() % 256);

  return startMatrix;
}

unsigned int sobeleFilt(std::vector <unsigned int> matrix, int cols, int pos) {
  int xRes = 0;
  int yRes = 0;

  xRes = matrix[pos + cols - 1] + 2 * matrix[pos + cols] + matrix[pos + cols + 1] - matrix[pos - cols - 1] - 2 * matrix[pos - cols] - matrix[pos - cols + 1];
  yRes = matrix[pos - cols + 1] + 2 * matrix[pos + 1] + matrix[pos + cols + 1] - matrix[pos - cols - 1] - 2 * matrix[pos - 1] - matrix[pos + cols - 1];

  /*if (pos == cols + 1)
  {
  std::cout << "pix =" << matrix[pos] << "\n";
  std::cout << "xRes =" << xRes << "\n";
  std::cout << "yRes =" << yRes << "\n";
  std::cout << "___________________________________" << '\n';
  std::cout << matrix[pos - cols - 1] << ' ' << matrix[pos - cols] << ' ' << matrix[pos - cols + 1] << '\n';
  std::cout << matrix[pos - 1] << ' ' << matrix[pos] << ' ' << matrix[pos + 1] << '\n';
  std::cout << matrix[pos + cols - 1] << ' ' << matrix[pos + cols] << ' ' << matrix[pos + cols + 1] << '\n';
  std::cout << "_____________________________________" << '\n';
  }*/
  return static_cast<unsigned int> (sqrt(xRes * xRes + yRes * yRes));
}

std::vector<unsigned int> soloSobele(std::vector<unsigned int> matrix, int rows, int cols) {
  
  std::vector<unsigned int> finishMatrix(cols * rows);
  
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j) {
      if ((i == 0) || (j == 0) || (i == rows - 1) || (j == cols - 1))
        finishMatrix[i * cols + j] = 0;
      else
        finishMatrix[i * cols + j] = sobeleFilt(matrix, cols, i* cols + j);
    }
  
  return finishMatrix;
}

std::vector<unsigned int> parSobele(std::vector<unsigned int> matrix, int rows, int cols) {
  ///int parSobele(std::vector<unsigned int> matrix, int rows, int cols) {
  int comm_size, rank;
  MPI_Status status;

  std::vector<unsigned int> parMatrix(cols * rows);

  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if ((rows < comm_size + 1)||(comm_size <= 2)) {
    return soloSobele(matrix, rows, cols);
  }

  std::cout << "proc " << rank << " started mpi" << '\n';
  MPI_Barrier(MPI_COMM_WORLD);
  int pack = 0;
  int pack2 = 0;

  if (rows % (comm_size - 1) != 0) {
    pack = rows / (comm_size - 1) + 1;
    if ((comm_size - 2) * pack > rows)
      --pack;
    pack2 = rows - pack * (comm_size - 2);
  } else {
    pack = rows / (comm_size - 1);
    pack2 = pack;
  }
  std::cout << "proc" << rank << "pack= " << pack  << '\n';
  std::cout << "proc" << rank << "pack2= " << pack2  << '\n';
  
  

  if (rank == 0) {

    for (int i = 1; i < comm_size; i++) {
      int firstPix;
      int pixCount;
      if (i == 1) {
        pixCount = cols * (pack + 1);
        firstPix = 0;
      }
      else if (i == comm_size - 1) {
        pixCount = cols * (pack2 + 1);
          firstPix =  cols * pack * (i - 1) - cols;
      }
      else {
        pixCount = cols * (pack + 2);
        firstPix = cols * pack * (i - 1) - cols;
      }

      MPI_Send(&matrix[firstPix], pixCount, MPI_UNSIGNED, i, i*10, MPI_COMM_WORLD);

      std::cout << "Proc 0 sended packs to" << i << "proc" << '\n';
    }
  }
  else if (rank != 0) {
    int pixCount;

    std::vector<unsigned int> inMatrix(pack * cols + 2 * cols);
    std::vector<unsigned int> outMatrix(pack * cols);
    std::cout << "proc " << rank << " initialized IN and OUT matrix" << '\n';

    if (rank == 1) {
      
      inMatrix.resize(cols * (pack + 1));
      pixCount = cols * (pack + 1);
      
      MPI_Recv(&inMatrix[0], pixCount, MPI_UNSIGNED, 0, rank * 10, MPI_COMM_WORLD, &status);
      //MPI_Barrier(MPI_COMM_WORLD);
      std::cout << "proc" << rank << "get inMatrix" << '\n';
      for (int i = 0; i < pack; ++i)
        for (int j = 0; j < cols; ++j)
          if ((i == 0) || (j == 0) || (j == cols - 1))
            outMatrix[i * cols + j] = 0;
          else
            outMatrix[i * cols + j] = sobeleFilt(inMatrix, cols, i*cols + j);
      std::cout << "proc" << rank << "filted" << '\n';
      MPI_Send(&outMatrix[0], pack * cols, MPI_UNSIGNED, 0, rank * 20, MPI_COMM_WORLD);
    
    
    }
    else if (rank == comm_size - 1) {
      inMatrix.resize(cols * (pack2 + 1));
      pixCount = cols * (pack2 + 1);

      MPI_Recv(&inMatrix[0], pixCount, MPI_UNSIGNED, 0, rank * 10, MPI_COMM_WORLD, &status);
      //MPI_Barrier(MPI_COMM_WORLD);
      std::cout << "proc" << rank << "get inMatrix" << '\n';
      for (int i = 1; i <= pack2; ++i)
        for (int j = 0; j < cols; ++j)
          if ((i == pack2) || (j == 0) || (j == cols - 1))
            outMatrix[(i - 1) * cols + j] = 0;
          else
            outMatrix[(i - 1) * cols + j] = sobeleFilt(inMatrix, cols, i * cols + j);
      std::cout << "proc" << rank << "filted" << '\n';

      MPI_Send(&outMatrix[0], pack2 * cols, MPI_UNSIGNED, 0, rank * 20, MPI_COMM_WORLD);
      
    }
    
    else  {
      pixCount = cols * (pack + 2);
      MPI_Recv(&inMatrix[0], pixCount, MPI_UNSIGNED, 0, rank * 10, MPI_COMM_WORLD, &status);
      //MPI_Barrier(MPI_COMM_WORLD);
      std::cout << "proc" << rank << "get inMatrix" << '\n';
      for (int i = 1; i <= pack; ++i)
        for (int j = 0; j < cols; ++j)
          if ((j == 0) || (j == cols - 1))
            outMatrix[(i - 1) * cols + j] = 0;
          else
            outMatrix[(i - 1)  * cols + j] = sobeleFilt(inMatrix, cols, i * cols + j);
      std::cout << "proc" << rank << "filted" << '\n';

      MPI_Send(&outMatrix[0], pack * cols, MPI_UNSIGNED, 0, rank * 20, MPI_COMM_WORLD);

    }

  }
  
  
  if (rank == 0) {
    int start = 0;
    for (int i = 1; i < comm_size; ++i) {
      if (i != comm_size - 1) {
        start = cols * pack * (i - 1);
        MPI_Recv(&parMatrix[start], pack * cols, MPI_UNSIGNED, i, i * 20, MPI_COMM_WORLD, &status);
      }
      else {
        int start = cols * pack * (i - 1);
        MPI_Recv(&parMatrix[start], pack2 * cols, MPI_UNSIGNED, i, i*20, MPI_COMM_WORLD, &status);
      }
      std::cout << "proc 0 get from " << rank << "filted" << '\n';
    }

  }
  MPI_Barrier(MPI_COMM_WORLD);
  return parMatrix;
 }



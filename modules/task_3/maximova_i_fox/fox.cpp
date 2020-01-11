// Copyright 2019 Maximova Irina
#include <mpi.h>
#include <cmath>
#include <ctime>
#include <random>
#include <stdexcept>
#include "../../../modules/task_3/maximova_i_fox/fox.h"

#define ndims 2
#define reoder true

int procNum;
int rank;
int cartSize;
int blockSize;
MPI_Comm comm_cart;
MPI_Comm comm_row;
MPI_Comm comm_col;
int coords[2];

void ImportResult(double* C, double* Cblock, int size) {
  double* rowbuff = new double[size * blockSize];
  for (int i = 0; i < blockSize; i++) {
    MPI_Gather(&Cblock[i * blockSize], blockSize, MPI_DOUBLE,
               &rowbuff[i * size], blockSize, MPI_DOUBLE, 0, comm_row);
  }
  if (coords[1] == 0) {
    MPI_Gather(rowbuff, blockSize * size, MPI_DOUBLE, C, blockSize * size,
               MPI_DOUBLE, 0, comm_col);
  }
  delete[] rowbuff;
}

void Scatter(double* Matrix, double* block, int size) {
  double* rowbuff = new double[blockSize * size];
  if (coords[1] == 0) {
    MPI_Scatter(Matrix, blockSize * size, MPI_DOUBLE, rowbuff, blockSize * size,
                MPI_DOUBLE, 0, comm_col);
  }
  for (int i = 0; i < blockSize; i++) {
    MPI_Scatter(&rowbuff[i * size], blockSize, MPI_DOUBLE,
                &(block[i * blockSize]), blockSize, MPI_DOUBLE, 0, comm_row);
  }
  delete[] rowbuff;
}

void BlockExport(double* A, double* B, double* AMatrixblock, double* Bblock,
                 int size) {
  Scatter(A, AMatrixblock, size);
  Scatter(B, Bblock, size);
}

void ABlockTransfer(int it, double* Ablock, double* AMatrixblock) {
  int main = (coords[0] + it) % cartSize;
  if (coords[1] == main) {
    for (int i = 0; i < blockSize * blockSize; ++i) Ablock[i] = AMatrixblock[i];
  }
  MPI_Bcast(Ablock, blockSize * blockSize, MPI_DOUBLE, main, comm_row);
}

void BlockMultiplication(double* Ablock, double* Bblock, double* Cblock,
                         int _blockSize) {
  double temp;
  for (int i = 0; i < _blockSize; i++)
    for (int j = 0; j < _blockSize; j++) {
      temp = 0;
      for (int k = 0; k < _blockSize; k++)
        temp += Ablock[i * _blockSize + k] * Bblock[k * _blockSize + j];
      Cblock[i * _blockSize + j] += temp;
    }
}

void BblockTransfer(double* Bblock) {
  MPI_Status status;
  int dest = coords[0] - 1;
  if (coords[0] == 0) dest = cartSize - 1;
  int source = coords[0] + 1;
  if (coords[0] == cartSize - 1) source = 0;
  MPI_Sendrecv_replace(Bblock, blockSize * blockSize, MPI_DOUBLE, dest, 0,
                       source, 0, comm_col, &status);
}

void FoxAlg(double* Ablock, double* AMatrixblock, double* Bblock,
            double* Cblock) {
  for (int it = 0; it < cartSize; ++it) {
    ABlockTransfer(it, Ablock, AMatrixblock);
    BlockMultiplication(Ablock, Bblock, Cblock, blockSize);
    BblockTransfer(Bblock);
  }
}

void Topology() {
  int dimSize[2] = {cartSize, cartSize};
  int periods[2] = {false, false};
  MPI_Cart_create(MPI_COMM_WORLD, ndims, dimSize, periods, reoder, &comm_cart);
  MPI_Cart_coords(comm_cart, rank, ndims, coords);

  int subdims[2];
  subdims[0] = false;
  subdims[1] = true;
  MPI_Cart_sub(comm_cart, subdims, &comm_row);

  subdims[0] = true;
  subdims[1] = false;
  MPI_Cart_sub(comm_cart, subdims, &comm_col);
}

void Fox(double* A, double* B, double* C, int size) {
  if (size <= 0) throw std::runtime_error("Wrong size matrix");

  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (procNum == 1) {
    SequentialAlgorithm(A, B, C, size);
  } else {
    cartSize = static_cast<int>(std::sqrt(procNum));
    if (cartSize * cartSize != procNum)
      throw std::runtime_error("Wrong number of processes");
    if (size % cartSize != 0) throw std::runtime_error("Wrong size matrix");
    blockSize = size / cartSize;

    int numElemBlock = blockSize * blockSize;
    double* Ablock = new double[numElemBlock];
    double* Bblock = new double[numElemBlock];
    double* Cblock = new double[numElemBlock];
    double* AMatrixblock = new double[numElemBlock];
    for (int i = 0; i < numElemBlock; i++) Cblock[i] = 0;

    Topology();

    BlockExport(A, B, AMatrixblock, Bblock, size);

    FoxAlg(Ablock, AMatrixblock, Bblock, Cblock);

    ImportResult(C, Cblock, size);

    delete[] Ablock;
    delete[] Bblock;
    delete[] Cblock;
    delete[] AMatrixblock;
  }
}

void SequentialAlgorithm(double* A, double* B, double* C, int size) {
  if (size <= 0) throw std::runtime_error("Wrong size matrix");
  for (int i = 0; i < size * size; ++i) C[i] = 0;
  BlockMultiplication(A, B, C, size);
}

void RandomOperandMatrix(double* A, double* B, int size) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  for (int i = 0; i < size * size; ++i) {
    A[i] = gen() % 5 + static_cast<float>(gen() % 10) / 10;
    B[i] = gen() % 5 + static_cast<float>(gen() % 10) / 10;
  }
}

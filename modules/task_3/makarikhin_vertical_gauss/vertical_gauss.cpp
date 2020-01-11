// Copyright 2019 Makarikhin Semen
#include <mpi.h>
#include <iostream>
#include <random>
#include <numeric>
#include <algorithm>
#include <vector>
#include <ctime>
#include <list>
#include "../../../modules/task_3/makarikhin_vertical_gauss/vertical_gauss.h"

std::vector<unsigned int> Rand_Img(int cols, int rows) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<unsigned int> img(cols * rows);
  for (auto& i : img) {
    i = gen() % 256;
  }
  return img;
}

unsigned int Pixel(std::vector<unsigned int> vec, int a, int b, int rows, int cols) {
  int sum = 0;
  int sumMask = 16;
  for (int i = -1; i < 2; ++i) {
    for (int j = -1; j < 2; ++j) {
      int x = a + i;
      int y = b + j;

      if (x < 0 || x > rows - 1) {
        x = a;
      }
      if (y < 0 || y > cols - 1) {
        y = b;
      }
      sum += vec[x + y * rows] * gaussFilter[i + 1][j + 1];
    }
  }
  return static_cast<int>(sum / sumMask);
}

std::vector<unsigned int> fil_img_sequential(std::vector<unsigned int> source, int rows, int cols) {
  std::vector<unsigned int> result(cols * rows);
  for (int i = 0; i < cols; ++i)
    for (int j = 0; j < rows; ++j) {
      result[i * cols + j] = Pixel(source, j, i, rows, cols);
    }
  return result;
}

std::vector<unsigned int> fil_img_parall(std::vector<unsigned int> source, int rows, int cols) {
  int size, rank;
  std::vector<unsigned int> globalResult(cols * rows);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (cols < size || size == 1) {
    if (rank == 0)
      globalResult = fil_img_sequential(source, rows, cols);
    return globalResult;
  }

  const int len_bloc = rows / size;
  const int last_bloc = rows / size + rows % size;

  std::vector<unsigned int> localImage;
  std::vector<unsigned int> localResult;
  if (rank == size - 1) {
    localImage.resize((last_bloc + 1) * rows);
    localResult.resize(last_bloc * rows);
  } else {
    localImage.resize((len_bloc + 2) * rows);
    localResult.resize(len_bloc * rows);
  }

  MPI_Datatype MPI_BLOCK, MPI_LAST_BLOCK;

  MPI_Type_vector(rows, len_bloc + 2, rows, MPI_INT, &MPI_BLOCK);
  MPI_Type_commit(&MPI_BLOCK);
  MPI_Type_vector(rows, last_bloc + 1, rows, MPI_INT, &MPI_LAST_BLOCK);
  MPI_Type_commit(&MPI_LAST_BLOCK);

  if (rank == 0) {
    localImage.resize((len_bloc + 1) * rows);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < len_bloc + 1; j++) {
        localImage[i * (len_bloc + 1) + j] = source[j + cols * i];
      }
    }
    for (int i = 1; i < size - 1; i++) {
      MPI_Send(source.data() + (i)*len_bloc - 1, 1, MPI_BLOCK, i, 0, MPI_COMM_WORLD);
    }
    MPI_Send(source.data() + (size - 1) * len_bloc - 1, 1, MPI_LAST_BLOCK, size - 1, 0, MPI_COMM_WORLD);
  } else {
    MPI_Status status;
    if (rank != size - 1) {
      MPI_Recv(&localImage.front(), rows * (len_bloc + 2), MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    } else {
      MPI_Recv(&localImage.front(), rows * (last_bloc + 1), MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
  }
  if (rank != 0 && rank < size - 1) {
    for (int i = 0; i < cols; i++) {
      for (int j = 0; j < len_bloc; j++) {
        localResult[i * len_bloc + j] = Pixel(localImage, j + 1, i, len_bloc + 2, cols);
      }
    }
  }
  if (rank == 0) {
    for (int i = 0; i < cols; i++) {
      for (int j = 0; j < len_bloc; j++) {
        localResult[i * len_bloc + j] = Pixel(localImage, j, i, len_bloc + 1, cols);
      }
    }
  }
  if (rank == size - 1) {
    for (int i = 0; i < cols; i++) {
      for (int j = 0; j < last_bloc; j++) {
        localResult[i * last_bloc + j] = Pixel(localImage, j + 1, i, last_bloc + 1, cols);
      }
    }
  }

  int* sendcounts = new int[size];
  int* displs = new int[size];

  for (int i = 0; i < size; i++) {
    displs[i] = 0;
    if (i < size - 1) {
      sendcounts[i] = len_bloc * rows;
    } else {
      sendcounts[i] = last_bloc * rows;
    }
    if (i > 0) {
      displs[i] = displs[i - 1] + sendcounts[i - 1];
    }
  }

  std::vector<unsigned int> tmp(source.size());

  MPI_Gatherv(localResult.data(), sendcounts[rank], MPI_INT, &tmp.front(),
    sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    for (int i = 0; i < size; i++)
      for (int j = 0; j < rows; j++) {
        if (i != size - 1) {
          for (int k = 0; k < len_bloc; k++) {
            globalResult[j * cols + i * len_bloc + k] = tmp[i * rows * len_bloc + j * len_bloc + k];
          }
        } else {
          for (int k = 0; k < last_bloc; k++) {
            globalResult[j * cols + i * len_bloc + k] = tmp[i * rows * len_bloc + j * last_bloc + k];
          }
        }
      }
  }
  return globalResult;
}

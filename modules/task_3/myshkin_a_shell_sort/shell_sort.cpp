// Copyright 2019 Myshkin Andrew
#include <mpi.h>
#include <vector>
#include <random>
#include <stdexcept>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <utility>
#include "../../../modules/task_3/myshkin_a_shell_sort/shell_sort.h"

int* getRandomArray(int sizeA) {
  if (sizeA <= 0)
    throw std::runtime_error("Error Array size");
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  int *localBuf = reinterpret_cast<int*>(malloc(sizeof(int) * sizeA));
  for (int i = 0; i < sizeA; i++) {
    localBuf[i] = gen() % 100;
  }
  return localBuf;
}

int getMinArray(int* buffer, int sizeA) {
  int min = buffer[0];
  for (int i = 1; i < sizeA; i++) {
    if (min > buffer[i]) {
      min = buffer[i];
    }
  }
  return min;
}


int* ShellSortSenq(int* buffer, int length) {
  int i, j, temp;
  int step = length / 2;

  while (step > 0) {
    for (i = 0; i < (length - step); i++) {
      j = i;
      while (j >= 0 && buffer[j] > buffer[j + step]) {
        temp = buffer[j];
        buffer[j] = buffer[j + step];
        buffer[j + step] = temp;
        j--;
      }
    }
    step = step / 2;
  }
  return buffer;
}

void ownMergeBound(int *buffer, unsigned int left, unsigned int right, unsigned int middle) {
  if (left >= right || middle < left || middle > right) return;
  if (right == left + 1 && buffer[left] > buffer[right]) {
    int value = buffer[right];
    buffer[right] = buffer[left];
    buffer[left] = value;
    return;
  }
  int *tmp = reinterpret_cast<int*>(malloc(((right - left) + 1) * sizeof(int)));
  if (tmp == nullptr) return;
  for (unsigned int i = 0; i < ((right - left) + 1); i++) tmp[i] = buffer[left + i];

  for (unsigned int i = left, j = 0, k = (middle - left + 1); i <= right; ++i) {
    if (j > middle - left) {
      buffer[i] = tmp[k++];
    } else if (k > right - left) {
      buffer[i] = tmp[j++];
    } else {
      buffer[i] = (tmp[j] < tmp[k]) ? tmp[j++] : tmp[k++];
    }
  }
  if (tmp) { free(tmp); tmp = nullptr; }

  return;
}

void ownMergeSort(int *buffer, unsigned int left, unsigned int right) {
  if (left >= right) return;

  unsigned int middle = left + (right - left) / 2;

  ownMergeSort(buffer, left, middle);
  ownMergeSort(buffer, (middle + 1), right);
  ownMergeBound(buffer, left, right, middle);
  return;
}

int mergeSort(int *buffer, unsigned int size) {
  if ((buffer == nullptr) || (size < 2)) return -1;
  unsigned int left = 0, right = size - 1;

  ownMergeSort(buffer, left, right);
  return 0;
}

int* parallelShellSort(int* buffer, int length) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int sizeBuf = length;

  if (sizeBuf <= 0) {
    throw std::runtime_error("Error size of array");
  }

  if (sizeBuf == 1) {
    return buffer;
  }

  int sts = 0;
  int step = sizeBuf / 2;
  int sizeP, residue, sizeProc;
  int counter = 0;

  while (step > 0) {
    residue = step % size;
    if (size > step) {
      sizeP = step;
    } else {
      sizeP = size;
    }

    sizeProc = sizeBuf / step;
    int *bufForProc = reinterpret_cast<int*>(malloc(sizeProc * sizeof(int)));

    counter = 0;

    while (counter <= step + sizeP - residue || counter <= step) {
      if (counter + residue == step) { sizeP = residue; }
      for (int proc = 0; proc < sizeP; proc++) {
        if (rank == 0) {
          for (int i = 0; i < sizeProc; i++) {
            bufForProc[i] = buffer[proc + counter + step * i];
          }
          if (proc == 0) {
            sts = mergeSort(bufForProc, sizeProc);
            if (sts == -1) { throw std::runtime_error("Error in Merge Sort"); }
            for (int i = 0; i < sizeProc; i++) {
              buffer[counter + step * i] = bufForProc[i];
            }
          } else {
            MPI_Send(&bufForProc[0], sizeProc, MPI_INT, proc, 0, MPI_COMM_WORLD);
          }
        } else {
          if (rank == proc) {
            MPI_Status Status;
            MPI_Recv(&bufForProc[0], sizeProc, MPI_INT, 0, 0, MPI_COMM_WORLD, &Status);
            sts = mergeSort(bufForProc, sizeProc);
            if (sts == -1) { throw std::runtime_error("Error in Merge Sort"); }
            MPI_Send(&bufForProc[0], sizeProc, MPI_INT, 0, proc, MPI_COMM_WORLD);
          }
        }
      }
      counter = counter + size;
    }

    if (size > step) {
      sizeP = step;
    } else {
      sizeP = size;
    }

    counter = 0;

    if (rank == 0) {
      while (counter <= step + sizeP - residue || counter <= step) {
        if (counter + residue == step) { sizeP = residue; }
        for (int proc = 1; proc < sizeP; proc++) {
          MPI_Status Status;
          MPI_Recv(&bufForProc[0], sizeProc, MPI_INT, proc,
            proc, MPI_COMM_WORLD, &Status);
          for (int i = 0; i < sizeProc; i++) {
            buffer[proc + step * i + counter] = bufForProc[i];
          }
        }
        counter = counter + size;
      }
    }
    step = step / 2;
  }
  return buffer;
}

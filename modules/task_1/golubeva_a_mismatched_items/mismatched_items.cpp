// Copyright 2019 Golubeva Anna
#include "../../../modules/task_1/golubeva_a_mismatched_items/mismatched_items.h"
#include <mpi.h>
#include <random>
#include <cstring>

void getRandomStr(char *str, int size) {
  const char arr[] = "abcdefghijklmnopqrstuvwxyz0123456789";
  if (size <= 0)
    throw "Error size";
  for (int i = 0; i < size; i++) {
    str[i] = arr[rand() % (sizeof(arr) - 1)];
  }
  str[size] = 0;
}

int getCountDiffChar(char *str1, char *str2) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int add, step;
  int remainder;
  add = 0;
  if (strlen(str1) <= strlen(str2)) {
    step = strlen(str1) / size;
    add = strlen(str2) - strlen(str1);
    remainder = strlen(str1) % size;
  } else {
    step = strlen(str2) / size;
    add = strlen(str1) - strlen(str2);
    remainder = strlen(str2) % size;
  }
  int localResult = 0, result = 0;
  char* strLocal1 = new char[step + 1];
  char* strLocal2 = new char[step + 1];
  if (rank == 0) {
    if (step > 0) {
      for (int process = 1; process < size; process++) {
        MPI_Send(&str1[0] + process * step + remainder, step, MPI_CHAR, process, 0, MPI_COMM_WORLD);
        MPI_Send(&str2[0] + process * step + remainder, step, MPI_CHAR, process, 1, MPI_COMM_WORLD);
      }
    }
  }
  if (rank == 0) {
    delete[] strLocal1;
    delete[] strLocal2;
    strLocal1 = new char[step + remainder + 1];
    strLocal2 = new char[step + remainder + 1];
    for (int i = 0; i < step + remainder; i++) {
      strLocal1[i] = str1[i];
      strLocal2[i] = str2[i];
    }
    strLocal1[step + remainder] = 0;
    strLocal2[step + remainder] = 0;
  } else {
    MPI_Status status;
    if (step > 0) {
      MPI_Recv(&strLocal1[0], step, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
      MPI_Recv(&strLocal2[0], step, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
      strLocal1[step] = 0;
      strLocal2[step] = 0;
    }
  }
  localResult = getCount(strLocal1, strLocal2);
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Reduce(&localResult, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  delete[] strLocal1;
  delete[] strLocal2;
  return result + add;
}

int getCount(char* str1, char* str2) {
  int counter = 0;
  for (size_t i = 0; i < strlen(str1); i++) {
    if (str1[i] != str2[i])
      counter++;
  }
  return counter;
}



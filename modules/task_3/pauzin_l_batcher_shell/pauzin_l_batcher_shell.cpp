// Copyright 2019 Pauzin Leonid
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <utility>
#include "../../../modules/task_3/pauzin_l_batcher_shell/pauzin_l_batcher_shell.h"



std::vector <int> ShellSort(const std::vector <int> &vec) {
  int step, i, j, tmp;
  int size = vec.size();
  std::vector <int> resulVec(vec);
  for (step = size / 2; step > 0; step /= 2)
    for (i = step; i < size; i++)
      for (j = i - step; j >= 0 && resulVec[j] > resulVec[j + step]; j -= step) {
        tmp = resulVec[j];
        resulVec[j] = resulVec[j + step];
        resulVec[j + step] = tmp;
      }
  return resulVec;
}

std::vector<int> getRandomVector(int size) {
  std::vector<int> vector(size);
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  for (int i = 0; i < size; i++) {
    vector[i] = gen() % 100;
  }
  return vector;
}

std::vector<int> batcherParallel(std::vector<int> globalVec, int vecSize) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (vecSize <= 0)
    throw "Wrong vector size";

  const int delta = vecSize / size;
  const int remainder = vecSize % size;
  int evenS, oddS, tag, sendLenghtNew;
  std::vector<int> localVec;

  if (vecSize < size) {
    if (rank == 0)
      localVec = ShellSort(globalVec);
    return localVec;
  }

  if (size == 1) {
    localVec = ShellSort(globalVec);
    return localVec;
  }
  if (rank == 0) {
    localVec.resize(delta + remainder);
  } else {
    localVec.resize(delta);
  }

  if (rank == 0) {
    for (int i = 0; i < delta + remainder; i++) {
      localVec[i] = globalVec[i];
    }
    for (int i = 1; i < size; i++)
      MPI_Send(&globalVec[0] + delta*i + remainder, delta, MPI_INT, i, 0, MPI_COMM_WORLD);
  } else {
    MPI_Status status;
    MPI_Recv(&localVec.front(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }

  int count = 1;
  for (count = 1; pow(2, count) < size; count++) {}
  localVec = ShellSort(localVec);
  localVec = permutation(localVec);

  int offset = 1, proc = 2, lengthS, lenghtRcv;
  std::vector<int> promVec;
  std::vector<int> vecOdd;
  std::vector<int> vecEven;
  MPI_Status status;

  for (int i = 0; i < count; i++) {
    if (rank % proc == 0 && rank + offset < size) {
      lengthS = localVec.size() / 2;
      tag = rank + offset;
      MPI_Send(&lengthS, 1, MPI_INT, tag, 0, MPI_COMM_WORLD);
      MPI_Recv(&lenghtRcv, 1, MPI_INT, tag, 0, MPI_COMM_WORLD, &status);

      promVec.resize(lenghtRcv / 2 + lenghtRcv % 2);
      MPI_Send(&localVec[localVec.size() / 2 + localVec.size() % 2], lengthS, MPI_INT, tag, 0, MPI_COMM_WORLD);
      MPI_Recv(&promVec.front(), lenghtRcv / 2 + lenghtRcv % 2, MPI_INT, tag, 0, MPI_COMM_WORLD, &status);

      vecEven = evenFunc(localVec, promVec);
      vecOdd.resize(lenghtRcv / 2 + localVec.size() / 2);
      evenS = vecEven.size();
      oddS = vecOdd.size();

      MPI_Recv(&vecOdd.front(), lenghtRcv / 2 + localVec.size() / 2, MPI_INT,
        tag, 0, MPI_COMM_WORLD, &status);

      localVec.resize(evenS + oddS);
      std::copy(vecEven.begin(), vecEven.end(), localVec.begin());
      std::copy(vecOdd.begin(), vecOdd.end(), localVec.begin() + evenS);

      if (i + 1 == count)
        localVec = merge(localVec, evenS, oddS);
      else
        localVec = transposition(localVec, evenS, oddS);
    }

    if (rank - offset >= 0 && (rank - offset) % proc == 0) {
      lengthS = localVec.size();
      tag = rank - offset;
      MPI_Send(&lengthS, 1, MPI_INT, tag, 0, MPI_COMM_WORLD);
      MPI_Recv(&lenghtRcv, 1, MPI_INT, tag, 0, MPI_COMM_WORLD, &status);

      promVec.resize(lenghtRcv);

      sendLenghtNew = lengthS / 2 + lengthS % 2;
      MPI_Sendrecv(&localVec[0], sendLenghtNew, MPI_INT, tag, 0, &promVec[0], lenghtRcv, MPI_INT,
        tag, 0, MPI_COMM_WORLD, &status);
      vecOdd = oddFunc(localVec, promVec);
      sendLenghtNew = vecOdd.size();
      MPI_Send(&vecOdd[0], sendLenghtNew, MPI_INT, tag, 0, MPI_COMM_WORLD);
    }
    proc *= 2;
    offset *= 2;
  }
  return localVec;
}


std::vector<int> permutation(std::vector<int> localVec) {
  std::vector<int> promVec(localVec.size());

  for (unsigned int i = 0; i < localVec.size() / 2 + localVec.size() % 2; i++) {
    promVec[i] = localVec[2 * i];
  }
  for (unsigned int i = 1; i < localVec.size(); i += 2) {
    promVec[localVec.size() / 2 + localVec.size() % 2 + i / 2] = localVec[i];
  }
  for (unsigned int i = 0; i < localVec.size(); i++) {
    localVec[i] = promVec[i];
  }
  return localVec;
}


std::vector<int> evenFunc(const std::vector<int>& localVec, const std::vector<int>& promVec) {
  int size1 = localVec.size() / 2 + localVec.size() % 2;
  int size2 = promVec.size();
  int size = localVec.size() / 2 + localVec.size() % 2 + promVec.size();
  std::vector <int> result(size);
  int j = 0, k = 0, l = 0;
  while ((j < size1) && (k < size2)) {
    if (localVec[j] < promVec[k]) {
      result[l] = localVec[j];
      j++;
    } else {
      result[l] = promVec[k];
      k++;
    }
    l++;
  }
  if (j >= size1) {
    for (int a = k; a < size2; a++) {
      result[l] = promVec[a];
      l++;
    }
  } else {
    for (int a = j; a < size1; a++) {
      result[l] = localVec[a];
      l++;
    }
  }
  return result;
}

std::vector<int> oddFunc(const std::vector<int>& localVec, const std::vector<int>& promVec) {
  int size1 = localVec.size();
  int size2 = promVec.size();
  int size = localVec.size() / 2 + promVec.size();
  std::vector <int> result(size);
  int j = localVec.size() / 2 + localVec.size() % 2, k = 0, l = 0;
  while ((j < size1) && (k < size2)) {
    if (localVec[j] < promVec[k]) {
      result[l] = localVec[j];
      l++;
      j++;
    } else {
      result[l] = promVec[k];
      k++;
      l++;
    }
  }

  if (j < size1) {
    for (int a =j; a < size1; a++) {
      result[l] = localVec[a];
      l++;
    }
  }
  if (k < size2) {
    for (int t = k; t < size2; t++) {
      result[l] = promVec[t];
      l++;
    }
  }
  return result;
}


std::vector<int> transposition(std::vector<int> localVec, int evenSize, int oddSize) {
  int i;
  if (evenSize - oddSize == 2) {
    std::vector<int> result(localVec.size());
    int a = 0;
    int b = 0;
    int c = 0;
    for (a = 0, b = 0; a < evenSize && b < oddSize; a++, b++) {
      result[c] = localVec[a];
      c++;
      result[c] = localVec[evenSize + b];
      c++;
    }

    for (int t = a; t < evenSize; t++, c++) {
      result[c] = localVec[t];
    }
    int size = result.size();
    for (i = 1; i < size - 1; i += 2) {
      if (result[i] > result[i + 1])
        std::swap(result[i], result[i + 1]);
    }

    result = permutation(result);
    return result;

  } else {
    for (i = 0; i < evenSize - 1; i++)
      if (localVec[1 + i] < localVec[evenSize + i])
        std::swap(localVec[1 + i], localVec[evenSize + i]);
    return localVec;
  }
}

std::vector<int>  merge(std::vector<int> localVec, int evenSize, int oddSize) {
  std::vector<int> result(localVec.size());
  int a = 0;
  int b = 0;
  int c = 0;

  for (a = 0; a < evenSize && b < oddSize; a++, b++) {
    result[c] = localVec[a];
    c++;
    result[c] = localVec[evenSize + b];
    c++;
  }

  for (int t = a; t < evenSize; t++, c++) {
    result[c] = localVec[t];
  }

  int size = result.size();
  for (int i = 1; i < size - 1; i += 2) {
    if (result[i] > result[i + 1])
      std::swap(result[i], result[i + 1]);
  }
  return result;
}

// Copyright 2019 Kukushkina Ksenia
#include <mpi.h>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "../../../modules/task_1/kukushkina_k_lexicograph/lexicograph.h"

std::string generateString(const int length) {
  if (length <= 0)
    throw "Wrong size";
  int n;
  std::string s;
  for (int i = 0; i < length; i++) {
    n = std::rand() % 3;
    switch (n) {
    case 0:
      s += static_cast<char>(std::rand() % 10 + 48);  //  0 - 9
      break;
    case 1:
      s += static_cast<char>(std::rand() % 26 + 65);  //  A - Z
      break;
    case 2:
      s += static_cast<char>(std::rand() % 26 + 97);  //  a - z
      break;
    }
  }
  return s;
}

bool isFragmentOrdered(const std::string source) {
  if (static_cast<int>(source.size()) < 2)
    return true;
  int size = static_cast<int>(source.size());
  for (int i = 1; i < size; i++)
    if (source[i] < source[i - 1]) {
      if ((source[i - 1] != '*') && (source[i] != '*'))
        return false;
    }
  return true;
}

bool isStringOrdered(const std::string source1, const std::string source2) {
  if (source1.empty() || source2.empty()) {
    throw "Empty string";
  }
  std::string source(source1);
  source += '*' + source2 + '*';
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int len = (static_cast<int>(source.size()) - 1) / size;
  int rem = (static_cast<int>(source.size()) - 1) % size;
  bool lres, res;
  if (rank == 0) {
    for (int process = 1; process < size; process++) {
      int start, end;
      start = process * len + rem;
      end = start + len;
      MPI_Send(&start, 1, MPI_INT, process, 3, MPI_COMM_WORLD);
      MPI_Send(&end, 1, MPI_INT, process, 4, MPI_COMM_WORLD);
    }
  }
  std::string str = "";
  int start, end;
  if (rank == 0) {
    str = source.substr(0, len + rem + 1);
  } else {
    MPI_Status status;
    MPI_Recv(&start, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, &status);
    MPI_Recv(&end, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
    for (int i = start; i <= end; i++)
      str += source[i];
  }
  lres = isFragmentOrdered(str);
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Allreduce(&lres, &res, 1, MPI_C_BOOL, MPI_LAND, MPI_COMM_WORLD);
  return res;
}

// Copyright 2019 Koltyushkina Yanina
#include <mpi.h>
#include <string>
#include <random>
#include "../../../modules/task_1/koltyushkina_ya_num_of_elem/num_of_elem.h"

std::string RandomString(int _size) {
  std::string _str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789  ";
  if (_size <= 0)
    throw "Zero or negative size";
  char *s;
  s = new char[_size];
  for (int i = 0; i < _size; i++) {
    s[i] = _str[rand() % _str.size()];
  }
  s[_size] = '\0';
  std::string str = s;
  return str;
}

int GetNumOnPart(std::string str) {
  int num = 0;
  int size = str.size();
  for (int i = 0; i < size; i++) {
    if (str[i] != ' ')
      num++;
  }
  return num;
}

int GetNumOnText(std::string str) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int segm = str.size() / size;
  int ost = str.size() % size;

  if (rank == 0) {
    for (int i = 1; i < size; i++) {
      MPI_Send(str.c_str() + i * segm + ost, segm,
        MPI_CHAR, i, 0, MPI_COMM_WORLD);
    }
  }
  std::string stmp;
  char *chtmp = new char[segm];
  if (rank == 0) {
    stmp.insert(0, str, 0, segm + ost);
  } else {
    MPI_Status status;
    MPI_Recv(&chtmp[0], segm, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    chtmp[segm] = '\0';
    stmp = chtmp;
  }

  int ntmp = 0;
  int num = 0;

  ntmp = GetNumOnPart(stmp);
  MPI_Reduce(&ntmp, &num, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return num;
}

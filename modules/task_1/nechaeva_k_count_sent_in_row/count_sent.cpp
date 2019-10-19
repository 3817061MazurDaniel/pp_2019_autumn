// Copyright 2019 Nechaeva Ekaterina
#include <mpi.h>
#include <string>
#include <random>
#include "../../../modules/task_1/nechaeva_k_count_sent_in_row/count_sent.h"

std::string RandomString(int _size) {
  std::string _str = "0123456789abcdefghijklmnopqrstuvwxyz";
  if (_size <= 0)
    throw "Negative size";
  char *a;
  a = new char[_size];
  for (int i = 0; i < _size; i++) {
    a[i] = _str[rand() % _str.size()];
  }
  a[_size] = '\0';
  std::string str = a;
  for (int i = 1; i < _size; i += 4) {
    str[i] = '.';
  }
  return str;
}

int GetCountInPart(std::string str) {
  int count = 0;
  int j = str.size();
  for (int i = 0; i < j; i++) {
    if (str[i] == '.')
      count++;
  }
  return count;
}

int GetCountInText(std::string str) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int p = str.size() / size;
  int r = str.size() % size;
  int lc = 0;
  int gl = 0;
  if (rank == 0) {
    for (int proc = 1; proc < size; proc++) {
      MPI_Send(str.c_str() + proc * p + r, p,
        MPI_CHAR, proc, 0, MPI_COMM_WORLD);
    }
  }

  std::string lstr;
  char *lch = new char[p];
  if (rank == 0) {
    lstr.insert(0, str, 0, p + r);
  } else {
    MPI_Status status;
    MPI_Recv(&lch[0], p, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    lch[p] = '\0';
    lstr = lch;
    }
  lc = GetCountInPart(lstr);
  MPI_Reduce(&lc, &gl, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return gl;
}

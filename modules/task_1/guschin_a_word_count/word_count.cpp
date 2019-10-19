// Copyright 2019 Guschin Alexander
#include "../../../modules/task_1/guschin_a_word_count/word_count.h"
#include <mpi.h>
#include <string>
#include <vector>

bool isLetter(char sym) {
  return (sym <= 'z' && sym >= 'a') || (sym <= 'Z' && sym >= 'A');
}

int getLinearCount(std::string st, int size) {
  int count = 0;
  bool flag = 0;
  for (int i = 0; i < size; i++) {
    if (isLetter(st[i]) == true) {
      flag = 1;
    } else {
      if (flag == true) count++;
      flag = false;
    }
  }

  if (flag == true) count++;

  return count;
}

int getCount(const std::string st, int st_size) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int local_count = 0;
  int global_count;
  int delta = st_size / size;
  int rem = st_size % size;

  if (rank == 0) {
    for (int i = 1; i < size; i++) {
      MPI_Send(&st[0] + i * delta + rem, delta, MPI_CHAR, i, 0, MPI_COMM_WORLD);
    }
  }

  std::vector<char> local_st(delta);
  if (rank == 0) {
    local_st = std::vector<char>(st.begin(), st.begin() + delta + rem);
    local_count =
        getLinearCount(std::string(&local_st[0], delta + rem), delta + rem);
  } else {
    MPI_Status status;
    MPI_Recv(&local_st[0], delta, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    local_count = getLinearCount(std::string(&local_st[0], delta), delta);
  }


  MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);

  if (rank == 0 && st_size >= size) {
    for (int i = 1; i < size; i++)
      if (isLetter(st[i * delta + rem - 1]) && isLetter(st[i * delta + rem]))
        global_count--;
  }
  return global_count;
}

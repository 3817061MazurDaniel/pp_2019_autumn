// Copyright 2019 Mityagina Daria
#include "../../../modules/task_2/mityagina_d_reduce/reduce.h"
#include <math.h>
#include <stdio.h>
#include <cstring>
#include <ctime>
#include <random>
#include <algorithm>
#include <stdexcept>

void Reduce(
  void *where_to_send_from,
  void *where_to_send_to,
  int how_much,
  MPI_Datatype datatype,
  func op,
  int root,
  MPI_Comm comm
  ) {
  MPI_Status status;
  int rank_came, num_of_proc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_came);
  MPI_Comm_size(MPI_COMM_WORLD, &num_of_proc);

  int rank = rank_came, num = num_of_proc;
  int ost = 0, type_size, step = 1;

  while (num > 1) {
    if (rank < num) {
      ost = num % 2;
      if (rank % 2 != 0) {
        MPI_Send(where_to_send_from, how_much, datatype, (rank - 1) * step, 19, comm);
        rank *= num;
        break;
      } else {
        if (rank != (num - 1)) {
          MPI_Recv(where_to_send_to, how_much, datatype, (rank + 1) * step, 19, comm, &status);
          op(where_to_send_to, where_to_send_from, how_much);
        }
        rank /= 2;
      }
      step *= 2;
    }
    num = num / 2 + ost;
  }
  if (rank_came == 0) {
    MPI_Type_size(datatype, &type_size);
    memcpy(where_to_send_to, where_to_send_from, how_much * type_size);
  }
}

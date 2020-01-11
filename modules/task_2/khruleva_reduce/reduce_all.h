// Copyright 2019 Khruleva Anastasia

#ifndef MODULES_TASK_2_KHRULEVA_REDUCE_REDUCE_ALL_H_
#define MODULES_TASK_2_KHRULEVA_REDUCE_REDUCE_ALL_H_

#include <mpi.h>

int MPI_Reduce_Gather(const void *sbuf, void *rbuf, int count, MPI_Datatype datatype, MPI_Op op,
  int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_KHRULEVA_REDUCE_REDUCE_ALL_H_

// Copyright 2019 Astafeva Irina

#ifndef MODULES_TASK_2_ASTAFEVA_I_MPI_REDUCE_MPI_REDUCE_H_
#define MODULES_TASK_2_ASTAFEVA_I_MPI_REDUCE_MPI_REDUCE_H_

#include <mpi.h>

int MPI_Reduce_User(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op,
    int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_ASTAFEVA_I_MPI_REDUCE_MPI_REDUCE_H_

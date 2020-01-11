// Copyright 2019 Maximova Irina
#ifndef MODULES_TASK_2_MAXIMOVA_I_ALLREDUCE_ALLREDUCE_H_
#define MODULES_TASK_2_MAXIMOVA_I_ALLREDUCE_ALLREDUCE_H_

#include <mpi.h>

int Allreduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
              MPI_Op op, MPI_Comm comm);

#endif  // MODULES_TASK_2_MAXIMOVA_I_ALLREDUCE_ALLREDUCE_H_

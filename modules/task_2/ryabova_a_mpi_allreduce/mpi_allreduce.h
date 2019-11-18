// Copyright 2019 Ryabova Alyona
#ifndef MODULES_TASK_2_RYABOVA_A_MPI_ALLREDUCE_MPI_ALLREDUCE_H_
#define MODULES_TASK_2_RYABOVA_A_MPI_ALLREDUCE_MPI_ALLREDUCE_H_

#include <mpi.h>

int MPI_Allreduce_c(void* sendbuf, void* recvbuf, int count,
    MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);

#endif  // MODULES_TASK_2_RYABOVA_A_MPI_ALLREDUCE_MPI_ALLREDUCE_H_

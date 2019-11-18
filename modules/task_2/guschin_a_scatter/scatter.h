// Copyright 2019 Guschin Alexander
#ifndef MODULES_TASK_2_GUSCHIN_A_SCATTER_SCATTER_H_
#define MODULES_TASK_2_GUSCHIN_A_SCATTER_SCATTER_H_
#include <mpi.h>
#include <string>
#include <cstring>
#include <vector>

int MPI_Scatter_custom(void* sendbuf, int sendcount, MPI_Datatype sendtype,
                       void* recvbuf, int recvcount, MPI_Datatype recvtype,
                       int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_GUSCHIN_A_SCATTER_SCATTER_H_

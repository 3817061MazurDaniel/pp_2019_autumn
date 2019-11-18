// Copyright 2019 Shashkin Evgeny
#ifndef MODULES_TASK_2_SHASHKIN_E_GATHER_GATHER_H_
#define MODULES_TASK_2_SHASHKIN_E_GATHER_GATHER_H_

#include <mpi.h>

int Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount,
  MPI_Datatype recvtype, int root, MPI_Comm comm);


#endif  // MODULES_TASK_2_SHASHKIN_E_GATHER_GATHER_H_

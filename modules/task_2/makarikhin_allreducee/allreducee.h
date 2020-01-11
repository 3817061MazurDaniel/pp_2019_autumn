// Copyright 2019 Makarikhin Semen
#ifndef MODULES_TASK_2_MAKARIKHIN_ALLREDUCEE_ALLREDUCEE_H_
#define MODULES_TASK_2_MAKARIKHIN_ALLREDUCEE_ALLREDUCEE_H_

#include <mpi.h>

int Allreduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
  MPI_Op op, MPI_Comm comm);

#endif  // MODULES_TASK_2_MAKARIKHIN_ALLREDUCEE_ALLREDUCEE_H_

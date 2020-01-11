#ifndef MODULES_TASK_2_KORNEV_N_BROADCAST_BROADCAST_H_
#define MODULES_TASK_2_KORNEV_N_BROADCAST_BROADCAST_H_
// Copyright 2019 Kornev Nikita

#include "./mpi.h"

void broadcast(void* buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_KORNEV_N_BROADCAST_BROADCAST_H_

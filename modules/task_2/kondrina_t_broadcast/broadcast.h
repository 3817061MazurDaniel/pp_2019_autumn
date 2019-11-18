// Copyright 2019 Kondrina Tatyana

#ifndef MODULES_TASK_2_KONDRINA_T_BROADCAST_BROADCAST_H_
#define MODULES_TASK_2_KONDRINA_T_BROADCAST_BROADCAST_H_

#include <mpi.h>

void bcast(void *buf, int count, MPI_Datatype datatype, int root,
           MPI_Comm comm);

#endif  // MODULES_TASK_2_KONDRINA_T_BROADCAST_BROADCAST_H_

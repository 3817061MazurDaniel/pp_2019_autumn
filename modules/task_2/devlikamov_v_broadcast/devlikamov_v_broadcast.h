// Copyright 2019 Devlikamov V
#ifndef MODULES_TASK_2_DEVLIKAMOV_V_BROADCAST_DEVLIKAMOV_V_BROADCAST_H_
#define MODULES_TASK_2_DEVLIKAMOV_V_BROADCAST_DEVLIKAMOV_V_BROADCAST_H_

#include <mpi.h>

void my_bcast_slow(void *data, int count, MPI_Datatype datatype, int root,
                MPI_Comm communicator);

double loga(double a, double b);

void my_bcast(void *data, int count, MPI_Datatype datatype, int root,
                MPI_Comm communicator);

#endif  // MODULES_TASK_2_DEVLIKAMOV_V_BROADCAST_DEVLIKAMOV_V_BROADCAST_H_

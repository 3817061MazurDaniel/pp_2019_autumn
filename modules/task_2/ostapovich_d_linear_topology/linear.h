// Copyright 2019 Ostapovich Denis

#ifndef MODULES_TASK_2_OSTAPOVICH_D_LINEAR_TOPOLOGY_LINEAR_H_
#define MODULES_TASK_2_OSTAPOVICH_D_LINEAR_TOPOLOGY_LINEAR_H_
#include <mpi.h>

const int NUM_OF_DIMENSIONS = 1;

MPI_Comm createBus(MPI_Comm baseComm);
bool isBus(MPI_Comm comm);
void pingBus(MPI_Comm comm);

#endif  // MODULES_TASK_2_OSTAPOVICH_D_LINEAR_TOPOLOGY_LINEAR_H_

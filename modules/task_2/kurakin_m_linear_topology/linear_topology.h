// Copyright 2019 Kurakin Mikhail
#ifndef MODULES_TASK_2_KURAKIN_M_LINEAR_TOPOLOGY_LINEAR_TOPOLOGY_H_
#define MODULES_TASK_2_KURAKIN_M_LINEAR_TOPOLOGY_LINEAR_TOPOLOGY_H_

#include <mpi.h>

MPI_Comm createLinearComm(MPI_Comm old, int size = 0);
bool testLinearTopology(MPI_Comm commLinear);
bool isLinearTopology(MPI_Comm new_comm);

#endif  // MODULES_TASK_2_KURAKIN_M_LINEAR_TOPOLOGY_LINEAR_TOPOLOGY_H_

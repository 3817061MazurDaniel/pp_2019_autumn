// Copyright 2019 Andronov Maxim
#ifndef MODULES_TASK_2_ANDRONOV_M_RING_TOPOLOGY_RING_TOPOLOGY_H_
#define MODULES_TASK_2_ANDRONOV_M_RING_TOPOLOGY_RING_TOPOLOGY_H_

#include <mpi.h>
#include <vector>

MPI_Comm CreateRingTopology(MPI_Comm oldcomm);

bool IsRingTopology(MPI_Comm comm);

std::vector<int> Send(MPI_Comm ringcomm, int source, int dest, std::vector<int> message, int mess_size);

#endif  // MODULES_TASK_2_ANDRONOV_M_RING_TOPOLOGY_RING_TOPOLOGY_H_

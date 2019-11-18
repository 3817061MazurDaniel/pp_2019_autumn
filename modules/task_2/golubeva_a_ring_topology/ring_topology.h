//  Copyright 2019 Golubeva Anna
#ifndef MODULES_TASK_2_GOLUBEVA_A_RING_TOPOLOGY_RING_TOPOLOGY_H_
#define MODULES_TASK_2_GOLUBEVA_A_RING_TOPOLOGY_RING_TOPOLOGY_H_
#include <mpi.h>

MPI_Comm createRingComm(MPI_Comm old, int size = 0);
bool testRingTopology(MPI_Comm commRing);
bool isRingTopology(MPI_Comm new_comm);

#endif  // MODULES_TASK_2_GOLUBEVA_A_RING_TOPOLOGY_RING_TOPOLOGY_H_


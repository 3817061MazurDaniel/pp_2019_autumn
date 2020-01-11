// Copyright 2019 Pinaev Danil
#ifndef MODULES_TASK_2_PINAEV_D_STAR_TOPOLOGY_STAR_TOPOLOGY_H_
#define MODULES_TASK_2_PINAEV_D_STAR_TOPOLOGY_STAR_TOPOLOGY_H_

#include <mpi.h>

MPI_Comm createStarComm(MPI_Comm old);
bool isStarTopology(MPI_Comm new_comm);

#endif  // MODULES_TASK_2_PINAEV_D_STAR_TOPOLOGY_STAR_TOPOLOGY_H_

// Copyright 2019 Vlasov Andrey
#ifndef MODULES_TASK_2_VLASOV_A_STAR_TOPOLOGY_STAR_TOPOLOGY_H_
#define MODULES_TASK_2_VLASOV_A_STAR_TOPOLOGY_STAR_TOPOLOGY_H_

#include <mpi.h>

MPI_Comm createStarComm(const MPI_Comm oldcomm);
bool isStarTopology(const MPI_Comm StarComm);
bool testStarTopology(const MPI_Comm StarComm);

#endif  // MODULES_TASK_2_VLASOV_A_STAR_TOPOLOGY_STAR_TOPOLOGY_H_

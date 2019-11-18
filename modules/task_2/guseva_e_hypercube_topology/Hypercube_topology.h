// Copyright 2019 Guseva Catherine
#ifndef MODULES_TASK_2_GUSEVA_E_HYPERCUBE_TOPOLOGY_HYPERCUBE_TOPOLOGY_H_
#define MODULES_TASK_2_GUSEVA_E_HYPERCUBE_TOPOLOGY_HYPERCUBE_TOPOLOGY_H_

#include <mpi.h>

MPI_Comm createHypercube(int tempDims, int sizeHypercube);
bool testHypercubeTopDATA(MPI_Comm Comm, int tempDims, int sizeHypercube);
bool isHypercubeTop(MPI_Comm isComm, int tempDims, int sizeHypercube);

#endif  // MODULES_TASK_2_GUSEVA_E_HYPERCUBE_TOPOLOGY_HYPERCUBE_TOPOLOGY_H_

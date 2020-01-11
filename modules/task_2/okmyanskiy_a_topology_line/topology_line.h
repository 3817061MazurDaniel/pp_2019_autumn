// Copyright 2019 Okmyanskiy Andrey
#ifndef MODULES_TASK_2_OKMYANSKIY_A_TOPOLOGY_LINE_TOPOLOGY_LINE_H_
#define MODULES_TASK_2_OKMYANSKIY_A_TOPOLOGY_LINE_TOPOLOGY_LINE_H_

#include <mpi.h>
#include <vector>

std::vector<int> getMessage(int ProcRank, int ProcNum, MPI_Comm MPI_COMM, int sizeVector);
MPI_Comm getLineComm(MPI_Comm oldComm, int ProcNum);
bool isLineTopology(MPI_Comm LineComm, int ProcRank);

#endif  // MODULES_TASK_2_OKMYANSKIY_A_TOPOLOGY_LINE_TOPOLOGY_LINE_H_

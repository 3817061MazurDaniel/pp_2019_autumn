// Copyright 2019 Soboleva Julia
#ifndef MODULES_TASK_2_SOBOLEVA_J_HYPERCUBE_HYPERCUBE_H_
#define MODULES_TASK_2_SOBOLEVA_J_HYPERCUBE_HYPERCUBE_H_

#include <mpi.h>

int getDimension(int nodes);
int getCountVertex(int d);
MPI_Comm getHypercube(int dim);


#endif  // MODULES_TASK_2_SOBOLEVA_J_HYPERCUBE_HYPERCUBE_H_

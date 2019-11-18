// Copyright 2019 Iamshchikov Ivan
#ifndef MODULES_TASK_2_IAMSHCHIKOV_I_TOR_NET_TOPOLOGY_TOR_NET_TOPOLOGY_H_
#define MODULES_TASK_2_IAMSHCHIKOV_I_TOR_NET_TOPOLOGY_TOR_NET_TOPOLOGY_H_

#include <mpi.h>
#include <vector>
#include <cstdlib>

int getNodenumInNetworkTopology(const std::vector<int> dims);
MPI_Comm createTorusTopology(const std::vector<int> dims,
                             const MPI_Comm comm_base = MPI_COMM_WORLD);
bool isNeighbor(MPI_Comm comm_cart, std::vector<int> dims, int ndims,
                int root, int dest);
int getDirection(int coord_root, int coord_dest, int dim_size);
int getStep(int dir);
int getNextDest(std::vector<int> curr_coord, int curr_dim,
                  int step, MPI_Comm comm);
std::vector<int> getTransit(int root, int dest, std::vector<int> dims,
                       MPI_Comm comm_cart, int procrank);
void send(std::vector<int>* pdata, int count, MPI_Datatype datatype,
          int root, int dest, std::vector<int> dims, MPI_Comm comm_cart);

#endif  // MODULES_TASK_2_IAMSHCHIKOV_I_TOR_NET_TOPOLOGY_TOR_NET_TOPOLOGY_H_

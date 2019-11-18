// Copyright 2019 Pinaev Danil
#include "../../../modules/task_2/pinaev_d_star_topology/star_topology.h"
#include <mpi.h>

// make star topo comm from old comm
MPI_Comm createStarComm(MPI_Comm old) {
    int size;
    MPI_Comm_size(old, &size);

    int nnodes = size;

    int* index = new int[nnodes];
    for (int i = 0, j = nnodes - 1; i < nnodes; i++, j++)
        index[i] = j;

    int* edges = new int[(nnodes - 1) * 2];
    for (int i = 0; i < nnodes - 1; i++)
        edges[i] = i + 1;
    for (int i = nnodes - 1; i < (nnodes - 1) * 2; i++)
        edges[i] = 0;

    int reorder = 0;
    MPI_Comm commStar;
    MPI_Graph_create(old, nnodes, index, edges, reorder, &commStar);

    return commStar;
}

bool isStarTopology(MPI_Comm cur_comm) {
    int topo_type;
    MPI_Topo_test(cur_comm, &topo_type);
    if (topo_type != MPI_GRAPH)
        return false;

    int nnodes, nedges;
    MPI_Graphdims_get(cur_comm, &nnodes, &nedges);

    int index_size = nnodes;
    int edges_size = 2 * (nnodes - 1);
    int* index = new int[index_size];
    int* edges = new int[edges_size];
    MPI_Graph_get(cur_comm, index_size, edges_size, index, edges);

    for (int i = 0, j = nnodes - 1; i < index_size; i++, j++)
        if (index[i] != j)
            return false;

    for (int i = 0, j = 1; i < edges_size / 2; i++, j++)
        if (edges[i] != j)
            return false;

    for (int i = nnodes - 1; i < edges_size; i++)
        if (edges[i] != 0)
            return false;

    return true;
}

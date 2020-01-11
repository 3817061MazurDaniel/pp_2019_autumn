// Copyright 2019 Savkin Yuriy
#include <mpi.h>
#include <stdexcept>
#include "../../../modules/task_2/savkin_y_grid-torus_topology/grid-torus_topology.h"

MPI_Comm getTorusComm(const MPI_Comm old, int height, int width) {
    int oldsize;
    MPI_Comm_size(old, &oldsize);

    int dims[] = { height, width };
    int mul = 1;
    if (height > 0)
        mul *= height;
    if (width > 0)
        mul *= width;
    if (oldsize % mul != 0)
        return MPI_COMM_NULL;

    MPI_Dims_create(oldsize, 2, dims);

    MPI_Comm torus_comm;
    int periods[] = {1, 1};

    MPI_Cart_create(old, 2, dims, periods, 1, &torus_comm);

    return torus_comm;
}

bool testTorusTopology(const MPI_Comm t_comm) {
    int topology;
    MPI_Topo_test(t_comm, &topology);
    if (topology != MPI_CART)
        return false;

    int dims_count;
    MPI_Cartdim_get(t_comm, &dims_count);
    if (dims_count != 2)
        return false;

    int dims[2];
    int periods[2];
    int coords[2];
    MPI_Cart_get(t_comm, 2, dims, periods, coords);
    if ((periods[0] != 1) || (periods[1] != 1))
        return false;
    return true;
}

bool testCommunications(const MPI_Comm t_comm) {
    int rank;
    MPI_Comm_rank(t_comm, &rank);

    int coords[2];
    MPI_Cart_coords(t_comm, rank, 2, coords);

    int flag = 0;
    int front, back;
    MPI_Cart_shift(t_comm, 0, 1, &back, &front);
    if ((back == MPI_PROC_NULL) || (front == MPI_PROC_NULL))
       flag = 1;

    MPI_Cart_shift(t_comm, 1, 1, &back, &front);
    if ((back == MPI_PROC_NULL) || (front == MPI_PROC_NULL))
        flag = 1;

    if (rank == 0) {
        int res = 0;
        MPI_Reduce(&flag, &res, 1, MPI_INT, MPI_LOR, 0, t_comm);
        if (res == 0)
            return true;
    } else {
        MPI_Reduce(&flag, NULL, 1, MPI_INT, MPI_LOR, 0, t_comm);
    }
    return false;
}


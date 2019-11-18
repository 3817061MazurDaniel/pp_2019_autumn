// Copyright 2019 Kurakin Mikhail
#include "../../../modules/task_2/kurakin_m_linear_topology/linear_topology.h"
#include <mpi.h>
#define LINEARTOPOLEN 1
MPI_Comm createLinearComm(MPI_Comm old, int size) {
    int reorder = 0;
    int dims[LINEARTOPOLEN]{0}, periods[LINEARTOPOLEN]{0};
    int oldSize;
    MPI_Comm_size(old, &oldSize);
    if (size <= 0 || size > oldSize)
        MPI_Dims_create(oldSize, LINEARTOPOLEN, dims);
    else
        MPI_Dims_create(size, LINEARTOPOLEN, dims);
    MPI_Comm commLinear;
    MPI_Cart_create(MPI_COMM_WORLD, LINEARTOPOLEN, dims, periods, reorder, &commLinear);
    return commLinear;
}

bool testLinearTopology(MPI_Comm commLinear) {
    if (!isLinearTopology(commLinear)) return false;
    int rank, size;
    int sourceLess, sourceBig, destLess, destBig;
    int new_coords[LINEARTOPOLEN];
    int val, valFromLess, valFromBig;
    MPI_Status status;
    MPI_Comm_rank(commLinear, &rank);
    MPI_Comm_size(commLinear, &size);
    MPI_Cart_coords(commLinear, rank, LINEARTOPOLEN, new_coords);
    val = new_coords[0];
    valFromLess = -1;
    valFromBig = -1;
    MPI_Cart_shift(commLinear, 0, 1, &sourceLess, &destBig);
    MPI_Cart_shift(commLinear, 0, -1, &sourceBig, &destLess);
    MPI_Sendrecv(&val, 1, MPI_INT, destBig, 4, &valFromLess, 1, MPI_INT,
                 sourceLess, 4, commLinear, &status);
    MPI_Sendrecv(&val, 1, MPI_INT, destLess, 4, &valFromBig, 1, MPI_INT,
                 sourceBig, 4, commLinear, &status);
    MPI_Comm_free(&commLinear);

    if ((rank + 1) != valFromBig) {
        if (new_coords[0] != size - 1) return false;
    }
    if ((rank - 1) != valFromLess) {
        if (new_coords[0] != 0) return false;
    }
    return true;
}

bool isLinearTopology(MPI_Comm new_comm) {
    int curDims, topology;
    MPI_Topo_test(new_comm, &topology);
    if (topology != MPI_CART) return false;
    MPI_Cartdim_get(new_comm, &curDims);
    if (curDims != LINEARTOPOLEN) return false;

    int dims[LINEARTOPOLEN], periods[LINEARTOPOLEN], coords[LINEARTOPOLEN];
    MPI_Cart_get(new_comm, LINEARTOPOLEN, dims, periods, coords);
    if (periods[0] != 0) return false;
    return true;
}

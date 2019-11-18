// Copyright 2019 Guseva Catherine
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include "../../../modules/task_2/guseva_e_hypercube_topology/Hypercube_topology.h"

MPI_Comm createHypercube(int tempDims, int sizeHypercube) {
    if (tempDims < 2 || sizeHypercube < 2) {
        throw "Hypercube > 2";
    }
    int sizeProc;
    int reorder = 1;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    int* Dims = new int[tempDims];
    int* periods = new int[tempDims];
    for (int i = 0; i < tempDims; i++) {
        Dims[i] = sizeHypercube;
        periods[i] = 1;
    }

    MPI_Comm hypercube;

    MPI_Cart_create(MPI_COMM_WORLD, tempDims, Dims, periods, reorder, &hypercube);

    return hypercube;
}

bool isHypercubeTop(MPI_Comm isComm, int tempDims, int sizeHypercube) {
    if (tempDims < 2 || sizeHypercube < 2) {
        throw "Hypercube > 2";
    }

    int countDims;
    int status;
    int* Dims = new int[tempDims];
    int* periods = new int[tempDims];
    int* coord = new int[tempDims];
    for (int i = 0; i < tempDims; i++) {
        Dims[i] = sizeHypercube;
    }

    MPI_Cartdim_get(isComm, &countDims);
    if (tempDims != countDims) {
        return false;
    }

    MPI_Cart_get(isComm, tempDims, Dims, periods, coord);
    for (int i = 0; i < tempDims; i++) {
        if (periods[i] != 1) {
            return false;
        }
    }

    MPI_Topo_test(isComm, &status);
    if (status != MPI_CART) {
        return false;
    }

    return true;
}

bool testHypercubeTopDATA(MPI_Comm Comm, int tempDims, int sizeHypercube) {
    if (tempDims < 2 || sizeHypercube < 2) {
        throw "Hypercube > 2";
    }

    int rank;
    int proc;
    int testcount = 1;
    int left, right;
    int local_data = 0, global_data = 0;
    MPI_Comm_rank(Comm, &rank);
    MPI_Comm_size(Comm, &proc);
    for (int i = 0; i < tempDims; i++) {
        MPI_Cart_shift(Comm, i, 1, &left, &right);
        if ((left == MPI_PROC_NULL) || (right == MPI_PROC_NULL)) {
            return false;
        }
    }

    if (rank == 0) {
        local_data = 1;
    }

    MPI_Bcast(&local_data, 1, MPI_INT, 0, Comm);
    MPI_Reduce(&local_data, &global_data, 1, MPI_INT, MPI_SUM, 0, Comm);

    for (int i = 0; i < tempDims; i++) {
        testcount *= sizeHypercube;
    }

    if (testcount != global_data) {
        return false;
    }

    return true;
}

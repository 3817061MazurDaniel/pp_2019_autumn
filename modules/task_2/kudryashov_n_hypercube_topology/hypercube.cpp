// Copyright 2019 Kudryashov Nikita

#include "../../../modules/task_2/kudryashov_n_hypercube_topology/hypercube.h"

MPI_Comm createHcube(unsigned int dim) {
    MPI_Comm hypercubeComm;

    if (dim != 0) {
        int* dimSize = new int[dim];
        int* periods = new int[dim];
        for (unsigned int i = 0; i < dim; i++) {
            dimSize[i] = 2;
            periods[i] = 0;
        }

        MPI_Cart_create(MPI_COMM_WORLD, dim, dimSize, periods, 1, &hypercubeComm);

        delete[] dimSize;
        delete[] periods;

        return hypercubeComm;
    } else {
        return MPI_COMM_NULL;
    }
}

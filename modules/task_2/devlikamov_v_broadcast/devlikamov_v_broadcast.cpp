// Copyright 2019 Devlikamov V
#include <../../../modules/task_2/devlikamov_v_broadcast/devlikamov_v_broadcast.h>
#include <iostream>
#include <cmath>

void my_bcast_slow(void *data, int count, MPI_Datatype datatype, int root,
                MPI_Comm communicator) {
    int world_rank;
    MPI_Comm_rank(communicator, &world_rank);
    int world_size;
    MPI_Comm_size(communicator, &world_size);
    if (world_rank == root) {
        int i;
        for (i = 0; i < world_size; i++) {
            if (i != world_rank) {
                MPI_Send(data, count, datatype, i, 0, communicator);
            }
        }
    } else {
        MPI_Recv(data, count, datatype, root, 0, communicator,
            MPI_STATUS_IGNORE);
    }
}

double loga(double a, double b) {
    return log(a)/log(2);
}

void my_bcast(void *data, int count, MPI_Datatype datatype, int root,
                MPI_Comm communicator) {
    int world_rank;
    MPI_Comm_rank(communicator, &world_rank);
    int world_size;
    MPI_Comm_size(communicator, &world_size);
    for (int step = 0; step < ceil(loga(world_size, 2)); step++) {
        int temp = 1 << step;
        if ((world_rank < temp) && (temp + world_rank < world_size)) {
            MPI_Send(data, count, datatype, (temp + world_rank), 0, communicator);
        } else if (world_rank < (temp << 1) && world_rank >= temp) {
            MPI_Recv(data, count, datatype, (world_rank - temp), 0, communicator, MPI_STATUS_IGNORE);
        }
    }
}

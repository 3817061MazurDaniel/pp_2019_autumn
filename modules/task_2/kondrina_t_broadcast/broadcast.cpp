// Copyright 2019 Kondrina Tatyana

#include <mpi.h>

#include "../../../modules/task_2/kondrina_t_broadcast/broadcast.h"

void bcast(void *buf, int count, MPI_Datatype datatype, int root,
           MPI_Comm comm) {
  int rank;
  MPI_Comm_rank(comm, &rank);
  int size;
  MPI_Comm_size(comm, &size);

  if (rank == root) {
    for (int i = 0; i < size; i++) {
      if (i != rank) {
        MPI_Send(buf, count, datatype, i, 0, comm);
      }
    }
  } else {
    MPI_Recv(buf, count, datatype, root, 0, comm, MPI_STATUS_IGNORE);
  }
}

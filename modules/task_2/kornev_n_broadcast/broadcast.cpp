// Copyright 2019 Kornev Nikita

#include "../../../modules/task_2/kornev_n_broadcast/broadcast.h"

void broadcast(void* buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == root) {
    for (int i = 0; i < size; i++) {
      if (i != root) {
        MPI_Send(buffer, count, datatype, i, 0, comm);
      }
    }
  } else {
    MPI_Status status;
    MPI_Recv(buffer, count, datatype, root, 0, comm, &status);
  }
}





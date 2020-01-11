// Copyright 2019 Pauzin Leonid
#include <mpi.h>
#include "../../../modules/task_2/pauzin_l_prod_cons/pauzin_l_prod_cons.h"

void ParallelFunc(const int n, void *arr) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  const int delta = n;
  if (n < 1) {
    throw "Wrong count of elements";
  }

  if (size > 1) {
    const int remainder = n % (size / 2);  // remaining elems
    if (remainder == 0) {
      if (rank % 2 == 0) {
        MPI_Send(arr, delta, MPI_INT, rank + 1, rank, MPI_COMM_WORLD);
      } else {  // consumers
        MPI_Status status;
        MPI_Recv(arr, delta, MPI_INT, rank - 1, rank - 1, MPI_COMM_WORLD, &status);
      }
    } else {
        if (rank % 2 == 0) {  // producers
          if (rank == 0) {
            MPI_Send(arr, delta + remainder, MPI_INT, rank + 1, rank, MPI_COMM_WORLD);
          } else {
          MPI_Send(arr, delta, MPI_INT, rank + 1, rank, MPI_COMM_WORLD);
        }
      } else {  // consumers
        if (rank == 1) {
          MPI_Status status;
          MPI_Recv(arr, delta + remainder, MPI_INT, rank - 1, rank - 1, MPI_COMM_WORLD, &status);
        } else {
          MPI_Status status;
          MPI_Recv(arr, delta, MPI_INT, rank - 1, rank - 1, MPI_COMM_WORLD, &status);
        }
      }
    }
  } else {
    if (rank == 0) {
      for (int i = 0; i < n; i++) {
        if (rank % 2 != 0) {
          MPI_Send(arr, n, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
      }
    } else {  // consumer
      MPI_Status status;
      MPI_Recv(arr, n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
  }
}

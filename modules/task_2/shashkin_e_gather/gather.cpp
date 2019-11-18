// Copyright 2019 Shashkin Evgeny
#include "../../../modules/task_2/shashkin_e_gather/gather.h"
#include <mpi.h>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <cmath>

int Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount,
  MPI_Datatype recvtype, int root, MPI_Comm comm) {
  int size, rank;
  int sendtype_size, recvtype_size;
  MPI_Status status;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Type_size(sendtype, &sendtype_size);
  MPI_Type_size(recvtype, &recvtype_size);

  if (root < 0)
    throw std::runtime_error("Wrong root!\n");
  if (sendcount <= 0)
    throw std::runtime_error("Sendcount is wrong!\n");

  if (rank == root) {
    memcpy(reinterpret_cast<char*>(recvbuf) + recvcount * recvtype_size * root, sendbuf,
      sendcount * sendtype_size);
    for (int i = 0; i < size; ++i) {
      if (i != root)
        MPI_Recv(reinterpret_cast<char*>(recvbuf) + recvcount * recvtype_size * i, recvcount,
            recvtype, i, 0, comm, &status);
    }
  } else {
    MPI_Send(sendbuf, sendcount, sendtype, root, 0, comm);
  }

  return MPI_SUCCESS;
}


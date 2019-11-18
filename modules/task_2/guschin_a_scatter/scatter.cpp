// Copyright 2019 Guschin Alexander
#include "../../../modules/task_2/guschin_a_scatter/scatter.h"
#include <mpi.h>
#include <string>
#include <cstring>
#include <vector>

int MPI_Scatter_custom(void* send_buf, int send_count, MPI_Datatype send_type,
                       void* recv_buf, int recv_count, MPI_Datatype recv_type,
                       int root, MPI_Comm comm) {
  int size, rank;
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  int send_type_size, recv_type_size;

  if (MPI_Type_size(send_type, &send_type_size) == MPI_ERR_TYPE)
    return MPI_ERR_TYPE;

  if (MPI_Type_size(recv_type, &recv_type_size) == MPI_ERR_TYPE)
    return MPI_ERR_TYPE;

  if (send_count != recv_count || send_count <= 0 || recv_count <= 0 || root < 0)
    return MPI_ERR_COUNT;


  if (rank == root) {
    memcpy(
        recv_buf,
        reinterpret_cast<char*>(send_buf) + rank * send_count * send_type_size,
        send_count * send_type_size);

    for (int i = 0; i < size; i++) {
      if (i == root) continue;
      MPI_Send(
          reinterpret_cast<char*>(send_buf) + i * send_count * send_type_size,
          send_count, send_type, i, 0, comm);
    }
  } else {
    MPI_Status status;
    MPI_Recv(recv_buf, recv_count, recv_type, root, 0, comm, &status);
  }
  return MPI_SUCCESS;
}

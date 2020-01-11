// Copyright 2019 Guschin Alexander
#include "../../../modules/task_2/guschin_a_scatter/scatter.h"
#include <mpi.h>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <ctime>

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

  if (send_count != recv_count || send_count <= 0 || recv_count <= 0 ||
      root < 0)
    return MPI_ERR_COUNT;

  if (rank == root) {
    memcpy(recv_buf,
           static_cast<char*>(send_buf) + rank * send_count * send_type_size,
           send_count * send_type_size);

    for (int i = 0; i < size; i++) {
      if (i == root) continue;
      MPI_Send(static_cast<char*>(send_buf) + i * send_count * send_type_size,
               send_count, send_type, i, 0, comm);
    }
  } else {
    MPI_Status status;
    MPI_Recv(recv_buf, recv_count, recv_type, root, 0, comm, &status);
  }
  return MPI_SUCCESS;
}

int MPI_Scatter_bin(void* send_buf, int send_count, MPI_Datatype send_type,
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

  if (send_count != recv_count || send_count <= 0 || recv_count <= 0 ||
      root < 0)
    return MPI_ERR_COUNT;

  // size of recv_buf = cntr * recv_type_size * recv_count
  int cntr = size;
  int st = 0, pr = 0;
  while (st != (rank - root + size) % size) {
    --cntr;
    if (st < (rank - root + size) % size &&
        st + cntr / 2 + cntr % 2 + 1 > (rank - root + size) % size) {
      pr = st;
      st += 1;
      cntr = cntr / 2 + cntr % 2;
    } else {
      pr = st;
      st += (cntr / 2 + cntr % 2 + 1);
      cntr = cntr / 2;
    }
  }
  pr = (pr + root) % size;

  // not rooot
  if (rank != root) {
    char* recv_ptr = new char[cntr * recv_type_size * recv_count];
    MPI_Status status;
    MPI_Recv(recv_ptr, cntr * recv_count, recv_type, pr, 0, comm, &status);
    memcpy(recv_buf, &recv_ptr[0], recv_count * recv_type_size);
    --cntr;

    if (cntr > 0) {
      MPI_Send(recv_ptr + send_count * send_type_size,
               send_count * (cntr / 2 + cntr % 2), send_type, (rank + 1) % size,
               0, comm);
    }
    if (cntr > 1) {
      MPI_Send(
          recv_ptr + send_count * send_type_size * (cntr / 2 + cntr % 2 + 1),
          send_count * (cntr / 2), send_type,
          (rank + cntr / 2 + cntr % 2 + 1) % size, 0, comm);
    }
    delete[] recv_ptr;
  }

  // root
  if (rank == root) {
    --cntr;
    int first_step = (rank + 1) * send_count * send_type_size;
    int second_step =
        send_count * send_type_size * (rank + cntr / 2 + cntr % 2 + 1);
    int end = size * send_count * send_type_size;

    char* fir_ptr = nullptr;
    char* sec_ptr = nullptr;
    char* tmp = nullptr;
    if (root == 0) {
      fir_ptr = static_cast<char*>(send_buf) + first_step;
      sec_ptr = static_cast<char*>(send_buf) + second_step;
    } else if (root == size - 1) {
      fir_ptr = static_cast<char*>(send_buf);
      sec_ptr = static_cast<char*>(send_buf) +
                send_count * send_type_size * (cntr / 2 + cntr % 2);
    } else if (root < size / 2 && root != 0 && root != size - 1) {
      tmp = new char[send_count * (cntr / 2) * send_type_size];
      int j = 0;
      int new_end =
          (second_step + send_count * (cntr / 2) * send_type_size) % end;
      for (int i = second_step; i < end; ++i)
        tmp[j++] = static_cast<char*>(send_buf)[i];
      for (int i = 0; i < new_end; i++)
        tmp[j++] = static_cast<char*>(send_buf)[i];
      sec_ptr = tmp;
      fir_ptr = static_cast<char*>(send_buf) + first_step;
    } else if (root != 0 && root != size - 1) {
      tmp = new char[send_count * (cntr / 2 + cntr % 2) * send_type_size];
      int j = 0;
      int new_end =
          (first_step + send_count * (cntr / 2 + cntr % 2) * send_type_size) %
          end;
      for (int i = first_step; i < end; ++i)
        tmp[j++] = static_cast<char*>(send_buf)[i];
      for (int i = 0; i < new_end; i++)
        tmp[j++] = static_cast<char*>(send_buf)[i];

      fir_ptr = tmp;
      sec_ptr = static_cast<char*>(send_buf) +
                send_count * send_type_size *
                    ((rank + cntr / 2 + cntr % 2 + 1) % size);
    }

    memcpy(recv_buf,
           static_cast<char*>(send_buf) + rank * send_count * send_type_size,
           send_count * send_type_size);

    if (cntr > 0)
      MPI_Send(fir_ptr, send_count * (cntr / 2 + cntr % 2), send_type,
               (rank + 1) % size, 0, comm);
    if (cntr > 1)
      MPI_Send(sec_ptr, send_count * (cntr / 2), send_type,
               (rank + cntr / 2 + cntr % 2 + 1) % size, 0, comm);

    if (tmp != nullptr) delete[] tmp;
  }

  return MPI_SUCCESS;
}

double Vector_sum_bin(std::vector<int> vec) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int vec_size = 0;
  if (rank == 0) vec_size = vec.size();

  MPI_Bcast(&vec_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int gen_size = vec_size / size;
  int side_size = vec_size % size;

  std::vector<double> local_vec(gen_size);

  if (rank == 0) local_vec.resize(gen_size + side_size);

  MPI_Scatter_bin(&vec[0], gen_size, MPI_INT, &local_vec[0], gen_size, MPI_INT,
                  0, MPI_COMM_WORLD);
  if (rank == 0) {
    for (int i = 0; i < gen_size + side_size; ++i) local_vec[i] = vec[i];
  }

  int local_size = local_vec.size();
  double global_sum = 0;
  double local_sum = 0;
  for (int i = 0; i < local_size; ++i) local_sum += local_vec[i];

  MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);
  return global_sum;
}

double Vector_sum(std::vector<int> vec) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int vec_size = 0;
  if (rank == 0) vec_size = vec.size();

  MPI_Bcast(&vec_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int gen_size = vec_size / size;
  int side_size = vec_size % size;

  std::vector<double> local_vec(gen_size);

  if (rank == 0) local_vec.resize(gen_size + side_size);

  MPI_Scatter(&vec[0], gen_size, MPI_INT, &local_vec[0], gen_size, MPI_INT, 0,
              MPI_COMM_WORLD);
  if (rank == 0) {
    for (int i = 0; i < gen_size + side_size; ++i) local_vec[i] = vec[i];
  }

  int local_size = local_vec.size();
  double global_sum = 0;
  double local_sum = 0;
  for (int i = 0; i < local_size; ++i) local_sum += local_vec[i];

  MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);
  return global_sum;
}

void Get_rand(int* ptr, int size) {
  std::mt19937 gen;
  gen.seed(time(0));
  for (int i = 0; i < size; i++) {
    ptr[i] = gen() % 207;
  }
}

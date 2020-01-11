// Copyright 2019 Khruleva Anastasia

#include <mpi.h>
#include <vector>
#include <cstring>
#include "../../../modules/task_2/khruleva_reduce/reduce_all.h"

int MPI_Reduce_Gather(const void *sbuf, void *rbuf, int count, MPI_Datatype datatype, MPI_Op op,
  int root, MPI_Comm comm) {
  if (count <= 0) return MPI_ERR_COUNT;
  if (sbuf == nullptr || rbuf == nullptr) return MPI_ERR_BUFFER;
  int ProcNum;
  int ProcRank;

  if (MPI_Comm_size(comm, &ProcNum) == MPI_ERR_COMM ||
    MPI_Comm_rank(comm, &ProcRank) == MPI_ERR_COMM) {
    return MPI_ERR_COMM;
  }

  if (datatype == MPI_INT) {
    std::memcpy(rbuf, sbuf, count * sizeof(int));
  } else {
    if (datatype == MPI_DOUBLE) {
      std::memcpy(rbuf, sbuf, count * sizeof(double));
    } else {
      if (datatype == MPI_FLOAT) {
        std::memcpy(rbuf, sbuf, count * sizeof(float));
      } else {
        return MPI_ERR_TYPE;
      }
    }
  }

  while (ProcNum > 1) {
    int NewProcNum = ProcNum / 2;
    int remainder = ProcNum % 2;

    if (ProcRank >= NewProcNum + remainder && ProcRank < ProcNum) {
      int dest = ProcNum - ProcRank - 1;
      MPI_Send(rbuf, count, datatype, dest, 0, comm);
      break;
    } else {
      if (ProcRank < NewProcNum) {
        int source = ProcNum - ProcRank - 1;
        MPI_Status status;
        if (datatype == MPI_INT) {
          int * buf = new int[count];
          MPI_Recv(buf, count, datatype, source, 0, comm, &status);
          if (op == MPI_MAX) {
            for (int i = 0; i < count; i++) {
              if (buf[i] > static_cast<int*>(rbuf)[i]) {
                static_cast<int*>(rbuf)[i] = buf[i];
              }
            }
          } else {
            if (op == MPI_MIN) {
              for (int i = 0; i < count; i++) {
                if (buf[i] < static_cast<int*>(rbuf)[i]) {
                  static_cast<int*>(rbuf)[i] = buf[i];
                }
              }
            } else {
              if (op == MPI_SUM) {
                for (int i = 0; i < count; i++) {
                  static_cast<int*>(rbuf)[i] += buf[i];
                }
              } else {
                if (op == MPI_PROD) {
                  for (int i = 0; i < count; i++) {
                    static_cast<int*>(rbuf)[i] *= buf[i];
                  }
                }
              }
            }
          }
        } else {
          if (datatype == MPI_DOUBLE) {
            double * buf = new double[count];
            MPI_Recv(buf, count, datatype, source, 0, comm, &status);
            if (op == MPI_MAX) {
              for (int i = 0; i < count; i++) {
                if (buf[i] > static_cast<double*>(rbuf)[i]) {
                  static_cast<double*>(rbuf)[i] = buf[i];
                }
              }
            } else {
              if (op == MPI_MIN) {
                for (int i = 0; i < count; i++) {
                  if (buf[i] < static_cast<double*>(rbuf)[i]) {
                    static_cast<double*>(rbuf)[i] = buf[i];
                  }
                }
              } else {
                if (op == MPI_SUM) {
                  for (int i = 0; i < count; i++) {
                    static_cast<double*>(rbuf)[i] += buf[i];
                  }
                } else {
                  if (op == MPI_PROD) {
                    for (int i = 0; i < count; i++) {
                      static_cast<double*>(rbuf)[i] *= buf[i];
                    }
                  }
                }
              }
            }
          } else {
            if (datatype == MPI_FLOAT) {
              float * buf = new float[count];
              MPI_Recv(buf, count, datatype, source, 0, comm, &status);
              if (op == MPI_MAX) {
                for (int i = 0; i < count; i++) {
                  if (buf[i] > static_cast<float*>(rbuf)[i]) {
                    static_cast<float*>(rbuf)[i] = buf[i];
                  }
                }
              } else {
                if (op == MPI_MIN) {
                  for (int i = 0; i < count; i++) {
                    if (buf[i] < static_cast<float*>(rbuf)[i]) {
                      static_cast<float*>(rbuf)[i] = buf[i];
                    }
                  }
                } else {
                  if (op == MPI_SUM) {
                    for (int i = 0; i < count; i++) {
                      static_cast<float*>(rbuf)[i] += buf[i];
                    }
                  } else {
                    if (op == MPI_PROD) {
                      for (int i = 0; i < count; i++) {
                        static_cast<float*>(rbuf)[i] *= buf[i];
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    ProcNum = NewProcNum + remainder;
  }
  if (ProcRank == 0 && root != 0) {
    MPI_Send(rbuf, count, datatype, root, 0, comm);
  }
  if (root != 0 && ProcRank == root) {
    MPI_Status status;
    MPI_Recv(rbuf, count, datatype, 0, 0, comm, &status);
  }

  return MPI_SUCCESS;
}

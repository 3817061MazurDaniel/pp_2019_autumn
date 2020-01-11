// Copyright 2019 Astafeva Irina

#include <mpi.h>
#include <vector>
#include <cstring>
#include "../../../modules/task_2/astafeva_i_mpi_reduce/mpi_reduce.h"

int MPI_Reduce_User(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op,
    int root, MPI_Comm comm) {
    if (count < 0) return MPI_ERR_COUNT;

    if (sendbuf == nullptr || recvbuf == nullptr) return MPI_ERR_BUFFER;

    int size;
    int rank;
    if (MPI_Comm_size(comm, &size) == MPI_ERR_COMM ||
        MPI_Comm_rank(comm, &rank) == MPI_ERR_COMM) {
        return MPI_ERR_COMM;
    }

    if (datatype == MPI_INT) {
        std::memcpy(recvbuf, sendbuf, count * sizeof(int));
    } else {
        if (datatype == MPI_DOUBLE) {
            std::memcpy(recvbuf, sendbuf, count * sizeof(double));
        } else {
            if (datatype == MPI_FLOAT) {
                std::memcpy(recvbuf, sendbuf, count * sizeof(float));
            } else {
                return MPI_ERR_TYPE;
            }
        }
    }

    while (size > 1) {
        int newsize = size / 2;
        int deltasize = size % 2;

        if (rank >= newsize + deltasize && rank < size) {
            int dest = size - 1 - rank;
            MPI_Send(recvbuf, count, datatype, dest, 0, comm);
            break;
        } else {
            if (rank < newsize) {
                int source = size - 1 - rank;
                MPI_Status status;
                if (datatype == MPI_INT) {
                    int * buf = new int[count];
                    MPI_Recv(buf, count, datatype, source, 0, comm, &status);
                    if (op == MPI_MAX) {
                        for (int i = 0; i < count; i++) {
                            if (buf[i] > static_cast<int*>(recvbuf)[i]) {
                                static_cast<int*>(recvbuf)[i] = buf[i];
                            }
                        }
                    } else {
                        if (op == MPI_MIN) {
                            for (int i = 0; i < count; i++) {
                                if (buf[i] < static_cast<int*>(recvbuf)[i]) {
                                    static_cast<int*>(recvbuf)[i] = buf[i];
                                }
                            }
                        } else {
                            if (op == MPI_SUM) {
                                for (int i = 0; i < count; i++) {
                                    static_cast<int*>(recvbuf)[i] += buf[i];
                                }
                            } else {
                                if (op == MPI_PROD) {
                                    for (int i = 0; i < count; i++) {
                                        static_cast<int*>(recvbuf)[i] *= buf[i];
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
                                if (buf[i] > static_cast<double*>(recvbuf)[i]) {
                                    static_cast<double*>(recvbuf)[i] = buf[i];
                                }
                            }
                        } else {
                            if (op == MPI_MIN) {
                                for (int i = 0; i < count; i++) {
                                    if (buf[i] < static_cast<double*>(recvbuf)[i]) {
                                        static_cast<double*>(recvbuf)[i] = buf[i];
                                    }
                                }
                            } else {
                                if (op == MPI_SUM) {
                                    for (int i = 0; i < count; i++) {
                                        static_cast<double*>(recvbuf)[i] += buf[i];
                                    }
                                } else {
                                    if (op == MPI_PROD) {
                                        for (int i = 0; i < count; i++) {
                                            static_cast<double*>(recvbuf)[i] *= buf[i];
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
                                    if (buf[i] > static_cast<float*>(recvbuf)[i]) {
                                        static_cast<float*>(recvbuf)[i] = buf[i];
                                    }
                                }
                            } else {
                                if (op == MPI_MIN) {
                                    for (int i = 0; i < count; i++) {
                                        if (buf[i] < static_cast<float*>(recvbuf)[i]) {
                                            static_cast<float*>(recvbuf)[i] = buf[i];
                                        }
                                    }
                                } else {
                                    if (op == MPI_SUM) {
                                        for (int i = 0; i < count; i++) {
                                            static_cast<float*>(recvbuf)[i] += buf[i];
                                        }
                                    } else {
                                        if (op == MPI_PROD) {
                                            for (int i = 0; i < count; i++) {
                                                static_cast<float*>(recvbuf)[i] *= buf[i];
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
        size = newsize + deltasize;
    }
    if (rank == 0 && root != 0) {
        MPI_Send(recvbuf, count, datatype, root, 0, comm);
    }
    if (root != 0 && rank == root) {
        MPI_Status status;
        MPI_Recv(recvbuf, count, datatype, 0, 0, comm, &status);
    }

    return MPI_SUCCESS;
}

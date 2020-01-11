// Copyright 2019 Ryabova Alyona

#include <mpi.h>
#include <algorithm>
#include <cstring>
#include <iostream>
#include "../../../modules/task_2/ryabova_a_mpi_allreduce/mpi_allreduce.h"

int MPI_Allreduce_c(void* sendbuf, void* recvbuf, int count,
    MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {

    if (count <= 0)
        return MPI_ERR_COUNT;
    if ((datatype != MPI_INT) && (datatype != MPI_FLOAT) && (datatype != MPI_DOUBLE))
        return MPI_ERR_TYPE;
    if ((op != MPI_SUM) && (op != MPI_PROD) && (op != MPI_MIN) && (op != MPI_MAX))
        return MPI_ERR_OP;

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* getbufInt;
    float* getbufFloat;
    double* getbufDouble;

    if (datatype == MPI_INT) {
        std::memcpy(recvbuf, sendbuf, count * sizeof(int));
        getbufInt = new int[count];
    }
    if (datatype == MPI_FLOAT) {
        std::memcpy(recvbuf, sendbuf, count * sizeof(float));
        getbufFloat = new float[count];
    }
    if (datatype == MPI_DOUBLE) {
        std::memcpy(recvbuf, sendbuf, count * sizeof(double));
        getbufDouble = new double[count];
    }

    if (size < 2) {
        return MPI_SUCCESS;
    }

    bool odd = 0;
    if (size % 2 == 1)
        odd = 1;
    int localsize = size;

    while (localsize != 1) {
        for (int i = 0; i < localsize / 2; i++)
            if (rank == localsize - i - 1)
                MPI_Send(recvbuf, count, datatype, i, i, comm);
        for (int i = 0; i < localsize / 2; i++)
            if (rank == i) {
                MPI_Status status;
                if (datatype == MPI_INT)
                    MPI_Recv(getbufInt, count, datatype, localsize - i - 1, i, comm, &status);
                if (datatype == MPI_FLOAT)
                    MPI_Recv(getbufFloat, count, datatype, localsize - i - 1, i, comm, &status);
                if (datatype == MPI_DOUBLE)
                    MPI_Recv(getbufDouble, count, datatype, localsize - i - 1, i, comm, &status);
                if (op == MPI_MAX) {
                    if (datatype == MPI_INT) {
                        for (int j = 0; j < count; j++) {
                            if (static_cast<int*>(recvbuf)[j] < getbufInt[j])
                                static_cast<int*>(recvbuf)[j] = getbufInt[j];
                        }
                    } else if (datatype == MPI_FLOAT) {
                        for (int j = 0; j < count; j++) {
                            if (static_cast<float*>(recvbuf)[j] < getbufFloat[j])
                                static_cast<float*>(recvbuf)[j] = getbufFloat[j];
                        }
                    } else if (datatype == MPI_DOUBLE) {
                        for (int j = 0; j < count; j++) {
                            if (static_cast<double*>(recvbuf)[j] < getbufDouble[j])
                                static_cast<double*>(recvbuf)[j] = getbufDouble[j];
                        }
                    }
                } else if (op == MPI_MIN) {
                    if (datatype == MPI_INT) {
                        for (int j = 0; j < count; j++) {
                            if (static_cast<int*>(recvbuf)[j] > getbufInt[j])
                                static_cast<int*>(recvbuf)[j] = getbufInt[j];
                        }
                    } else if (datatype == MPI_FLOAT) {
                        for (int j = 0; j < count; j++) {
                            if (static_cast<float*>(recvbuf)[j] > getbufFloat[j])
                                static_cast<float*>(recvbuf)[j] = getbufFloat[j];
                        }
                    } else if (datatype == MPI_DOUBLE) {
                        for (int j = 0; j < count; j++) {
                            if (static_cast<double*>(recvbuf)[j] > getbufDouble[j])
                                static_cast<double*>(recvbuf)[j] = getbufDouble[j];
                        }
                    }
                } else if (op == MPI_SUM) {
                    if (datatype == MPI_INT) {
                        for (int j = 0; j < count; j++) {
                            static_cast<int*>(recvbuf)[j] += getbufInt[j];
                        }
                    } else if (datatype == MPI_FLOAT) {
                        for (int j = 0; j < count; j++) {
                            static_cast<float*>(recvbuf)[j] += getbufFloat[j];
                        }
                    } else if (datatype == MPI_DOUBLE) {
                        for (int j = 0; j < count; j++) {
                            static_cast<double*>(recvbuf)[j] += getbufDouble[j];
                        }
                    }
                } else if (op == MPI_PROD) {
                    if (datatype == MPI_INT) {
                        for (int j = 0; j < count; j++) {
                            static_cast<int*>(recvbuf)[j] *= getbufInt[j];
                        }
                    } else if (datatype == MPI_FLOAT) {
                        for (int j = 0; j < count; j++) {
                            static_cast<float*>(recvbuf)[j] *= getbufFloat[j];
                        }
                    } else if (datatype == MPI_DOUBLE) {
                        for (int j = 0; j < count; j++) {
                            static_cast<double*>(recvbuf)[j] *= getbufDouble[j];
                        }
                    }
                }
            }
        if (localsize % 2 == 1)
            odd = 1;
        else
            odd = 0;
        localsize = localsize / 2;
        if (odd == 1)
            localsize++;
    }

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(recvbuf, count, datatype, i, i, comm);
        }
    } else {
        if (datatype == MPI_INT) {
            for (int i = 1; i < size; i++) {
                if (rank == i) {
                    MPI_Status status;
                    MPI_Recv(recvbuf, count, datatype, 0, i, comm, &status);
                }
            }
        } else if (datatype == MPI_FLOAT) {
            for (int i = 1; i < size; i++) {
                if (rank == i) {
                    MPI_Status status;
                    MPI_Recv(recvbuf, count, datatype, 0, i, comm, &status);
                }
            }
        } else if (datatype == MPI_DOUBLE) {
            for (int i = 1; i < size; i++) {
                if (rank == i) {
                    MPI_Status status;
                    MPI_Recv(recvbuf, count, datatype, 0, i, comm, &status);
                }
            }
        }
    }

    return MPI_SUCCESS;
}

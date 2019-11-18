// Copyright 2019 Andronov Maxim
#include <random>
#include <vector>
#include "../../../modules/task_2/andronov_m_ring_topology/ring_topology.h"

MPI_Comm CreateRingTopology(MPI_Comm oldcomm) {
    MPI_Comm ringcomm;
    std::vector<int> dims(1), periods(1);

    MPI_Comm_size(oldcomm, &dims[0]);
    periods[0] = 1;

    MPI_Cart_create(oldcomm, 1, dims.data(), periods.data(), 0, &ringcomm);

    return ringcomm;
}

bool IsRingTopology(MPI_Comm comm) {
    int status;
    MPI_Topo_test(comm, &status);
    if (status != MPI_CART)
        return false;

    int ndims;
    MPI_Cartdim_get(comm, &ndims);
    if (ndims != 1)
        return false;

    std::vector<int> dims(ndims), periods(ndims), coords(ndims);
    MPI_Cart_get(comm, ndims, dims.data(), periods.data(), coords.data());
    if (periods[0] != 1)
        return false;

    return true;
}

std::vector<int> Send(MPI_Comm ringcomm, int source, int dest,
                        std::vector <int> message, int mess_size) {
    int size, rank;
    MPI_Comm_size(ringcomm, &size);
    MPI_Comm_rank(ringcomm, &rank);
    std::vector<int> result;

    if (source > (size-1) || source < 0 || dest > (size - 1) || dest < 0)
        throw -1;

    if (((rank >= source) && (rank <= dest)) ||
        ((dest - source < 0) && ((rank >= source) || (rank <= dest)))) {
        if (dest == source)
            return message;

        int curr_dest, curr_source;
        MPI_Status status;

        MPI_Cart_shift(ringcomm, 0, 1, &curr_source, &curr_dest);

        if (rank == source) {
            MPI_Send(&message[0], mess_size, MPI_INT, curr_dest, 1, ringcomm);
        } else if (rank == dest) {
            result.resize(mess_size);
            MPI_Recv(&result[0], mess_size, MPI_INT, curr_source, 1, ringcomm, &status);
        } else {
            std::vector<int> *tmp = new std::vector<int>;
            tmp->resize(mess_size);

            MPI_Recv(&((*tmp)[0]), mess_size, MPI_INT, curr_source, 1, ringcomm, &status);
            MPI_Send(&((*tmp)[0]), mess_size, MPI_INT, curr_dest, 1, ringcomm);

            delete tmp;
        }
    }
    return result;
}

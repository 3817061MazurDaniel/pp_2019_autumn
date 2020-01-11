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

std::vector<int> GetRandomVector(int size) {
    if (size < 1)
        throw - 1;

    std::vector <int> vec;
    vec.resize(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution <int> dist(-100, 100);

    for (int i = 0; i < size; i++)
        vec[i] = dist(gen);

    return vec;
}

int SeqSum(std::vector<int> vec) {
    int local_sum = 0;
    for (size_t i = 0; i < vec.size(); i++) {
        local_sum += vec[i];
    }
    return local_sum;
}

int ParallelSum(std::vector <int> vec, int sz, MPI_Comm ringcomm) {
    int size, rank;
    MPI_Comm_size(ringcomm, &size);
    MPI_Comm_rank(ringcomm, &rank);
    const int delta = (sz / size);
    const int delta_rem = (sz % size);

    int global_sum = 0;
    int local_sum = 0;

    std::vector <int> local_columns(sz);
    std::vector <int> local_columns0;

    if (rank == 0) {
        if (delta > 0) {
            for (int i = 0; i < size - 1; i++)
                MPI_Send(&vec[(delta + delta_rem)] + delta * i,
                    delta, MPI_INT, i + 1, 2, ringcomm);
        }
        local_columns0 = std::vector<int>(vec.begin(),
            vec.begin() + (delta + delta_rem));
    } else {
        MPI_Status status;
        if (delta > 0)
            MPI_Recv(&local_columns[0],
                delta, MPI_INT, 0, 2, ringcomm, &status);
    }

    if (rank == 0) {
        global_sum = SeqSum(local_columns0);

        if (delta > 0) {
            std::vector <int> local_result(delta);
            for (int i = 1; i < size; i++) {
                MPI_Status status;
                MPI_Recv(&local_sum, 1, MPI_INT, i, 3, ringcomm, &status);
                global_sum += local_sum;
            }
        }
    } else {
        if (delta > 0) {
            local_sum = SeqSum(local_columns);
        }
        MPI_Send(&local_sum, 1, MPI_INT, 0, 3, ringcomm);
    }

    return global_sum;
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

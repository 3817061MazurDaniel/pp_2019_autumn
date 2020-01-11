// Copyright 2019 Iamshchikov Ivan
#include <vector>
#include "../../../modules/task_2/iamshchikov_i_tor_net_topology/tor_net_topology.h"

int getNodenumInNetworkTopology(const std::vector<int> dims) {
    if (dims.size() == 0) throw -1;
    int nodenum = 1;
    for (size_t i = 0; i < dims.size(); ++i)
        nodenum *= dims[i];
    return nodenum;
}

MPI_Comm createTorusTopology(const std::vector<int> dims,
                             const MPI_Comm comm_base) {
    int procnum_in_base_comm;
    MPI_Comm_size(comm_base, &procnum_in_base_comm);
    int nodenum_in_network_topology = getNodenumInNetworkTopology(dims);
    if (nodenum_in_network_topology > procnum_in_base_comm) {
    throw -1;
    } else {
        MPI_Comm comm_cart = MPI_COMM_NULL;
        int ndims = dims.size();
        std::vector<int> periods(ndims);
        periods.assign(ndims, 1);
        MPI_Cart_create(comm_base, ndims, dims.data(), periods.data(),
                        false, &comm_cart);
        return comm_cart;
    }
}

bool isNeighbor(MPI_Comm comm_cart, std::vector<int> dims, int ndims,
                int root, int dest) {
    if (root == dest) {
        return false;
    } else {
        std::vector<int> coord_root(ndims), coord_dest(ndims), dims(ndims);
        MPI_Cart_coords(comm_cart, root, ndims, coord_root.data());
        MPI_Cart_coords(comm_cart, dest, ndims, coord_dest.data());
        for (int i = 0; i < ndims; ++i)
            if (abs(coord_root[i] - coord_dest[i]) > 1
                && abs(coord_root[i] - coord_dest[i]) < abs(dims[i] - 1))
                return false;
        return true;
    }
}

int getDirection(int coord_source, int coord_dest, int dim_size) {
    int half = dim_size / 2;
    int delta;
    if (coord_source < coord_dest) {
        delta = coord_dest - coord_source;
        if (delta <= half)
            return delta;
        else
            return -delta;
    } else if (coord_source > coord_dest) {
        delta = coord_source - coord_dest;
        if (delta <= half)
            return -delta;
        else
            return delta;
    } else {
        throw -2;
    }
}

int getStep(int dir) {
    return dir > 0 ? 1 : -1;
}

int getNextDest(std::vector<int> curr_coord, int curr_dim, int step,
                MPI_Comm comm) {
    int dest;
    std::vector<int> dest_coord(curr_coord);
    dest_coord[curr_dim] += step;
    MPI_Cart_rank(comm, dest_coord.data(), &dest);
    return dest;
}

std::vector<int> getTransit(int root, int dest, std::vector<int> dims,
                            MPI_Comm comm_cart, int procrank) {
    int ndims = dims.size();
    std::vector<int> route, transit;
    std::vector<int> coord_root(ndims), coord_dest(ndims),
                     coord_curr_source(ndims), coord_dest_in_dim(ndims);
    int curr_source = root, next_dest = root, dest_in_dim = root;

    MPI_Cart_coords(comm_cart, root, ndims, coord_root.data());
    MPI_Cart_coords(comm_cart, dest, ndims, coord_dest.data());

    route.push_back(curr_source);
    for (int curr_dim = 0; curr_dim < ndims; ++curr_dim) {
        MPI_Cart_coords(comm_cart, curr_source, ndims,
                        coord_curr_source.data());
        coord_dest_in_dim = coord_curr_source;
        coord_dest_in_dim[curr_dim] = coord_dest[curr_dim];
        MPI_Cart_rank(comm_cart, coord_dest_in_dim.data(), &dest_in_dim);

        if (dest_in_dim != curr_source) {
            int steps = getDirection(coord_root[curr_dim],
                                      coord_dest[curr_dim], dims[curr_dim]);

            while (next_dest != dest_in_dim) {
                MPI_Cart_coords(comm_cart, curr_source, ndims,
                                coord_curr_source.data());
                next_dest = getNextDest(coord_curr_source, curr_dim,
                                        getStep(steps), comm_cart);
                if (procrank == next_dest)
                    transit.push_back(curr_source);
                else if (procrank == curr_source)
                    transit.push_back(next_dest);
                route.push_back(next_dest);
                curr_source = next_dest;
            }
        }
    }
    return transit;
}

void send(std::vector<int>* pdata, int count, MPI_Datatype datatype,
          int root, int dest, std::vector<int> dims, MPI_Comm comm_cart) {
    int procrank, procnum, ndims = dims.size();
    std::vector<int> transit;
    MPI_Status st;

    if (comm_cart == MPI_COMM_NULL) throw -1;
    MPI_Comm_size(comm_cart, &procnum);

    if (root >= procnum || dest >= procnum) throw -1;
    MPI_Cartdim_get(comm_cart, &ndims);

    MPI_Comm_size(MPI_COMM_WORLD, &procnum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procrank);

    if (procrank != root && pdata->size() != static_cast<size_t>(count))
        pdata->resize(count);

    if (root != dest) {
        transit = getTransit(root, dest, dims, comm_cart, procrank);
        if (transit.size() != 0) {
            if (procrank == root) {
                if (!isNeighbor(comm_cart, dims, ndims, procrank, transit[0]))
                    throw -1;
                MPI_Send(pdata->data(), count, datatype, transit[0], 0,
                         comm_cart);
            } else if (procrank != dest) {
                if (!isNeighbor(comm_cart, dims, ndims, procrank, transit[0])
                    ||
                    !isNeighbor(comm_cart, dims, ndims, procrank, transit[1]))
                        throw - 1;
                MPI_Recv(pdata->data(), count, datatype, transit[0], 0,
                         comm_cart, &st);
                MPI_Send(pdata->data(), count, datatype, transit[1], 0,
                         comm_cart);
            } else {
                if (!isNeighbor(comm_cart, dims, ndims, procrank, transit[0]))
                    throw - 1;
                MPI_Recv(pdata->data(), count, datatype, transit[0], 0,
                         comm_cart, &st);
            }
        }
    }
}

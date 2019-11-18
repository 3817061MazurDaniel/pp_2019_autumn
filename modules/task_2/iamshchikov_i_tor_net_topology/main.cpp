// Copyright 2019 Iamshchikov Ivan
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./tor_net_topology.h"

TEST(torus_network_topology, throw_when_ndims_is_null) {
    MPI_Comm comm_cart = MPI_COMM_NULL;
    std::vector<int> dims;

    ASSERT_ANY_THROW(createTorusTopology(dims));
    EXPECT_EQ(MPI_COMM_NULL, comm_cart);
}

TEST(torus_network_topology, can_get_nodenum_in_network_topology) {
    int rank;
    std::vector<int> dims = { 3, 8, 4 };
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        EXPECT_EQ(96, getNodenumInNetworkTopology(dims));
    }
}

TEST(torus_network_topology, throw_when_nodenum_in_network_too_many) {
    int procnum;
    std::vector<int> dims(1);
    MPI_Comm comm_cart = MPI_COMM_NULL;
    MPI_Comm_size(MPI_COMM_WORLD, &procnum);
    dims[0] = procnum + 1;

    ASSERT_ANY_THROW(comm_cart = createTorusTopology(dims));
    EXPECT_EQ(MPI_COMM_NULL, comm_cart);
}

TEST(torus_network_topology, can_create_torus_topology) {
    int procnum_in_base_comm;
    std::vector<int> dims(2);
    MPI_Comm comm_base = MPI_COMM_WORLD;
    MPI_Comm_size(comm_base, &procnum_in_base_comm);
    MPI_Dims_create(procnum_in_base_comm, 2, dims.data());

    ASSERT_NO_THROW(createTorusTopology(dims, comm_base));
}

TEST(torus_network_topology, type_created_topology_is_cart) {
    int procnum_in_base_comm, type;
    std::vector<int> dims(2);
    MPI_Comm comm_cart = MPI_COMM_NULL, comm_base = MPI_COMM_WORLD;
    MPI_Comm_size(comm_base, &procnum_in_base_comm);
    MPI_Dims_create(procnum_in_base_comm, 2, dims.data());
    comm_cart = createTorusTopology(dims, comm_base);

    MPI_Topo_test(comm_cart, &type);

    EXPECT_EQ(MPI_CART, type);
}

TEST(torus_network_topology, creating_topology_changes_comm_cart) {
    int procnum_in_base_comm;
    std::vector<int> dims(2);
    MPI_Comm comm_cart = MPI_COMM_NULL, comm_base = MPI_COMM_WORLD;
    MPI_Comm_size(comm_base, &procnum_in_base_comm);
    MPI_Dims_create(procnum_in_base_comm, 2, dims.data());
    comm_cart = createTorusTopology(dims, comm_base);

    EXPECT_EQ(1, comm_cart != MPI_COMM_NULL);
}

TEST(torus_network_topology, procnum_in_tor_top_is_equal_procnum_in_net_top) {
    int procnum_in_base_comm, procnum_in_cart_comm;
    std::vector<int> dims(2);
    MPI_Comm comm_cart = MPI_COMM_NULL, comm_base = MPI_COMM_WORLD;
    MPI_Comm_size(comm_base, &procnum_in_base_comm);
    MPI_Dims_create(procnum_in_base_comm, 2, dims.data());
    comm_cart = createTorusTopology(dims, comm_base);
    MPI_Comm_size(comm_cart, &procnum_in_cart_comm);

    EXPECT_EQ(getNodenumInNetworkTopology(dims), procnum_in_cart_comm);
}

TEST(torus_network_topology, throw_when_send_from_proc_which_is_not_in_top) {
    int procnum_in_base_comm, root, size = 10;
    std::vector<int> dims(2), data(size);
    std::vector<int>* pdata = &data;
    MPI_Comm comm_cart = MPI_COMM_NULL, comm_base = MPI_COMM_WORLD;
    MPI_Comm_size(comm_base, &procnum_in_base_comm);
    MPI_Dims_create(procnum_in_base_comm, 2, dims.data());
    comm_cart = createTorusTopology(dims, comm_base);
    root = procnum_in_base_comm;

    ASSERT_ANY_THROW(send(pdata, 1, MPI_INT, root, 0, dims, comm_cart));
}

TEST(torus_network_topology, throw_when_send_to_proc_which_is_not_in_top) {
    int procnum_in_base_comm, dest, size = 10;
    std::vector<int> dims(2), data(size);
    std::vector<int>* pdata = &data;
    MPI_Comm comm_cart = MPI_COMM_NULL, comm_base = MPI_COMM_WORLD;
    MPI_Comm_size(comm_base, &procnum_in_base_comm);
    MPI_Dims_create(procnum_in_base_comm, 2, dims.data());
    comm_cart = createTorusTopology(dims, comm_base);
    dest = procnum_in_base_comm;

    ASSERT_ANY_THROW(send(pdata, 1, MPI_INT, 0, dest, dims, comm_cart));
}

TEST(torus_network_topology, throw_when_try_send_in_null_comm) {
    MPI_Comm comm_cart = MPI_COMM_NULL;
    int size = 10;
    std::vector<int> data(size), dims;
    std::vector<int>* pdata = &data;

    ASSERT_ANY_THROW(send(pdata, 1, MPI_INT, 0, 0, dims, comm_cart));
}

TEST(torus_network_topology, can_send) {
    int procrank, procnum, root, dest, ndims = 2, size = 10;
    size_t count = 6;
    std::vector<int> data(size), dims(ndims);
    std::vector<int>* pdata = &data;
    MPI_Comm_size(MPI_COMM_WORLD, &procnum);
    if (procnum != 1) {
        MPI_Dims_create(procnum, ndims, dims.data());
        MPI_Comm comm_cart = createTorusTopology(dims, MPI_COMM_WORLD);
        MPI_Comm_rank(comm_cart, &procrank);
        root = 0; dest = procnum-1;
        if (procrank == root) {
            for (size_t i = 0; i < pdata->size(); ++i)
                pdata->at(i) = i * i;
        }

        if (procrank == dest && dest != root) {
            for (int i = 0; i < size; ++i)
                ASSERT_EQ(0, pdata->at(i));
        }

        send(pdata, count, MPI_INT, root, dest, dims, comm_cart);

        if (procrank == dest) {
            ASSERT_EQ(count, pdata->size());
            for (size_t i = 0; i < count; ++i)
                ASSERT_EQ(static_cast<int>(i*i), pdata->at(i));
        }
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}

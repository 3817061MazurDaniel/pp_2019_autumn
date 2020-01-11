// Copyright 2019 Kudryashov Nikita
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./hypercube.h"

TEST(HCubeTopology, can_create_hcube) {
    MPI_Comm Hcube;
    int size;
    unsigned int dim;

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < 32; i++) {
        if (size & (1 << i)) {
            dim = i;
        }
    }

    ASSERT_NO_THROW(Hcube = createHcube(dim));
    if (Hcube != MPI_COMM_NULL) {
        MPI_Comm_size(Hcube, &size);
    }
}

TEST(HCubeTopology, topology_check) {
    MPI_Comm Hcube;
    int status, size;
    unsigned int dim;

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < 32; i++) {
        if (size & (1 << i)) {
            dim = i;
        }
    }

    Hcube = createHcube(dim);

    if (Hcube != MPI_COMM_NULL) {
        MPI_Topo_test(Hcube, &status);
        ASSERT_EQ(status, MPI_CART);
    }
}

TEST(HCubeTopology, param_dims_check) {
    MPI_Comm Hcube;
    int dims, size;
    unsigned int dim;

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < 32; i++) {
        if (size & (1 << i)) {
            dim = i;
        }
    }

    Hcube = createHcube(dim);

    if (Hcube != MPI_COMM_NULL) {
        MPI_Cartdim_get(Hcube, &dims);
        ASSERT_EQ(dims, (int)dim);
    }
}

TEST(HCubeTopology, param_periods_check) {
    MPI_Comm Hcube;
    int dims, size;
    unsigned int dim_s;

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < 32; i++) {
        if (size & (1 << i)) {
            dim_s = i;
        }
    }

    Hcube = createHcube(dim_s);

    if (Hcube != MPI_COMM_NULL) {
        MPI_Cartdim_get(Hcube, &dims);

        int* dim = new int[dims];
        int* period = new int[dims];
        int* coords = new int[dims];
        bool check = true;

        MPI_Cart_get(Hcube, dims, dim, period, coords);

        for (int i = 0; i < dims; i++) {
            if (period[i] != 0) {
                check = false;
                break;
            }
        }
        ASSERT_EQ(check, true);
    }
}

TEST(HCubeTopology, param_coords_check) {
    MPI_Comm Hcube;
    int dims, size, rank;
    unsigned int dim_s;

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < 32; i++) {
        if (size & (1 << i)) {
            dim_s = i;
        }
    }

    Hcube = createHcube(dim_s);

    if (Hcube != MPI_COMM_NULL) {
        MPI_Cartdim_get(Hcube, &dims);
        MPI_Comm_rank(Hcube, &rank);

        int* dim = new int[dims];
        int* period = new int[dims];
        int* coords = new int[dims];
        int* rank_coords = new int[dims];
        bool check = true;

        MPI_Cart_get(Hcube, dims, dim, period, coords);
        MPI_Cart_coords(Hcube, rank, dims, rank_coords);

        for (int i = 0; i < dims; i++) {
            if (rank_coords[i] != coords[i]) {
                check = false;
                break;
            }
        }
        ASSERT_EQ(check, true);
    }
}

TEST(HCubeTopology, param_dim_array_check) {
    MPI_Comm Hcube;
    int dims, size;
    unsigned int dim_s;

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < 32; i++) {
        if (size & (1 << i)) {
            dim_s = i;
        }
    }

    Hcube = createHcube(dim_s);

    if (Hcube != MPI_COMM_NULL) {
        MPI_Cartdim_get(Hcube, &dims);

        int* dim = new int[dims];
        int* period = new int[dims];
        int* coords = new int[dims];
        bool check = true;

        MPI_Cart_get(Hcube, dims, dim, period, coords);

        for (int i = 0; i < dims; i++) {
            if (dim[i] != 2) {
                check = false;
                break;
            }
        }
        ASSERT_EQ(check, true);
    }
}

TEST(HCubeTopology, can_send_msg) {
    MPI_Comm Hcube;
    int rank, size;
    unsigned int dim_s;

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < 32; i++) {
        if (size & (1 << i)) {
            dim_s = i;
        }
    }

    Hcube = createHcube(dim_s);
    if (Hcube != MPI_COMM_NULL) {
        MPI_Comm_rank(Hcube, &rank);
        MPI_Comm_size(Hcube, &size);
        if (size > 1) {
            if (rank == 0) {
                int msg = 15;

                MPI_Send(&msg, 1, MPI_INT, size - 1, 1, Hcube);
            }
            if (rank == size - 1) {
                int msg, ret;
                MPI_Status status;

                ret = MPI_Recv(&msg, 1, MPI_INT, 0, 1, Hcube, &status);

                ASSERT_TRUE(ret == MPI_SUCCESS && msg == 15);
            }
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

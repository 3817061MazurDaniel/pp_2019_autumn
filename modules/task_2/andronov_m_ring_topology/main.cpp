// Copyright 2019 Andronov Maxim
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
// #include <iostream>
#include "../../../modules/task_2/andronov_m_ring_topology/ring_topology.h"

TEST(Ring_Topology, can_create_ring_topology) {
    MPI_Comm ringcomm = CreateRingTopology(MPI_COMM_WORLD);

    EXPECT_TRUE(ringcomm != MPI_COMM_NULL);
}

TEST(Ring_Topology, test_ring_topology) {
    MPI_Comm ringcomm = CreateRingTopology(MPI_COMM_WORLD);

    EXPECT_TRUE(IsRingTopology(ringcomm));
}

TEST(Ring_Topology, no_ring_topology_if_no_cart) {
    EXPECT_FALSE(IsRingTopology(MPI_COMM_WORLD));
}

TEST(Ring_Topology, no_ring_topology_if_ndims_not_equal_one) {
    MPI_Comm testcomm;
    std::vector<int> dims(2, 0), periods(2, 1);
    int size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Dims_create(size, 2, dims.data());
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims.data(), periods.data(), 0, &testcomm);

    EXPECT_FALSE(IsRingTopology(testcomm));
}

TEST(Ring_Topology, no_ring_topology_if_period_not_equal_one) {
    MPI_Comm testcomm;
    std::vector<int> dims(1), periods(1, 0);

    MPI_Comm_size(MPI_COMM_WORLD, &dims[0]);
    MPI_Cart_create(MPI_COMM_WORLD, 1, dims.data(), periods.data(), 0, &testcomm);

    EXPECT_FALSE(IsRingTopology(testcomm));
}

TEST(Ring_Topology, test_ring_topology_on_dest_and_source) {
    MPI_Comm ringcomm = CreateRingTopology(MPI_COMM_WORLD);

    int rank, size, source, dest;
    MPI_Comm_rank(ringcomm, &rank);
    MPI_Comm_size(ringcomm, &size);
    MPI_Cart_shift(ringcomm, 0, 1, &source, &dest);

    if (size == 1) {
        EXPECT_EQ(0, dest);
        EXPECT_EQ(0, source);
    } else if (rank == 0) {
        EXPECT_EQ(1, dest);
        EXPECT_EQ((size - 1), source);
    } else if (rank == (size - 1)) {
        EXPECT_EQ(0, dest);
        EXPECT_EQ((rank - 1), source);
    } else {
        EXPECT_EQ((rank + 1), dest);
        EXPECT_EQ((rank - 1), source);
    }
}

TEST(Ring_Topology, send_message) {
    MPI_Comm ringcomm = CreateRingTopology(MPI_COMM_WORLD);
    int rank, size;
    MPI_Comm_rank(ringcomm, &rank);
    MPI_Comm_size(ringcomm, &size);

    std::vector<int> message;

    if (rank == 0) {
        message.resize(3);
        for (size_t i = 0; i < message.size(); i++)
            message[i] = 1;
    }

    std::vector<int> result = Send(ringcomm, 0, (size-1), message, 3);

    if (rank == (size - 1)) {
        std::vector<int> emessage(3, 1);
        EXPECT_EQ(emessage, result);
    }
}

TEST(Ring_Topology, send_message_opposite) {
    MPI_Comm ringcomm = CreateRingTopology(MPI_COMM_WORLD);
    int rank, size;
    MPI_Comm_rank(ringcomm, &rank);
    MPI_Comm_size(ringcomm, &size);

    std::vector<int> message;

    if (rank == (size - 1)) {
        message.resize(3);
        for (size_t i = 0; i < message.size(); i++)
            message[i] = 1;
    }

    std::vector<int> result = Send(ringcomm, (size - 1), 0, message, 3);

    if (rank == 0) {
        std::vector<int> emessage(3, 1);
        EXPECT_EQ(emessage, result);
    }
}

TEST(Ring_Topology, cant_send_if_source_less_zero) {
    MPI_Comm ringcomm = CreateRingTopology(MPI_COMM_WORLD);
    int rank, size;
    MPI_Comm_rank(ringcomm, &rank);
    MPI_Comm_size(ringcomm, &size);

    std::vector<int> message(3);

    ASSERT_ANY_THROW(Send(ringcomm, -1, 0, message, 3));
}

TEST(Ring_Topology, cant_send_if_dest_less_zero) {
    MPI_Comm ringcomm = CreateRingTopology(MPI_COMM_WORLD);
    int rank, size;
    MPI_Comm_rank(ringcomm, &rank);
    MPI_Comm_size(ringcomm, &size);

    std::vector<int> message(3);

    ASSERT_ANY_THROW(Send(ringcomm, 0, -1, message, 3));
}

TEST(Ring_Topology, cant_send_if_dest_more_size) {
    MPI_Comm ringcomm = CreateRingTopology(MPI_COMM_WORLD);
    int rank, size;
    MPI_Comm_rank(ringcomm, &rank);
    MPI_Comm_size(ringcomm, &size);

    std::vector<int> message(3);

    ASSERT_ANY_THROW(Send(ringcomm, 0, size, message, 3));
}

TEST(Ring_Topology, cant_send_if_source_more_size) {
    MPI_Comm ringcomm = CreateRingTopology(MPI_COMM_WORLD);
    int rank, size;
    MPI_Comm_rank(ringcomm, &rank);
    MPI_Comm_size(ringcomm, &size);

    std::vector<int> message(3);

    ASSERT_ANY_THROW(Send(ringcomm, size, 0, message, 3));
}

TEST(Ring_Topology, can_send_to_itself) {
    MPI_Comm ringcomm = CreateRingTopology(MPI_COMM_WORLD);
    int rank, size;
    MPI_Comm_rank(ringcomm, &rank);
    MPI_Comm_size(ringcomm, &size);

    std::vector<int> message;

    if (rank == 0) {
        message.resize(3);
        for (size_t i = 0; i < message.size(); i++)
            message[i] = 1;
    }

    std::vector<int> result = Send(ringcomm, 0, 0, message, 3);

    if (rank == 0) {
        std::vector<int> emessage(3, 1);
        EXPECT_EQ(emessage, result);
    }
}

TEST(Ring_Topology, Performance_Test_On_Vector_Sum) {
    int rank;
    MPI_Comm ringcomm = CreateRingTopology(MPI_COMM_WORLD);
    MPI_Comm_rank(ringcomm, &rank);

    // double start_world_comm, end_world_comm, time_world_comm;
    // double start_ring_topo, end_ring_topo, time_ring_topo;
    std::vector<int> global_vec;
    const int count_size_vector = 999;

    if (rank == 0) {
        global_vec = GetRandomVector(count_size_vector);
    }

    // start_world_comm = MPI_Wtime();
    int global_sum_parallel_world_comm = ParallelSum(global_vec,
        count_size_vector, MPI_COMM_WORLD);
    /*end_world_comm = MPI_Wtime();
    time_world_comm = end_world_comm - start_world_comm;
    if (rank == 0)
        std::cout << time_world_comm << std::endl;*/

        // start_ring_topo = MPI_Wtime();
    int global_sum_parallel_ring_topology = ParallelSum(global_vec,
        count_size_vector, ringcomm);
    /*end_ring_topo = MPI_Wtime();
    time_ring_topo = end_ring_topo - start_ring_topo;
    if (rank == 0)
        std::cout << time_ring_topo << std::endl;*/

    if (rank == 0) {
        int global_sum_seq = SeqSum(global_vec);

        EXPECT_EQ(global_sum_seq, global_sum_parallel_world_comm);
        EXPECT_EQ(global_sum_seq, global_sum_parallel_ring_topology);
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

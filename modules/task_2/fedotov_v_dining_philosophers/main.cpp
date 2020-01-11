// Copyright 2019 Fedotov Vlad
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./dining_philosophers.h"

TEST(Parallel_Operations_MPI, Test_Philosophers_Ended_Their_Job) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int philosophers_count = size - 1;  // one process is waiter, others - philosophers

    if (rank == 0) {
        std::vector<bool> current_all_ate_eating_cycle_times(philosophers_count, false);
        std::vector<bool> expected_all_ate_eating_cycle_times(philosophers_count, true);
        current_all_ate_eating_cycle_times = waiter(philosophers_count);

        ASSERT_EQ(current_all_ate_eating_cycle_times, expected_all_ate_eating_cycle_times);
    } else {
        philosopher(rank);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    // int rank, size;
    MPI_Init(&argc, &argv);
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // MPI_Comm_size(MPI_COMM_WORLD, &size);
    // int philosophers_count = size - 1;  // one process is waiter, others - philosophers

    // if(rank == 0)
    //     waiter(philosophers_count);
    // else
    //     philosopher(rank);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);

    return RUN_ALL_TESTS();
    // upd because of travis problem
}

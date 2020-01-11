// Copyright 2019 Evdokimov Artem
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <array>
#include "./philosophers.h"

TEST(MPI_Hpilosophers, No_starvation_and_deadlock) {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    ASSERT_NO_THROW(philosophersHasLunch(rank, size));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    srand(time(NULL));
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);

    return RUN_ALL_TESTS();
    // upd because of travis problem
}

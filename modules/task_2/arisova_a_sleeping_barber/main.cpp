  // Copyright 2019 Arisova Anastasiia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <queue>
#include "./sleeping_barber.h"

TEST(Task_Sleeping_Barber, Test_1) {
    int size;
    int ans;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    ans = BarberShop(5, 3);
    EXPECT_EQ(ans, size - 1);
}

TEST(Task_Sleeping_Barber, Test_2) {
    int size;
    int ans;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    ans = BarberShop(0, 10);
    EXPECT_EQ(ans, size - 1);
}

TEST(Task_Sleeping_Barber, Test_3) {
    int size;
    int ans;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    ans = BarberShop(85, 1);
    EXPECT_EQ(ans, size - 1);
}

TEST(Task_Sleeping_Barber, Test_4) {
    ASSERT_ANY_THROW({BarberShop(1, 0);});
}

TEST(Task_Sleeping_Barber, Test_5) {
    int size;
    int ans;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    ans = BarberShop(3, 5);
    EXPECT_EQ(ans, size - 1);
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

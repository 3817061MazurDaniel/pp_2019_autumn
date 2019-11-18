// Copyright 2019 Devlikamov Vladislav

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_2/devlikamov_v_broadcast/devlikamov_v_broadcast.h"

TEST(Broadcast_Mpi, test1) {
    int data;
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank == 0) {
      data = 100;
      my_bcast_slow(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
      my_bcast_slow(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
      EXPECT_EQ(data, 100);
    }
}

TEST(Broadcast_Mpi, test2) {
    int data;
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank == 0) {
      data = 100;
      my_bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
      my_bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
      EXPECT_EQ(data, 100);
    }
}

TEST(Broadcast_Mpi, test3) {
    double data;
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank == 0) {
      data = 1.0;
      my_bcast_slow(&data, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } else {
      my_bcast_slow(&data, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      EXPECT_EQ(data, 1.0);
    }
}

TEST(Broadcast_Mpi, test4) {
    int data;
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank == 0) {
      data = 1.0;
      my_bcast(&data, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } else {
      my_bcast(&data, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      EXPECT_EQ(data, 1.0);
    }
}

TEST(Broadcast_Mpi, test5) {
    int* data = reinterpret_cast<int*>(malloc(sizeof(int) * 2));
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank == 0) {
      data[0] = 1;
      data[1] = 2;
      my_bcast_slow(data, 2, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
      my_bcast_slow(data, 2, MPI_INT, 0, MPI_COMM_WORLD);
      EXPECT_EQ(data[0], 1);
    }
}


TEST(Broadcast_Mpi, test6) {
    double* data = reinterpret_cast<double*>(malloc(sizeof(int) * 2));
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank == 0) {
      data[0] = 1.0;
      data[1] = 2.0;
      my_bcast(data, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } else {
      my_bcast(data, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      EXPECT_EQ(data[0], 1.0);
    }
}

TEST(Broadcast_Mpi, test7) {
    std::vector <int> a(1);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank == 0) {
      a[0] = 1;
      my_bcast_slow(&a[0], 1, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
      my_bcast_slow(&a[0], 1, MPI_INT, 0, MPI_COMM_WORLD);
      EXPECT_EQ(a[0], 1);
    }
}


TEST(Broadcast_Mpi, test8) {
    double* data = reinterpret_cast<double*>(malloc(sizeof(int) * 2));
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank == 0) {
      data[0] = 1.0;
      data[1] = 2.0;
      my_bcast_slow(data, 2, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
      my_bcast_slow(data, 2, MPI_INT, 0, MPI_COMM_WORLD);
      EXPECT_EQ(data[0], 1.0);
    }
}

TEST(Broadcast_Mpi, test9) {
    int world_rank;
    char data;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank == 0) {
      data = 'a';
      my_bcast_slow(&data, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    } else {
      my_bcast_slow(&data, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
      EXPECT_EQ(data, 'a');
    }
}

TEST(Broadcast_Mpi, test10) {
    int world_rank;
    char data;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank == 0) {
      data = 'a';
      my_bcast(&data, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    } else {
      my_bcast(&data, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
      EXPECT_EQ(data, 'a');
    }
}

TEST(Broadcast_Mpi, test11) {
    std::vector <char> a(2);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank == 0) {
      a[1] = 'z';
      my_bcast_slow(&a[0], 2, MPI_CHAR, 0, MPI_COMM_WORLD);
    } else {
      my_bcast_slow(&a[0], 2, MPI_CHAR, 0, MPI_COMM_WORLD);
      EXPECT_EQ(a[1], 'z');
    }
}

TEST(Broadcast_Mpi, test12) {
    std::vector <char> a(2);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank == 0) {
      a[1] = 'z';
      my_bcast(&a[0], 2, MPI_CHAR, 0, MPI_COMM_WORLD);
    } else {
      my_bcast(&a[0], 2, MPI_CHAR, 0, MPI_COMM_WORLD);
      EXPECT_EQ(a[1], 'z');
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

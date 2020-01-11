// Copyright 2019 Suslov Egor
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./producers_consumers.h"

TEST(Producer_Consumer, Producer_Test1) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int kol_elem_in_buffer = 100;
    int *buffer = new int[kol_elem_in_buffer];
    for (int i = 0; i < kol_elem_in_buffer; i++) {
        buffer[i]=-1;
    }
    int kol_resursov = 5;
    if (size == 1) {
        for (int i = 0; i < kol_resursov; i++) {
            Producer(buffer, kol_elem_in_buffer, rank, 1);
        }
        for (int i = 0; i < kol_resursov; i++) {
            ASSERT_EQ(buffer[i], 1);
        }
    } else {
        for (int i = 0; i < size; i++) {
            Producer(buffer, kol_elem_in_buffer, 1, 2);
        }
        if (rank == 0) {
            for (int i = 0; i < size; i++) {
                ASSERT_EQ(buffer[i], 2);
            }
        }
    }
}

TEST(Producer_Consumer, Producer_Test2) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int kol_elem_in_buffer = 100;
    int *buffer = new int[kol_elem_in_buffer];
    for (int i = 0; i < kol_elem_in_buffer; i++) {
        buffer[i] = -1;
    }
    int kol_resursov = 10;
    if (size == 1) {
        for (int i = 0; i < kol_resursov; i++) {
            Producer(buffer, kol_elem_in_buffer, rank, 1);
        }
        for (int i = 0; i < kol_resursov; i++) {
            ASSERT_EQ(buffer[i], 1);
        }
    } else {
        for (int i = 0; i < kol_resursov; i++) {
            Producer(buffer, kol_elem_in_buffer, 1, 2);
        }
        if (rank == 0) {
            for (int i = 0; i < size; i++) {
                ASSERT_EQ(buffer[i], 2);
            }
        }
    }
}

TEST(Producer_Consumer, Producer_Test3) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int kol_elem_in_buffer = 100;
    int *buffer = new int[kol_elem_in_buffer];
    for (int i = 0; i < kol_elem_in_buffer; i++) {
        buffer[i] = -1;
    }
    int kol_resursov = 5;
    if (size == 1) {
        for (int i = 0; i < kol_resursov; i++) {
            Producer(buffer, kol_elem_in_buffer, rank, 1);
        }
        if (rank == 0) {
            for (int i = 0; i < kol_resursov; i++) {
                ASSERT_EQ(buffer[i], 1);
            }
        }
    } else {
        Producer(buffer, kol_elem_in_buffer, 1, 2);
        if (rank == 0) {
            ASSERT_EQ(buffer[0], 2);
        }
    }
}

TEST(Producer_Consumer, Consumer_Test1) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int kol_elem_in_buffer = 100;
    int *buffer = new int[kol_elem_in_buffer];
    for (int i = 0; i < kol_elem_in_buffer; i++) {
        buffer[i] = -1;
    }
    int kol_resursov = 5;
    if (size == 1) {
        for (int i = 0; i < kol_resursov; i++) {
            Producer(buffer, kol_elem_in_buffer, rank, 1);
        }
        int *resurce_consume = new int[kol_elem_in_buffer];
        for (int i = 0; i < kol_elem_in_buffer; i++) {
            resurce_consume[i] = -1;
        }
        for (int i = 0; i < kol_resursov; i++) {
            Consumer(buffer, kol_elem_in_buffer, 0, &resurce_consume[i]);
            ASSERT_EQ(1, resurce_consume[i]);
        }
    } else {
        for (int i = 0; i < size; i++) {
            Producer(buffer, kol_elem_in_buffer, 1, 2);
        }
        if (rank == 0) {
            for (int i = 0; i < size; i++) {
                ASSERT_EQ(buffer[i], 2);
            }
        }
    }
}

TEST(Producer_Consumer, Producer_Consumer_Test1) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int kol_elem_in_buffer = 100;
    int *buffer = new int[kol_elem_in_buffer];
    for (int i = 0; i < kol_elem_in_buffer; i++) {
        buffer[i] = -1;
    }
    int kol_resursov = 10;
    if (size == 1) {
        for (int i = 0; i < kol_resursov; i++) {
            Producer(buffer, kol_elem_in_buffer, rank, 1);
        }
        std::vector<int> resurce_consume1(kol_resursov, -1);
        int *resurce_consume;
        resurce_consume = Create_dinamic_massiv_from_vector(resurce_consume1);
        for (int i = 0; i < kol_resursov; i++) {
            Consumer(buffer, kol_elem_in_buffer, rank, &resurce_consume[i]);
            ASSERT_EQ(1, resurce_consume[i]);
        }
    } else {
        for (int i = 0; i < kol_resursov; i++) {
            Producer(buffer, kol_elem_in_buffer, 1, 3);
        }
        std::vector<int> resurce_consume1(kol_resursov, -1);
        int *resurce_consume;
        resurce_consume = Create_dinamic_massiv_from_vector(resurce_consume1);
        for (int i = 0; i < kol_resursov; i++) {
            Consumer(buffer, kol_elem_in_buffer, 1, &resurce_consume[i]);
        }
        if (rank == 1) {
            for (int i = 0; i < kol_resursov; i++) {
                ASSERT_EQ(3, resurce_consume[i]);
            }
        }
    }
}
TEST(Producer_Consumer, Producer_Consumer_Test2) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int kol_elem_in_buffer = 100;
    int *buffer = new int[kol_elem_in_buffer];
    for (int i = 0; i < kol_elem_in_buffer; i++) {
        buffer[i] = -1;
    }
    int kol_resursov = 15;
    if (size == 1) {
        for (int i = 0; i < kol_resursov; i++) {
            Producer(buffer, kol_elem_in_buffer, rank, 1);
        }
        std::vector<int> resurce_consume1(kol_resursov, -1);
        int *resurce_consume;
        resurce_consume = Create_dinamic_massiv_from_vector(resurce_consume1);
        for (int i = 0; i < kol_resursov; i++) {
            Consumer(buffer, kol_elem_in_buffer, rank, &resurce_consume[i]);
            ASSERT_EQ(1, resurce_consume[i]);
        }
    } else {
        for (int i = 0; i < kol_resursov; i++) {
            Producer(buffer, kol_elem_in_buffer, 1, 5);
        }
        std::vector<int> resurce_consume1(kol_resursov, -1);
        int *resurce_consume;
        resurce_consume = Create_dinamic_massiv_from_vector(resurce_consume1);
        for (int i = 0; i < kol_resursov; i++) {
            Consumer(buffer, kol_elem_in_buffer, 0, &resurce_consume[i]);
        }
        if (rank == 0) {
            for (int i = 0; i < kol_resursov; i++) {
                ASSERT_EQ(5, resurce_consume[i]);
            }
        }
        if (rank == 1) {
            for (int i = 0; i < kol_resursov; i++) {
                ASSERT_EQ(-1, resurce_consume[i]);
            }
        }
    }
}

TEST(Producer_Consumer, Producer_Consumer_Test3) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int kol_elem_in_buffer = 100;
    int *buffer = new int[kol_elem_in_buffer];
    for (int i = 0; i < kol_elem_in_buffer; i++) {
        buffer[i] = -1;
    }
    int kol_resursov = 7;
    if (size == 1) {
        for (int i = 0; i < kol_resursov; i++) {
            Producer(buffer, kol_elem_in_buffer, rank, i);
        }
        std::vector<int> resurce_consume1(kol_resursov, -1);
        int *resurce_consume;
        resurce_consume = Create_dinamic_massiv_from_vector(resurce_consume1);
        for (int i = 0; i < kol_resursov; i++) {
            Consumer(buffer, kol_elem_in_buffer, rank, &resurce_consume[i]);
            ASSERT_EQ(i, resurce_consume[i]);
        }
    } else {
        for (int i = 0; i < kol_resursov; i++) {
            Producer(buffer, kol_elem_in_buffer, 1, i);
        }
        std::vector<int> resurce_consume1(kol_resursov, -1);
        int *resurce_consume;
        resurce_consume = Create_dinamic_massiv_from_vector(resurce_consume1);
        for (int i = 0; i < kol_resursov; i++) {
            Consumer(buffer, kol_elem_in_buffer, 1, &resurce_consume[i]);
        }
        if (rank == 1) {
            for (int i = 0; i < kol_resursov; i++) {
                ASSERT_EQ(i, resurce_consume[i]);
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



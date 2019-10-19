// Copyright 2019 Guseva Catherine

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./vector_average_value.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}

TEST(average_value_vector_MPI, Random_Vector_10000_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector <int> vector;

    const int vector_size = 10000;

    if (rank == 0) {
        vector = getRandVector(vector_size);
    }

    int mid_value_p = parallelMidValueVector(vector, vector_size);

    if (rank == 0) {
        int mid_value_s = sumVector(vector) / vector_size;
        EXPECT_EQ(mid_value_p, mid_value_s);
    }
}

TEST(average_value_vector_MPI, Even_Size_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector <int> vector;

    const int vector_size = 12;

    if (rank == 0) {
        vector = getRandVector(vector_size);
    }

    int mid_value_p = parallelMidValueVector(vector, vector_size);

    if (rank == 0) {
        int mid_value_s = sumVector(vector) / vector_size;
        EXPECT_EQ(mid_value_p, mid_value_s);
    }
}

TEST(average_value_vector_MPI, Odd_Size_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector <int> vector;

    const int vector_size = 13;

    if (rank == 0) {
        vector = getRandVector(vector_size);
    }

    int mid_value_p = parallelMidValueVector(vector, vector_size);

    if (rank == 0) {
        int mid_value_s = sumVector(vector) / vector_size;
        EXPECT_EQ(mid_value_p, mid_value_s);
    }
}

TEST(average_value_vector_MPI, Equal_Size_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector <int> vector;

    if (rank == 0) {
        vector = { 9, 9, 9, 9, 9, 9, 9, 9, 9 };
    }

    int mid_value_p = parallelMidValueVector(vector, 9);

    if (rank == 0) {
        int mid_value_s = sumVector(vector) / 9;
        EXPECT_EQ(mid_value_p, mid_value_s);
    }
}

TEST(average_value_vector_MPI, Can_Detect_Wrong_Size) {
    std::vector <int> vector;
    const int vector_size = -100;

    ASSERT_ANY_THROW(vector = getRandVector(vector_size));
}

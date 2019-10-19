// Copyright Dudchenko Anton 2019
#include <mpi.h>
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <vector>
#include <algorithm>
#include "./max_vector_value.h"

TEST(Vector_Max_Value, Test_Vector_Generation) {
    int rank;
    unsigned int size = 100;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        return;
    }
    std::vector<int> randVec = getVector(size);
    EXPECT_EQ(size, randVec.size());
}

TEST(Vector_Max_Value, Test_Empty_Vector) {
    int rank;
    std::vector <int> vec;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vec = getVector(0);
    EXPECT_THROW(getMaxVectorValue(vec), std::runtime_error);
}

TEST(Vector_Max_Value, Test_Small_Vector) {
    int rank;
    std::vector <int> vec;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vec = getVector(2);
    int mpi_res = getMaxVectorValue(vec);
    if (rank == 0) {
        int res = *max_element(vec.begin(), vec.end());
        EXPECT_EQ(res, mpi_res);
    }
}

TEST(Vector_Max_Value, Test_Big_Vector) {
    int rank;
    std::vector <int> vec;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vec = getVector(1000000);
    int mpi_res = getMaxVectorValue(vec);
    if (rank == 0) {
        int res = *max_element(vec.begin(), vec.end());
        EXPECT_EQ(res, mpi_res);
    }
}

int main(int argc, char **argv) {
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

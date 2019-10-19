// Copyright 2019 Savosina
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "./sum_string.h"

TEST(Parallel_String_Sum_MPI, Test_Create_Vector_Char) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 5;
    std::vector<char> global_vec(n);
    if (rank == 0) {
        ASSERT_NO_THROW(global_vec = getRandomVector(n));
    }
}

TEST(Parallel_String_Sum_MPI, Test_Create_Negative_Vector_Char) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = -5;
    std::vector<char> global_vec(5);
    if (rank == 0) {
        ASSERT_ANY_THROW(global_vec = getRandomVector(n));
    }
}

TEST(Parallel_String_Sum_MPI, Test_Sum_Vector_3_Symbols_Only_Sequential) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 2;
    std::vector<char> global_str(3);
    global_str = {'f', '\n', 'g'};
    if (rank == 0) {
        int reference_sum = getSequentialOperationsVector(global_str);
        ASSERT_EQ(reference_sum, n);
    }
}

TEST(Parallel_String_Sum_MPI, Test_Sum_Vector_3_Symbols_Only_Parallel) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 3;
    std::vector<char> global_str(3);
    int sum = 2;
    global_str = {'d', '\n', 'd'};
    int global_sum = getParallelOperationsVectorTry(global_str, n);
    if (rank == 0) {
        ASSERT_EQ(global_sum, sum);
    }
}

TEST(Parallel_String_Sum_MPI, Test_Sum_Short_Vector_Two_Operations) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 7;
    std::vector<char> global_str(n);
    if (rank == 0) {
        global_str = getRandomVector(n);
    }
    int global_sum1 = getParallelOperationsVectorTry(global_str, n);
    if (rank == 0) {
        int reference_sum = getSequentialOperationsVector(global_str);
        ASSERT_EQ(reference_sum, global_sum1);
    }
}

TEST(Parallel_String_Sum_MPI, Test_Sum_Long_Vector_Two_Operations) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 170;
    std::vector<char> global_str(n);
    if (rank == 0) {
        global_str = getRandomVector(n);
    }
    int global_sum = getParallelOperationsVectorTry(global_str, n);
    if (rank == 0) {
        int reference_sum = getSequentialOperationsVector(global_str);
        ASSERT_EQ(reference_sum, global_sum);
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

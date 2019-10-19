// Copyright 2019 Shemetov Philipp
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./sum_vector.h"

TEST(Sum_Vector, Test_Wrong_Size_Random_Vector) {
    std::vector<int> vectorTest;
    ASSERT_ANY_THROW(vectorTest = getRandomVector(-100));
}

TEST(Sum_Vector, Test_Wrong_Size_Vector_In_Function_Get_Parallel_Sum_Vector) {
    std::vector<int> vectorTest;
    ASSERT_ANY_THROW(vectorTest = getRandomVector(-100));
}

TEST(Sum_Vector, Test_Wrong_Size_Get_Vector) {
    std::vector<int> vectorTest;
    ASSERT_ANY_THROW(vectorTest = getRandomVector(-100));
}

TEST(Sum_Vector, Test_Equal_Sum_Vectors_Without_Random) {
    int rankProc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    std::vector<int> vectorTest;
    if (rankProc == 0)
        vectorTest = getVector(50);
    int result = getParallelSumVector(vectorTest, 50);
    if (rankProc == 0) {
        int supposed = getLocalSum(vectorTest);
        EXPECT_EQ(supposed, result);
    }
}

TEST(Sum_Vector, Test_Equal_Sum_Vectors_Without_Parallel_And_With_Parallel) {
    int rankProc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    std::vector<int> vectorTest;
    if (rankProc == 0)
        vectorTest = getRandomVector(100);
    int result = getParallelSumVector(vectorTest, 100);
    if (rankProc == 0) {
        int supposed = getLocalSum(vectorTest);
        EXPECT_EQ(supposed, result);
    }
}

TEST(Sum_Vector, Test_Equal_Sum_Vectors_With_Great_Size_Value) {
    int rankProc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    std::vector<int> vectorTest;
    if (rankProc == 0)
        vectorTest = getRandomVector(1000000);
    int result = getParallelSumVector(vectorTest, 1000000);
    if (rankProc == 0) {
        int supposed = getLocalSum(vectorTest);
        EXPECT_EQ(supposed, result);
    }
}

TEST(Sum_Vector, Test_Equal_Sum_Vectors_On_Zero) {
    int rankProc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    std::vector<int> vectorTest;
    if (rankProc == 0)
        vectorTest = {0, 0, 0, 0};
    int result = getParallelSumVector(vectorTest, 4);
    EXPECT_EQ(0, result);
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

// Copyright 2019 Gribanov Mihail
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <random>
#include "./vector_difference.h"

TEST(Test_Vector_Difference, Test_Sequential_Difference_With_1_Elem) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> Vec = { 25 };
    if (rank == 0) {
        int res = getMaxDifferenceSeq(Vec);
        ASSERT_EQ(res, 0);
    }
}

TEST(Test_Vector_Difference, Test_Sequential_Difference) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> Vec = {0, 15, 21, 17, 11, 10, 100};
    if (rank == 0) {
        int res = getMaxDifferenceSeq(Vec);
        ASSERT_EQ(res, 90);
    }
}

TEST(Test_Vector_Difference, Test_MPI_Difference_With_1_Elem) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> Vec;
    if (rank == 0)
        Vec = getVector(1);
    int res = getMaxDifference(Vec);
    if (rank == 0) {
        ASSERT_EQ(res, getMaxDifferenceSeq(Vec));
    }
}

TEST(Test_Vector_Difference, Test_MPI_Difference_With_100_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> Vec;
    Vec = getVector(100);
    int res = getMaxDifference(Vec);
    if (rank == 0) {
        ASSERT_EQ(res, getMaxDifferenceSeq(Vec));
    }
}

TEST(Test_Vector_Difference, Test_MPI_Difference_With_1000_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> Vec;
    Vec = getVector(1000);
    int res = getMaxDifference(Vec);
    if (rank == 0) {
        ASSERT_EQ(res, getMaxDifferenceSeq(Vec));
    }
}

TEST(Test_Vector_Difference, Test_MPI_Difference_With_10000_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> Vec;
    Vec = getVector(10000);
    int res = getMaxDifference(Vec);
    if (rank == 0) {
        ASSERT_EQ(res, getMaxDifferenceSeq(Vec));
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

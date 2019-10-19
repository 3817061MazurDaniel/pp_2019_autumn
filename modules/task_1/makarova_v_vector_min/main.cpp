// Copyright 2019 Makarova Viktoria

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include <random>
#include <vector>
#include <algorithm>
#include "./vector_min.h"

TEST(Test_Vector_Min, Vector_Length_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> testVector;
    const int length = 100;

    if (rank == 0) {
        testVector = randomVector(length);
    }

    int result = minElem(testVector, length);

    if (rank == 0) {
        ASSERT_EQ(result, minElemSeq(testVector, length));
    }
}

TEST(Test_Vector_Min, Vector_Length_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> testVector;
    const int length = 1000;

    if (rank == 0) {
        testVector = randomVector(length);
    }

    int result = minElem(testVector, length);

    if (rank == 0) {
        ASSERT_EQ(result, minElemSeq(testVector, length));
    }
}

TEST(Test_Vector_Min, Vector_Length_500) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> testVector;
    const int length = 500;

    if (rank == 0) {
        testVector = randomVector(length);
    }

    int result = minElem(testVector, length);

    if (rank == 0) {
        ASSERT_EQ(result, minElemSeq(testVector, length));
    }
}

TEST(Test_Vector_Min, Vector_Min_Seq) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> testVector {2, 7, 5, 10, 123, 134};

    if (rank == 0) {
        ASSERT_EQ(2, minElemSeq(testVector, testVector.size()));
    }
}

TEST(Test_Vector_Min, Vector_Length_1234) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> testVector;
    const int length = 1234;

    if (rank == 0) {
        testVector = randomVector(length);
    }

    int result = minElem(testVector, length);

    if (rank == 0) {
        ASSERT_EQ(result, minElemSeq(testVector, length));
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

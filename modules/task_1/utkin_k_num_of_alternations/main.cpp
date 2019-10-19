// Copyright 2019 Utkin Konstantin
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./num_of_alternations.h"

TEST(Num_Of_Alternations, Test_Sequential_Operations_Size_0) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec = {};

    if (rank == 0) {
        int res = getSequentialOperations(vec);
        EXPECT_EQ(res, 0);
    }
}

TEST(Num_Of_Alternations, Test_Sequential_Operations_Size_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec = { 0 };

    if (rank == 0) {
        int res = getSequentialOperations(vec);
        EXPECT_EQ(res, 0);
    }
}

TEST(Num_Of_Alternations, Test_Sequential_Operations_Zeros) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sizeVec = 1000;
    std::vector<int> vec(sizeVec);
    for (int i = 0; i < sizeVec; ++i) {
        vec[i] = 0;
    }

    if (rank == 0) {
        int res = getSequentialOperations(vec);
        EXPECT_EQ(res, 0);
    }
}

TEST(Num_Of_Alternations, Test_Sequential_Operations_Size_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sizeVec = 1000;
    std::vector<int> vec(sizeVec);
    int coef = 1;
    for (int i = 0; i < sizeVec; ++i) {
        vec[i] = (i + 1) * coef;
        coef *= -1;
    }

    if (rank == 0) {
        int res = getSequentialOperations(vec);
        EXPECT_EQ(res, 999);
    }
}

TEST(Num_Of_Alternations, Test_Sequential_Operations_Size_1005) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sizeVec = 1005;
    std::vector<int> vec(sizeVec);
    int coef = 1;
    for (int i = 0; i < sizeVec; ++i) {
        vec[i] = (i + 1) * coef;
        coef *= -1;
    }

    if (rank == 0) {
        int res = getSequentialOperations(vec);
        EXPECT_EQ(res, 1004);
    }
}

TEST(Num_Of_Alternations, Test_Parallel_Operations_Size_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sizeVec = 1000;
    std::vector<int> vec = getRandomVector(sizeVec);
    int resPar = getParallelOperations(vec);
    if (rank == 0) {
        int resSeq = getSequentialOperations(vec);
        EXPECT_EQ(resSeq, resPar);
    }
}

TEST(Num_Of_Alternations, Test_Parallel_Operations_Size_1005) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sizeVec = 1005;
    std::vector<int> vec = getRandomVector(sizeVec);
    int resPar = getParallelOperations(vec);
    if (rank == 0) {
        int resSeq = getSequentialOperations(vec);
        EXPECT_EQ(resSeq, resPar);
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

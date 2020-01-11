// Copyright 2019 Trenina Elizaveta

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include "./shell_batcher_sort.h"

TEST(Shell, Test_Shell_Sort_Size_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        std::vector<int> exp(1, 0);
        std::vector<int> res = Shell(exp);
        EXPECT_EQ(exp, res);
    }
}

TEST(Shell, Test_Shell_Sort_Size_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int n = 100;
        std::vector<int> t = GetRandomVector(n);
        std::vector<int> exp(t);
        std::vector<int> res = Shell(t);
        std::sort(exp.begin(), exp.end());
        EXPECT_EQ(exp, res);
    }
}

TEST(Shell, Test_Shell_Sort_Size_101) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int n = 101;
        std::vector<int> t = GetRandomVector(n);
        std::vector<int> exp(t);
        std::vector<int> res = Shell(t);
        std::sort(exp.begin(), exp.end());
        EXPECT_EQ(exp, res);
    }
}


TEST(Shell, Test_Shell_Batcher_Sort_Size_6) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 6;
    std::vector<int> t = GetRandomVector(n);
    std::vector<int> exp(t);
    std::vector<int> res = Shell_Batcher(t);
    if (rank == 0) {
        exp = Shell(t);
        EXPECT_EQ(exp, res);
    }
}

TEST(Shell, Test_Shell_Batcher_Sort_Size_7) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 7;
    std::vector<int> t = GetRandomVector(n);
    std::vector<int> exp(t);
    std::vector<int> res = Shell_Batcher(t);
    if (rank == 0) {
        exp = Shell(exp);
        EXPECT_EQ(exp, res);
    }
}


TEST(Shell, Test_Shell_Batcher_Sort_Size_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 100;
    std::vector<int> t = GetRandomVector(n);
    std::vector<int> exp(t);
    std::vector<int> res = Shell_Batcher(t);
    if (rank == 0) {
        exp = Shell(exp);
        EXPECT_EQ(exp, res);
    }
}

TEST(Shell, Test_Shell_Batcher_Sort_Size_101) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 101;
    std::vector<int> t = GetRandomVector(n);
    std::vector<int> exp(t);
    std::vector<int> res = Shell_Batcher(t);
    if (rank == 0) {
        exp = Shell(exp);
        EXPECT_EQ(exp, res);
    }
}

TEST(Shell, Test_Shell_Batcher_Sort_Size_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 100;
    std::vector<int> t = GetRandomVector(n);
    std::vector<int> exp(t);
    std::vector<int> res = Shell_Batcher(t);
    if (rank == 0) {
        exp = Shell(exp);
        EXPECT_EQ(exp, res);
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

// Copyright 2019 Okmyanskiy Andrey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include "../../../modules/task_3/okmyanskiy_a_cannon_algorithm/cannon_algorithm.h"

double epsilon = 1E-5;
int range1 = 10;
int range2 = 50;
int range3 = 100;
TEST(Parallel_, Test_Equals_Parallel_And_Sequintial_Size_Number_x_10) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int Size = ProcNum * range1;
    int root = static_cast<int> (sqrt(ProcNum));
    double root2 = sqrt(ProcNum);
    if (fabs(root2 - root) <= epsilon) {
        if ((Size*Size) % ProcNum == 0) {
            std::vector<double> A = getRandomMatrix(Size*Size);
            std::vector<double> B = getRandomMatrix(Size*Size);
            std::vector<double> C_parallel(Size*Size);

            // double TotalTimeL = 0;
            // double startL = MPI_Wtime();
            C_parallel = getParallelMult(A, B, Size);
            // double endL = MPI_Wtime() - startL;

            if (ProcRank == 0) {
                // std::cout << "ProcNum: " << ProcNum;
                // std::cout << "\nSize: " << Size << "*" << Size << "\n";
                // double startW = MPI_Wtime();
                std::vector<double> C_sequintial = Multyplication(A, B);
                // double endW = MPI_Wtime() - startW;
                // std::cout << "Parallel: " << endL;
                // std::cout << "\nSequintial: " << endW << "\n";
                ASSERT_EQ(C_parallel, C_sequintial);
            }
        }
    }
}

TEST(Parallel_, Test_Equals_Parallel_And_Sequintial_Size_Number_x_50) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int Size = ProcNum * range2;
    int root = static_cast<int> (sqrt(ProcNum));
    double root2 = sqrt(ProcNum);
    if (fabs(root2 - root) <= epsilon) {
        if ((Size*Size) % ProcNum == 0) {
            std::vector<double> A = getRandomMatrix(Size*Size);
            std::vector<double> B = getRandomMatrix(Size*Size);
            std::vector<double> C_parallel(Size*Size);

            // double TotalTimeL = 0;
            // double startL = MPI_Wtime();
            C_parallel = getParallelMult(A, B, Size);
            // double endL = MPI_Wtime() - startL;

            if (ProcRank == 0) {
                // std::cout << "ProcNum: " << ProcNum;
                // std::cout << "\nSize: " << Size << "*" << Size << "\n";
                // double startW = MPI_Wtime();
                std::vector<double> C_sequintial = Multyplication(A, B);
                // double endW = MPI_Wtime() - startW;
                // std::cout << "Parallel: " << endL;
                // std::cout << "\nSequintial: " << endW << "\n";
                ASSERT_EQ(C_parallel, C_sequintial);
            }
        }
    }
}

TEST(Parallel_, Test_Equals_Parallel_And_Sequintial_Size_Number_x_100) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int Size = ProcNum * range3;
    int root = static_cast<int> (sqrt(ProcNum));
    double root2 = sqrt(ProcNum);
    if (fabs(root2 - root) <= epsilon) {
        if ((Size*Size) % ProcNum == 0) {
            std::vector<double> A = getRandomMatrix(Size*Size);
            std::vector<double> B = getRandomMatrix(Size*Size);
            std::vector<double> C_parallel(Size*Size);

            // double TotalTimeL = 0;
            // double startL = MPI_Wtime();
            C_parallel = getParallelMult(A, B, Size);
            // double endL = MPI_Wtime() - startL;

            if (ProcRank == 0) {
                // std::cout << "ProcNum: " << ProcNum;
                // std::cout << "\nSize: " << Size << "*" << Size << "\n";
                // double startW = MPI_Wtime();
                std::vector<double> C_sequintial = Multyplication(A, B);
                // double endW = MPI_Wtime() - startW;
                // std::cout << "Parallel: " << endL;
                // std::cout << "\nSequintial: " << endW << "\n";
                ASSERT_EQ(C_parallel, C_sequintial);
            }
        }
    }
}

TEST(Parallel_, Test_Different_Size_Sequintial) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int Size = ProcNum * range1;
    std::vector<double> A = getRandomMatrix(Size*Size);
    std::vector<double> B = getRandomMatrix(Size*Size + 1);
    if (ProcRank == 0) {
        ASSERT_ANY_THROW(Multyplication(A, B));
    }
}

TEST(Parallel, Test_Different_Size_Parallel) {
    int ProcRank, ProcNum;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int Size = ProcNum * range1;
    std::vector<double> A = getRandomMatrix(Size*Size);
    std::vector<double> B = getRandomMatrix(Size*Size + 1);
    if (ProcRank == 0) {
        ASSERT_ANY_THROW(getParallelMult(A, B, Size));
    }
}

TEST(Parallel, Test_Matrix_Size_Zero) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        ASSERT_ANY_THROW(getRandomMatrix(0));
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

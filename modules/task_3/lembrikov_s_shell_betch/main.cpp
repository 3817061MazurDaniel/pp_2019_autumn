// Copyright 2019 Lembrikov Stepan
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./shell_betch.h"

TEST(Mat_On_Vec_MPI, Test_On_Vector_EQ) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector <int> a(8);
    std::vector <int> res = {2, 3, 4, 5, 6, 7, 8, 9};
    std::vector <int> res_vector(8, 0);
    a[0] = 9;
    a[1] = 8;
    a[2] = 7;
    a[3] = 6;
    a[4] = 5;
    a[5] = 4;
    a[6] = 3;
    a[7] = 2;
    res_vector = Shell(a);
    if (rank == 0) {
        EXPECT_EQ(res_vector, res);
    }
}

TEST(Mat_On_Vec_MPI, Test_On_Vector_NE) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector <int> a(8);
    std::vector <int> res = {2, 3, 4, 5, 6, 7, 9, 8};
    std::vector <int> res_vector(8, 0);
    a[0] = 9;
    a[1] = 8;
    a[2] = 7;
    a[3] = 6;
    a[4] = 5;
    a[5] = 4;
    a[6] = 3;
    a[7] = 2;
    res_vector = Shell(a);
    if (rank == 0) {
        EXPECT_NE(res_vector, res);
    }
}

TEST(Mat_On_Vec_MPI, Test_On_Otr_Vector_EQ) {
    int razmer = 16;
    std::vector <int> a = getOtrVector(razmer);
    std::vector <int> res(razmer);
    std::vector <int> res_vector(razmer);
    res = a;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    res_vector = Shell(a);
    if (rank == 0) {
        res = ShellSort(res, razmer);
        EXPECT_EQ(res_vector, res);
    }
}

TEST(Mat_On_Vec_MPI, Test_On_Otr_Vector_NE) {
    int razmer = 16;
    std::vector <int> a = getOtrVector(razmer);
    std::vector <int> res(razmer);
    std::vector <int> res_vector(razmer);
    res = a;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    res_vector = Shell(a);
    if (rank == 0) {
        res = ShellSort(res, razmer);
        res[razmer - 1] = -1;
        EXPECT_NE(res_vector, res);
    }
}

TEST(Mat_On_Vec_MPI, Test_On_Random_Vector_EQ) {
    int razmer = 36;
    std::vector <int> a = getRandomVector(razmer);
    std::vector <int> res_vector_multy(razmer, 0);
    std::vector <int> res(razmer, 0);
    res = a;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // double start_multy = MPI_Wtime();
    res_vector_multy = Shell(a);
    // double end_multy = MPI_Wtime();

    if (rank == 0) {
        // double start_solo = MPI_Wtime();
        res = ShellSort(res, razmer);
        // double end_solo = MPI_Wtime();
        EXPECT_EQ(res_vector_multy, res);
        // std::cout << end_solo - start_solo << "\n";
        // std::cout << end_multy - start_multy << "\n";
    }
}

TEST(Mat_On_Vec_MPI, Test_On_Random_Vector_NE) {
    int razmer = 36;
    std::vector <int> a = getRandomVector(razmer);
    std::vector <int> res_vector_multy(razmer, 0);
    std::vector <int> res(razmer, 0);
    res = a;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    res_vector_multy = Shell(a);

    if (rank == 0) {
        res = ShellSort(res, razmer);
        res[razmer - 1] = -1;
        EXPECT_NE(res_vector_multy, res);
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

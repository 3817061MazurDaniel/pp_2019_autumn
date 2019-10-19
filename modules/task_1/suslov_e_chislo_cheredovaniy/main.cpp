// Copyright 2019 Suslov Egor
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_1/suslov_e_chislo_cheredovaniy/chislo_cheredovaniy.h"

TEST(Chislo_Cheredovaniy_Vector, Test_on_primere_chetnom) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec = { 2, -1, -2, -3, 5, 6, 7, 8 };
    int ChisloCheredovaniy = getParallelOperations(global_vec, global_vec.size());
    if (rank == 0) {
        ASSERT_EQ(ChisloCheredovaniy, 2);
    }
}

TEST(Chislo_Cheredovaniy_Vector, Test_on_primere_nechetnom) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec = { 2, -1, -2, -3, 5, 6, 7, 8, 9 };
    int ChisloCheredovaniy = getParallelOperations(global_vec, global_vec.size());
    if (rank == 0) {
        ASSERT_EQ(ChisloCheredovaniy, 2);
    }
}

TEST(Chislo_Cheredovaniy_Vector, Test_on_one_elem) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec = { 2 };
    int ChisloCheredovaniy = getParallelOperations(global_vec, global_vec.size());
    if (rank == 0) {
        ASSERT_EQ(ChisloCheredovaniy, 0);
    }
}

TEST(Chislo_Cheredovaniy_Vector, Test_on_two_elem_o) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec = { 2, 1 };
    int ChisloCheredovaniy = getParallelOperations(global_vec, global_vec.size());
    if (rank == 0) {
        ASSERT_EQ(ChisloCheredovaniy, 0);
    }
}

TEST(Chislo_Cheredovaniy_Vector, Test_on_two_elem_l) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec = { 2, -1 };
    int ChisloCheredovaniy = getParallelOperations(global_vec, global_vec.size());
    if (rank == 0) {
        ASSERT_EQ(ChisloCheredovaniy, 1);
    }
}

TEST(Chislo_Cheredovaniy_Vector, Test_on_rand_primere_nechetnom) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 101;
    int ChisloCheredovaniy;
    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }
    ChisloCheredovaniy = getParallelOperations(global_vec, count_size_vector);
    if (rank == 0) {
        int TrueChisloCheredovaniy = 0;
        TrueChisloCheredovaniy = getChisloCheredovaniy(global_vec, global_vec.size());
        ASSERT_EQ(ChisloCheredovaniy, TrueChisloCheredovaniy);
    }
}
TEST(Chislo_Cheredovaniy_Vector, Test_on_rand_primere_chetnom) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 102;
    int ChisloCheredovaniy;
    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }
    ChisloCheredovaniy = getParallelOperations(global_vec, count_size_vector);
    if (rank == 0) {
        int TrueChisloCheredovaniy = 0;
        TrueChisloCheredovaniy = getChisloCheredovaniy(global_vec, global_vec.size());
        ASSERT_EQ(ChisloCheredovaniy, TrueChisloCheredovaniy);
    }
}

TEST(Chislo_Cheredovaniy_Vector, Test_sravneniye_chisla_cheredovaniy) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec, local_vec;
    const int count_size_vector = 101;
    int ChisloCheredovaniy1, ChisloCheredovaniy2;
    if (rank == 0) {
        local_vec = getRandomVector(count_size_vector);
        global_vec = local_vec;
    }
    ChisloCheredovaniy2 = getParallelOperations(global_vec, count_size_vector);
    ChisloCheredovaniy1 = getParallelOperations(local_vec, count_size_vector);
    if (rank == 0) {
        ASSERT_EQ(ChisloCheredovaniy2, ChisloCheredovaniy1);
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


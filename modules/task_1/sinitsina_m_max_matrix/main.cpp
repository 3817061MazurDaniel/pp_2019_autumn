// Copyright 2019 Maria Sinitsina
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_1/sinitsina_m_max_matrix/max_matrix.h"

TEST(Matrix_max_MPI, test1_negative_in_rows) {
    std::vector <int> vect(4);
    vect = { 2, 1, 1, -100 };
    EXPECT_ANY_THROW(MaxMatrix(vect, -2, 2));
}

TEST(Matrix_max_MPI, test2_negative_in_cols) {
    std::vector <int> vect(4);
    vect = { 2, 1, 1, -100 };
    EXPECT_ANY_THROW(MaxMatrix(vect, 2, -2));
}

TEST(Matrix_max_MPI, test3_no_negative) {
    std::vector <int> vect(4);
    vect = { 2, 1, 1, -100 };
    EXPECT_NO_THROW(MaxMatrix(vect, 2, 2));
}

TEST(Matrix_max_MPI, test4_max_in_rand) {
    std::vector <int> vect = RandomMatrix(3, 3);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int max = MaxMatrix(vect, 3, 3);
    if (rank == 0) {
        EXPECT_NE(-1, max);
    }
}

TEST(Matrix_max_MPI, test5_max_in_rand) {
    std::vector <int> vect(9);
    vect = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int max = MaxMatrix(vect, 3, 3);
    if (rank == 0) {
        EXPECT_NE(-1, max);
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

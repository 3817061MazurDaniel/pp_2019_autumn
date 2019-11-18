// Copyright Gilenkov Alexander 2019

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_1/gilenkov_a_wrong_order/wrong_order_in_vector.h"

TEST(Wrong_Order_Vector, Test_on_primere_chetnom) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec = { 1, 2, 4, 3, 5, 7, 6, 8 };
    int WrongOrder = getParallelOperations(global_vec, global_vec.size());
    if (rank == 0) {
        ASSERT_EQ(WrongOrder, 2);
    }
}
TEST(Wrong_Order_Vector, Test_on_primere_nechetnom) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec = { 1, 2, 1, 2, 3, 6, 5, 8, 9 };
    int WrongOrder = getParallelOperations(global_vec, global_vec.size());
    if (rank == 0) {
        ASSERT_EQ(WrongOrder, 2);
    }
}
TEST(Wrong_Order_Vector, Test_on_one_elem) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec = { 2 };
    int WrongOrder = getParallelOperations(global_vec, global_vec.size());
    if (rank == 0) {
        ASSERT_EQ(WrongOrder, 0);
    }
}
TEST(Wrong_Order_Vector, Test_on_two_elem_o) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec = { 1, 2 };
    int WrongOrder = getParallelOperations(global_vec, global_vec.size());
    if (rank == 0) {
        ASSERT_EQ(WrongOrder, 0);
    }
}
TEST(Wrong_Order_Vector, Test_on_two_elem_l) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec = { 2, 1 };
    int WrongOrder = getParallelOperations(global_vec, global_vec.size());
    if (rank == 0) {
        ASSERT_EQ(WrongOrder, 1);
    }
}
TEST(Wrong_Order_Vector, Test_on_rand_primere_nechetnom) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 101;
    int WrongOrder;
    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
        for (int i = 0; i < count_size_vector; i++)
            std::cout << global_vec[i] << ", ";
    }
    WrongOrder = getParallelOperations(global_vec, count_size_vector);
    if (rank == 0) {
        int TrueWrongOrder = 0;
        TrueWrongOrder = getWrongOrder(global_vec, global_vec.size());
        ASSERT_EQ(WrongOrder, TrueWrongOrder);
    }
}
TEST(Wrong_Order_Vector, Test_on_rand_primere_chetnom) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 102;
    int WrongOrder;
    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
        for (int i = 0; i < count_size_vector; i++)
            std::cout << global_vec[i] << ", ";
    }
    WrongOrder = getParallelOperations(global_vec, count_size_vector);
    if (rank == 0) {
        int TrueWrongOrder = 0;
        TrueWrongOrder = getWrongOrder(global_vec, global_vec.size());
        ASSERT_EQ(WrongOrder, TrueWrongOrder);
    }
}
TEST(Wrong_Order_Vector, Test_sravneniye_vectors) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec, local_vec;
    const int count_size_vector = 101;
    int WrongOrder1, WrongOrder2;
    if (rank == 0) {
        local_vec = getRandomVector(count_size_vector);
        global_vec = local_vec;
    }
    WrongOrder2 = getParallelOperations(global_vec, count_size_vector);
    WrongOrder1 = getParallelOperations(local_vec, count_size_vector);
    if (rank == 0) {
        ASSERT_EQ(WrongOrder2, WrongOrder1);
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

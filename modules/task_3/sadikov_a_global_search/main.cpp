// Copyright 2019 Sadikov Artem
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <vector>
#include "./global_search.h"
#include "./functions.h"

TEST(Global_Search_MPI, Test_On_First_Func) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> actual(2);

    actual = get_res(1.0, 2.0, 2.0, 0.0001, func1);

    if (rank == 0) {
        std::vector<double> expected(2);
        expected = { 0.841, 1.000 };
        EXPECT_EQ(1, equal(actual, expected));
    }
}

TEST(Global_Search_MPI, Test_On_Second_Func) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> actual(2);

    actual = get_res(1.0, 50.0, 2.0, 0.00001, func2);

    if (rank == 0) {
        std::vector<double> expected(2);
        expected = { 0.530546, 1.00001 };
        EXPECT_EQ(1, equal(actual, expected));
    }
}

TEST(Global_Search_MPI, Test_On_Third_Func) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> actual(2);

    actual = get_res(1.0, 10.0, 2.0, 0.00001, func3);

    if (rank == 0) {
        std::vector<double> expected(2);
        expected = { 99.161, 1.00001 };
        EXPECT_EQ(1, equal(actual, expected));
    }
}

TEST(Global_Search_MPI, Cant_Get_On_Wrong_Length) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> actual(2);

    if (rank == 0) {
        ASSERT_ANY_THROW(actual = get_res(5.0, 2.0, 1.0, 0.00001, func1));
    }
}

TEST(Global_Search_MPI, Cant_Get_On_Equal_Ends) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> actual(2);

    if (rank == 0) {
        ASSERT_ANY_THROW(actual = get_res(5.0, 5.0, 2.0, 0.0001, func3));
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

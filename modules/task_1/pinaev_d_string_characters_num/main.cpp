// Copyright 2019 Obolenskiy Arseniy
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include "./string_characters_num.h"

TEST(Str_Char_Num_MPI, Can_Get_Not_Paral_Char_Num) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char str[] = "aBcD0 12";
    ASSERT_EQ(getCarNum(str, 9), 4);
}

TEST(Str_Char_Num_MPI, Not_Paral_Char_Num_Can_Process_Not_All_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char str[] = "aBcD0 12";
    ASSERT_EQ(getCarNum(str, 5), 4);
}

TEST(Str_Char_Num_MPI, Paral_Char_Num_Eq_Not_Paral_Char_Num) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char *str;
    size_t strSize = 10000;
    str = getRandomString(strSize);
    int paralAnswer = getParalCarNum(str, strSize);

    if (rank == 0) {
        int seqAnswer = getCarNum(str, strSize);
        ASSERT_EQ(seqAnswer, paralAnswer);
    }
}

TEST(Str_Char_Num_MPI, Paral_Char_Num_Can_Process_Not_All_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char *str;
    size_t strSize = 10000;
    str = getRandomString(strSize);
    int paralAnswer = getParalCarNum(str, strSize / 2);

    if (rank == 0) {
        int seqAnswer = getCarNum(str, strSize / 2);
        ASSERT_EQ(seqAnswer, paralAnswer);
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

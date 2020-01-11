// Copyright 2019 Tonkov Alexei
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "../../../modules/task_2/tonkov_a_producer_consumer/producer_consumer.h"

TEST(Producer_Consumer_MPI, test_func_rootsFromNumbers) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int numbers[4] = {4, 16, 9, 36};
    double roots[4];
    double real_roots[4] = {2, 4, 3, 6};
    rootsFromNumbers(numbers, roots, 4);
    bool flag = true;

    for (int i = 0; i < 4; i++) {
        if (roots[i] != real_roots[i]) {
            flag = false;
            break;
        }
    }

    if (rank == 0) {
        EXPECT_TRUE(flag);
    }
}

TEST(Producer_Consumer_MPI, test_func_gen_numbers) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int numbers[4] = {0, 1, 2, 3};
    int* test_numbers = generateNumbers(4);
    bool flag = true;

    for (int i = 0; i < 4; i++) {
        if (numbers[i] != test_numbers[i]) {
           flag = false;
           break;
        }
    }
    if (rank == 0) {
           EXPECT_TRUE(flag);
    }
}

TEST(Producer_Consumer_MPI, test_main_func_true) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int numbers[4] = { 4, 9, 16, 36 };
    double roots[4];

    double real_roots[4];
    rootsFromNumbers(numbers, real_roots, 4);

    produceConsume(numbers, roots, 4);

    bool flag = true;

    for (int i = 0; i < 4; i++) {
        if (roots[i] != real_roots[i]) {
            flag = false;
            break;
        }
    }
    if (rank == 0) {
        EXPECT_TRUE(flag);
    }
}

TEST(Producer_Consumer_MPI, test_main_func_false) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int numbers[4] = { 16, 16, 9, 36 };
    double roots[4];
    double real_roots[4] = { 2, 4, 3, 6 };

    produceConsume(numbers, roots, 4);

    bool flag = true;

    for (int i = 0; i < 4; i++) {
        if (roots[i] != real_roots[i]) {
            flag = false;
            break;
        }
    }
    if (rank == 0) {
        EXPECT_FALSE(flag);
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

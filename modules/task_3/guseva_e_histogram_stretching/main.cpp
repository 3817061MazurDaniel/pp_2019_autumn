// Copyright 2019 Guseva Catherine

#include <gtest/gtest.h>
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <vector>
#include "../../task_3/guseva_e_histogram_stretching/Histogram_stretching.h"

TEST(Histogram_stretching, Test_Dont_take_Image) {
    int cols = -1;
    int rows = 0;
    std::vector<int> temp;

    ASSERT_ANY_THROW(temp = getRandomImage(cols, rows));
}

TEST(Histogram_stretching, Test_Can_Parall_Metod) {
    std::vector<int> example = { 1, 3, 1, 2, 1, 3 };
    std::vector<int> example2 = { 1, 3, 1, 2, 1, 3 };
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    example = HistogramSequential(example, 2, 3);
    example2 = HistogramParallel(example2, 2, 3);

    if (rank == 0) {
        ASSERT_EQ(example, example2);
    }
}

TEST(Histogram_stretching, Test_Right_Max_Value) {
    std::vector<int> example = { 1, 3, 5, 2, 1, 3 };

    ASSERT_EQ(5, getMax(example));
}

TEST(Histogram_stretching, Test_Right_Min_Value) {
    std::vector<int> example = { 1, 3, 5, 2, 1, 3 };

    ASSERT_EQ(1, getMin(example));
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

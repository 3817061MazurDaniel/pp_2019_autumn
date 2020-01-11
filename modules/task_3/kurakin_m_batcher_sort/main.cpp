// Copyright 2019 Kurakin Mikhail
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <algorithm>
#include <vector>
#include "./batcher_sort.h"

TEST(Batcher_Sort_MPI, Throw_When_Input_Arr_Doesnt_Have_Vals) {
    std::vector<int> arr;
    ASSERT_ANY_THROW(BatcherSort(&arr));
}

TEST(Batcher_Sort_MPI, Size_InputArr_Equals_Size_OutputArr) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> arr{1, 8, 5, 3, 2, 9, 0, 4, 7, 6};
    uint32_t sizeIn = arr.size();
    BatcherSort(&arr);
    if (rank == 0) {
        EXPECT_EQ(sizeIn, arr.size());
    }
}

TEST(Batcher_Sort_MPI, Array_sorted_properly) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> arrP{1, 8, 5, 3, 2, 9, 0, 4, 7, 6};
    std::vector<int> arrS{1, 8, 5, 3, 2, 9, 0, 4, 7, 6};
    BatcherSort(&arrP);
    if (rank == 0) {
        std::sort(arrS.begin(), arrS.end());
        bool AreEq = true;
        for (uint32_t i = 0; i < arrS.size(); i++) {
            if (arrS[i] != arrP[i]) {
                AreEq = false;
                break;
            }
        }
        EXPECT_EQ(true, AreEq);
    }
}

TEST(Batcher_Sort_MPI, Array_sorted_Properly_With_Same_Numbers) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> arrP{1, 1, 1, 2, 2, 2, 0, 0, 0, 9};
    std::vector<int> arrS{1, 1, 1, 2, 2, 2, 0, 0, 0, 9};
    BatcherSort(&arrP);

    if (rank == 0) {
        std::sort(arrS.begin(), arrS.end());
        bool AreEq = true;
        for (uint32_t i = 0; i < arrS.size(); i++) {
            if (arrS[i] != arrP[i]) {
                AreEq = false;
                break;
            }
        }
        EXPECT_EQ(true, AreEq);
    }
}

TEST(Batcher_Sort_MPI, Array_sorted_properly_Rand) {
    int rank, size = 1000;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> arrS(size);

    if (rank == 0) {
        CreateArray(&arrS);
    }
    std::vector<int> arrP(arrS);
    BatcherSort(&arrP);
    if (rank == 0) {
        std::sort(arrS.begin(), arrS.end());
        bool AreEq = true;
        for (uint32_t i = 0; i < arrS.size(); i++) {
            if (arrS[i] != arrP[i]) {
                AreEq = false;
                break;
            }
        }
        EXPECT_EQ(true, AreEq);
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

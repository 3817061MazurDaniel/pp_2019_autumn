/*
 * Copyright (C) 2019 LOOGIN. All Rights Reserved.
 */

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "closerElem.h"
#include <vector>

TEST(closer_elem, return_empty_vector_when_empty_vector) {
    ASSERT_EQ(ISearch(std::vector<int>()), std::vector<int>());
}

TEST(closer_elem, return_empty_vector_when_1_element) {
    ASSERT_EQ(ISearch(std::vector<int>(1)), std::vector<int>());
}

TEST(closer_elem, check_correct_range) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    static const std::vector<int> randomVector = {47, 63, 60, 70, 87, 99, 8, 88, 0, 18, 37, 41, 72, 71, 66, 24, 77,
        67, 43, 85, 53, 13, 22, 95, 73, 59, 34, 35, 96, 19, 89, 68, 11, 52, 80, 97, 20, 55, 90, 79, 82, 7,
        93, 1, 6, 54, 56, 57, 91, 98, 46, 44, 39, 61, 83, 92, 49, 74, 62, 30, 33, 3, 36, 84, 78, 9, 29, 15};
    int correctRange = 1;
    auto minRange = ISearch(randomVector);

    if (procRank == 0) {
        ASSERT_EQ(minRange[0], correctRange);
    }
}

TEST(closer_elem, check_correct_numbers) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    static const std::vector<int> randomVector = {129, 5, 150, 140, -94, 177, -15, 64, -31, -89, -57,
        158, -124, 57, 90, 125, 166, 71, -41, -59, -67, -182, 146, -7, 5, 86, 44, -17, -48, 63, 172, 154,
        161, 180, -76, 26, -109, 46, 29, -991, -96, 138, 188, 108, -108, 21, -69, -197, -128, -33, 12,
        158, -99, -33, -9, -182, 15, 130, 66, 76, -69, -173, 70, -155, -96, -135, 91, 200, -191, -182,
        102, -21, 97, 39, -3, 54, 6, -54, 197, 124, 159, -28, 53, -58, -12, 9, 34, -116, -106, 24, -92,
        18, 0, -117, -72, -181, -9, -95, -16, 37};
    auto minRange = ISearch(randomVector);
    int correctLeftNumber = 154;
    int correctRightNumber = 161;

    if (procRank == 0) {
        ASSERT_EQ(minRange[1], correctLeftNumber);
        ASSERT_EQ(minRange[2], correctRightNumber);
    }
}

TEST(closer_elem, check_correct_position) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    static const std::vector<int> randomVector = {83417, 88210, 3969, -18147, 54619, 41968, 81648, -18266,
        67586, 18114, 55686, 76033, 61796, -16879, 74844, 92194, 77922, 17200, 225, 78523, 60284, -2640, 88210,
        77955, 52813, 6680, 57959, 8461, 37163, 70788, 52970, -6295, 76302, 86014, 32432, -16358, 60236, -3056,
        70006, 34794, -709, -16600, -18252, 75788, 58023, 96462, 52044, 98511, 55581, -15262, 68892, 59027,
        74541, 88994, 31534, 64822, 81232, 11451, 56471, 48089, 45526, 20656, 32792, -7029, 56298, 88167,
        56422, -12388, 14982, 77470, 41325, 47454, -12021, -14916, 23863, 96307, 27678, 3251, 51077, 43496,
        28915, 72752, 80915, 17991, 16309, -13104, 71431, 41717, 71537, -2902, 53496, 10850, 16652, 11722,
        19860, 7814, 51445, 6110, -14108, 50311};
    auto minRange = ISearch(randomVector);
    int correctPosition = 41;

    if (procRank == 0) {
        ASSERT_EQ(minRange[3], correctPosition);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}

// Copyright 2019 Makarova Viktoria
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <random>
#include <algorithm>
#include <cassert>
#include <vector>
#include "../../../modules/task_3/makarova_v_raz_sort/razr_sort.h"

TEST(Razr_Sort, pow10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
      ASSERT_EQ(pow10(0), 1);
      ASSERT_EQ(pow10(1), 10);
      ASSERT_EQ(pow10(2), 100);
      ASSERT_EQ(pow10(3), 1000);
    }
}

TEST(Razr_Sort, getRaz) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        int val = 4321;

        ASSERT_EQ(getRaz(val, 0), 1);
        ASSERT_EQ(getRaz(val, 1), 2);
        ASSERT_EQ(getRaz(val, 2), 3);
        ASSERT_EQ(getRaz(val, 3), 4);
    }
}

TEST(Razr_Sort, sort_iter) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    if (rank == 0) {
        int razr = 1;
        std::vector<int> in = {165, 556, 10, 17, 964, 6, 1};

        std::vector<int> out = {6, 1, 10, 17, 556, 165, 964};

        in = RazrIteration(in, razr);

        for (size_t i = 0; i < in.size(); ++i) {
            EXPECT_EQ(in[i], out[i]);
        }
    }
}

TEST(Razr_Sort, Razr_Sort_Static) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    if (rank == 0) {
        std::vector<int> in = {165, 556, 10, 17, 964, 6, 1};

        std::vector<int> out = {1, 6, 10, 17, 165, 556, 964};

        in = RazrSort(in);

        for (size_t i = 0; i < in.size(); ++i) {
            EXPECT_EQ(in[i], out[i]);
        }
    }
}

TEST(Razr_Sort, Razr_Sort_Random) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    if (rank == 0) {
        std::vector<int> in = GetRandVector(1000);

        in = RazrSort(in);

        ASSERT_TRUE(isSorted(in));
    }
}

TEST(Razr_Sort, Merge) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::vector<int> in1 = GetRandVector(1000);
        std::vector<int> in2 = GetRandVector(1000);

        in1 = RazrSort(in1);
        in2 = RazrSort(in2);

        ASSERT_TRUE(isSorted(in1));
        ASSERT_TRUE(isSorted(in2));

        std::vector<int> out = Merge(in1, in2);

        ASSERT_TRUE(isSorted(out));
    }
}

TEST(Razr_Sort, ParalSort) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> in;
    std::vector<int> out;

    if (rank == 0) {
        std::vector<int> in = GetRandVector(1000);
    }

    out = ParallSort(in);

    if (rank == 0) {
        ASSERT_TRUE(isSorted(out));
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

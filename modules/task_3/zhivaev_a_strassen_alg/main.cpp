// Copyright 2019 Zhivaev Artem
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <array>
#include <numeric>
#include "../../../modules/task_3/zhivaev_a_strassen_alg/strassen_alg.h"

using std::vector;
using std::array;
using std::accumulate;

TEST(Helper_Functions, Split_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        array<double, 16> m{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}};
        array<double, 4> e11{{1, 2, 5, 6}};
        array<double, 4> e12{{3, 4, 7, 8}};
        array<double, 4> e21{{9, 10, 13, 14}};
        array<double, 4> e22{{11, 12, 15, 16}};

        array<double, 4> m11;
        array<double, 4> m12;
        array<double, 4> m21;
        array<double, 4> m22;

        splitMatrix(m.data(), m11.data(), m12.data(), m21.data(), m22.data(), 4);

        ASSERT_EQ(e11, m11);
        ASSERT_EQ(e12, m12);
        ASSERT_EQ(e21, m21);
        ASSERT_EQ(e22, m22);
    }
}

TEST(Helper_Functions, Form_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        array<double, 16> e{ {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16} };
        array<double, 4> m11{ {1, 2, 5, 6} };
        array<double, 4> m12{ {3, 4, 7, 8} };
        array<double, 4> m21{ {9, 10, 13, 14} };
        array<double, 4> m22{ {11, 12, 15, 16} };

        array<double, 16> m;

        formMatrix(m.data(), m11.data(), m12.data(), m21.data(), m22.data(), 4);

        ASSERT_EQ(e, m);
    }
}

TEST(Helper_Functions, Add_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        array<double, 16> mA{ {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16} };
        array<double, 16> mB{ {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1} };
        array<double, 16> mC;
        array<double, 16> expected{{17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17}};

        addMatrix(mA.data(), mB.data(), mC.data(), 4);

        ASSERT_EQ(expected, mC);
    }
}

TEST(Helper_Functions, Sub_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        array<double, 16> mA{ {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16} };
        array<double, 16> mB{ {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1} };
        array<double, 16> mC;
        array<double, 16> expected{ {-15, -13, -11, -9, -7, -5, -3, -1, 1, 3, 5, 7, 9, 11, 13, 15} };

        subMatrix(mA.data(), mB.data(), mC.data(), 4);

        ASSERT_EQ(expected, mC);
    }
}

TEST(Multiply, Strassen_Sequential) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    constexpr int side = 128;

    if (rank == 0) {
        vector<double> mA(side* side);
        vector<double> mB(side* side);
        vector<double> mC(side* side);
        vector<double> expected(side* side);

        for (int i = 0, j = side * side; i < side * side; i++, j--) {
            mA[i] = i + 1;
            mB[i] = j;
        }
        multSeq(mA.data(), mB.data(), expected.data(), side);
        multStrassenOneProc(mA.data(), mB.data(), mC.data(), side);
        ASSERT_EQ(expected, mC);
    }
}

TEST(Multiply, Strassen_Parallel) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int side = 4;
    vector<double> mA;
    vector<double> mB;
    vector<double> mC;
    vector<double> expected;

    if (rank == 0) {
        mA.resize(side * side);
        mB.resize(side * side);
        mC.resize(side * side);
        expected.resize(side * side);
        for (size_t i = 0, j = mA.size(); i < mA.size(); i++, j--) {
            mA[i] = i + 1;
            mB[i] = j;
        }
        multSeq(mA.data(), mB.data(), expected.data(), side);
    }

    multStrassen(mA.data(), mB.data(), mC.data(), side);

    if (rank == 0) {
        ASSERT_EQ(expected, mC);
    }
}

TEST(DISABLED_Efficiency, Strassen_Parallel_Over_Multiply_Sequentially) {
    std::cout << std::flush;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int side = 512;
    constexpr int kRepeat = 2;
    vector<double> mA;
    vector<double> mB;
    vector<double> mC;
    vector<double> expected;
    double seqTime[kRepeat];
    double parTime[kRepeat];

    if (rank == 0) {
        mA.resize(side * side);
        mB.resize(side * side);
        mC.resize(side * side);
        expected.resize(side * side);
        for (size_t i = 0, j = mA.size(); i < mA.size(); i++, j--) {
            mA[i] = i + 1;
            mB[i] = j;
        }
        multSeq(mA.data(), mB.data(), expected.data(), side);
    }

    for (int i = 0; i < kRepeat; i++) {
        seqTime[i] = MPI_Wtime();
        if (rank == 0) multSeq(mA.data(), mB.data(), mC.data(), side);
        seqTime[i] = MPI_Wtime() - seqTime[i];

        MPI_Barrier(MPI_COMM_WORLD);
        parTime[i] = MPI_Wtime();
        multStrassen(mA.data(), mB.data(), mC.data(), side);
        parTime[i] = MPI_Wtime() - parTime[i];
        if (rank == 0)
            std::cout << "Iteration: " << i << std::endl;
    }

    if (rank == 0) {
        double seqAvgTime = accumulate(seqTime, seqTime + kRepeat, 0.0) / kRepeat;
        double parAvgTime = accumulate(parTime, parTime + kRepeat, 0.0) / kRepeat;
        std::cout << "seq avg: " << seqAvgTime << std::endl;
        std::cout << "par avg: " << parAvgTime << std::endl;
        std::cout << "effic over seq: " << seqAvgTime / parAvgTime << std::endl;

        ASSERT_LT(parAvgTime / seqAvgTime, 1);
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

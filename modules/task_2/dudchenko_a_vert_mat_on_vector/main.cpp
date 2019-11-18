// Copyright 2019 Dudchenko Anton

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_2/dudchenko_a_vert_mat_on_vector/vert_mat_on_vector.h"

TEST(Test_Vert_Mat_On_Vector, Create_Rand_Vec_And_Matrix) {
    int rows = 10;
    unsigned int columns = 10;
    std::vector<int> vec = getRandVector(columns);
    std::vector<int> mat = getRandMatrix(rows, columns);
    EXPECT_EQ(columns, vec.size());
    EXPECT_EQ(rows * columns, mat.size());
}


TEST(Test_Vert_Mat_On_Vector, Throw_With_Negative_Size) {
    int rows = 3;
    int columns = 3;

    std::vector<int> vec = getRandVector(columns);
    std::vector<int> mat = getRandMatrix(rows, columns);

    ASSERT_ANY_THROW(multiMatOnVec(mat, -1, columns, vec));
}

TEST(Test_Vert_Mat_On_Vector, Multi_Sequence) {
    int rows = 3;
    int columns = 3;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec = { 1, 2, 3 };
    std::vector<int> mat = { 4, 5, 6, 7, 8, 9, 10, 11, 12 };

    std::vector<int> expectVec = { 32, 50, 68 };

    if (rank == 0) {
        std::vector<int> vectorSeq = multiMatOnVecSeq(mat, rows, columns, vec);
        EXPECT_EQ(expectVec, vectorSeq);
    }
}

TEST(Test_Vert_Mat_On_Vector, Can_Multiply) {
    int rows = 3;
    int columns = 3;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec = getRandVector(columns);
    std::vector<int> mat = getRandMatrix(rows, columns);

    std::vector<int> resultVec = multiMatOnVec(mat, rows, columns, vec);

    if (rank == 0) {
        std::vector<int> vectorSeq = multiMatOnVecSeq(mat, rows, columns, vec);
        EXPECT_EQ(resultVec, vectorSeq);
    }
}

TEST(Test_Vert_Mat_On_Vector, Multiply_Big_Size) {
    int rows = 1000;
    int columns = 1000;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec = getRandVector(columns);
    std::vector<int> mat = getRandMatrix(rows, columns);

    std::vector<int> resultVec = multiMatOnVec(mat, rows, columns, vec);

    if (rank == 0) {
        std::vector<int> vectorSeq = multiMatOnVecSeq(mat, rows, columns, vec);
        EXPECT_EQ(resultVec, vectorSeq);
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

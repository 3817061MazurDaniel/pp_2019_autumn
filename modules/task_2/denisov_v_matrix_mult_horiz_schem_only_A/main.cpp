// Copyright 2019 Denisov Vladislav
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_mult_horiz_schem_only_A.h"

TEST(matrix_mult_horiz_schem_only_A, throw_when_create_random_matrix_with_negative_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        ASSERT_ANY_THROW(createRandomMatrix(-1));
    }
}

TEST(matrix_mult_horiz_schem_only_A, throw_when_seq_mult_matrix_with_negative_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrixA(1);
    std::vector<int> matrixB(1);

    if (rank == 0) {
        ASSERT_ANY_THROW(getMatrixMultSeq(matrixA, matrixB, -1));
    }
}

TEST(matrix_mult_horiz_schem_only_A, throw_when_par_mult_matrix_with_negative_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrixA(1);
    std::vector<int> matrixB(1);

    if (rank == 0) {
        ASSERT_ANY_THROW(getMatrixMultPar(matrixA, matrixB, -1));
    }
}

TEST(matrix_mult_horiz_schem_only_A, throw_when_seq_mult_matrix_with_incorrect_size_parameter) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrixA(1);
    std::vector<int> matrixB(2);

    if (rank == 0) {
        ASSERT_ANY_THROW(getMatrixMultSeq(matrixA, matrixB, 1));
    }
}

TEST(matrix_mult_horiz_schem_only_A, throw_when_par_mult_matrix_with_incorrect_size_parameter) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrixA(1);
    std::vector<int> matrixB(2);

    if (rank == 0) {
        ASSERT_ANY_THROW(getMatrixMultPar(matrixA, matrixB, 1));
    }
}

TEST(matrix_mult_horiz_schem_only_A, check_correct_seq_mult_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sizeSide = 2;
    std::vector<int> matrixA(sizeSide * sizeSide);
    std::vector<int> matrixB(sizeSide * sizeSide);
    std::vector<int> correctAnswer(sizeSide * sizeSide);

    matrixA[0] = 1;
    matrixA[1] = 2;
    matrixA[2] = 3;
    matrixA[3] = 4;

    matrixB[0] = 5;
    matrixB[1] = 6;
    matrixB[2] = 7;
    matrixB[3] = 8;

    correctAnswer[0] = 19;
    correctAnswer[1] = 22;
    correctAnswer[2] = 43;
    correctAnswer[3] = 50;

    if (rank == 0) {
        std::vector<int> resultSeq = getMatrixMultSeq(matrixA, matrixB, sizeSide);
        ASSERT_EQ(correctAnswer, resultSeq);
    }
}

TEST(matrix_mult_horiz_schem_only_A, check_correct_par_mult_matrix_1size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sizeSide = 1;
    std::vector<int> matrixA(sizeSide * sizeSide);
    std::vector<int> matrixB(sizeSide * sizeSide);
    std::vector<int> correctAnswer(sizeSide * sizeSide);

    matrixA[0] = 2;
    matrixB[0] = 3;
    correctAnswer[0] = 6;

    std::vector<int> resultPar = getMatrixMultPar(matrixA, matrixB, sizeSide);

    if (rank == 0) {
        ASSERT_EQ(correctAnswer, resultPar);
    }
}

TEST(matrix_mult_horiz_schem_only_A, check_correct_par_mult_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sizeSide = 2;
    std::vector<int> matrixA(sizeSide * sizeSide);
    std::vector<int> matrixB(sizeSide * sizeSide);
    std::vector<int> correctAnswer(sizeSide * sizeSide);

    matrixA[0] = 1;
    matrixA[1] = 2;
    matrixA[2] = 3;
    matrixA[3] = 4;

    matrixB[0] = 5;
    matrixB[1] = 6;
    matrixB[2] = 7;
    matrixB[3] = 8;

    correctAnswer[0] = 19;
    correctAnswer[1] = 22;
    correctAnswer[2] = 43;
    correctAnswer[3] = 50;

    std::vector<int> resultPar = getMatrixMultPar(matrixA, matrixB, sizeSide);

    if (rank == 0) {
        ASSERT_EQ(correctAnswer, resultPar);
    }
}

// TEST(matrix_mult_horiz_schem_only_A, check_correct_par_mult_matrix_for_time) {
//    int rank;
//    MPI_Barrier(MPI_COMM_WORLD);
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//
//    int sizeSide = 20;
//    std::vector<int> matrixA = createRandomMatrix(sizeSide);
//    std::vector<int> matrixB = createRandomMatrix(sizeSide);
//
//    // double startPar = MPI_Wtime();
//    std::vector<int> resultPar = getMatrixMultPar(matrixA, matrixB, sizeSide);
//    // double endPar = MPI_Wtime();
//
//    if (rank == 0) {
//        // double startSeq = MPI_Wtime();
//        std::vector<int> resultSeq = getMatrixMultSeq(matrixA, matrixB, sizeSide);
//        // double endSeq = MPI_Wtime();
//        ASSERT_EQ(resultSeq, resultPar);
//        // std::cout << "Time seq: " << endSeq - startSeq << std::endl;
//        // std::cout << "Time par: " << endPar - startPar << std::endl;
//    }
// }


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

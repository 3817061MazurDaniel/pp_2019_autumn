// Copyright 2019 Denisov Vladislav
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./gradient_method.h"

TEST(gradient_method, throw_when_create_random_matrix_with_negative_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        ASSERT_ANY_THROW(createRandomMatrix(-1));
    }
}

TEST(gradient_method, throw_when_create_random_vector_with_negative_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        ASSERT_ANY_THROW(createRandomVector(-1));
    }
}

TEST(gradient_method, throw_when_vector_mult_with_not_consistent_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> vector1(2), vector2(3);

    if (rank == 0) {
        ASSERT_ANY_THROW(vectorMult(vector1, vector2));
    }
}

TEST(gradient_method, throw_when_matrix_vector_mult_with_not_consistent_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size = 2;
    std::vector<double> matrix(size * size);
    std::vector<double> vector(size + 1);

    if (rank == 0) {
        ASSERT_ANY_THROW(matrixVectorMult(matrix, vector));
    }
}

TEST(gradient_method, check_correct_vector_mult) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size = 2;
    std::vector<double> vector1(size);
    std::vector<double> vector2(size);
    double correctAnswer;

    vector1[0] = 1;
    vector1[1] = 2;

    vector2[0] = 5;
    vector2[1] = 6;

    correctAnswer = 17;

    if (rank == 0) {
        double result = vectorMult(vector1, vector2);
        ASSERT_NEAR(correctAnswer, result, 0.5);
    }
}

TEST(gradient_method, check_correct_matrix_vector_mult) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size = 2;
    std::vector<double> vector(size);
    std::vector<double> matrix(size * size);
    std::vector<double> correctAnswer(size);

    vector[0] = 1;
    vector[1] = 2;

    matrix[0] = 4;
    matrix[1] = 5;
    matrix[2] = 6;
    matrix[3] = 7;

    correctAnswer[0] = 14;
    correctAnswer[1] = 20;

    if (rank == 0) {
        std::vector<double> result = matrixVectorMult(matrix, vector);
        for (size_t i = 0; i < result.size(); i++)
            ASSERT_NEAR(correctAnswer[i], result[i], 0.5);
    }
}

TEST(gradient_method, check_correct_seq_gradient_method) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size = 3;
    std::vector<double> vector(size);
    std::vector<double> matrix(size * size);
    std::vector<double> correctAnswer(size);

    vector[0] = 1;
    vector[1] = 5;
    vector[2] = 9;

    matrix[0] = 3;
    matrix[1] = 4;
    matrix[2] = 0;
    matrix[3] = 4;
    matrix[4] = -3;
    matrix[5] = 0;
    matrix[6] = 0;
    matrix[7] = 0;
    matrix[8] = 5;

    correctAnswer[0] = 0.92;
    correctAnswer[1] = -0.44;
    correctAnswer[2] = 1.80;

    if (rank == 0) {
        std::vector<double> result = getSolveSeq(matrix, vector, size);
        for (size_t i = 0; i < result.size(); i++)
            ASSERT_NEAR(correctAnswer[i], result[i], 0.5);
    }
}

TEST(gradient_method, check_correct_par_gradient_method) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size = 12;
    std::vector<double> vector = createRandomVector(size);
    std::vector<double> matrix = createRandomMatrix(size);

    std::vector<double> resultPar = getSolvePar(matrix, vector, size);

    if (rank == 0) {
        std::vector<double> resultSeq = getSolveSeq(matrix, vector, size);
        for (size_t i = 0; i < resultSeq.size(); i++)
            ASSERT_NEAR(resultSeq[i], resultPar[i], 0.5);
    }
}

// TEST(gradient_method, check_correct_par_gradient_method_for_time) {
//    int rank;
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//
//    int size = 20;
//    std::vector<double> vector = createRandomVector(size);
//    std::vector<double> matrix = createRandomMatrix(size);
//
//    double startPar = MPI_Wtime();
//    std::vector<double> resultPar = getSolvePar(matrix, vector, size);
//    double endPar = MPI_Wtime();
//
//    if (rank == 0) {
//        double startSeq = MPI_Wtime();
//        std::vector<double> resultSeq = getSolveSeq(matrix, vector, size);
//        double endSeq = MPI_Wtime();
//        for (size_t i = 0; i < resultSeq.size(); i++)
//            ASSERT_NEAR(resultSeq[i], resultPar[i], 0.5);
//        std::cout << "Time seq: " << endSeq - startSeq << std::endl;
//        std::cout << "Time par: " << endPar - startPar << std::endl;
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

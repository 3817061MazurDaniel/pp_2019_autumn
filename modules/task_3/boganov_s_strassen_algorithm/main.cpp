  // Copyright 2019 Boganov Sergey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./strassen_algorithm.h"

double eps = 1e-6;

TEST(getSequentialSquareMatrixAddition, Test_square_matrix_addition) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> A = {3, 2, -5, 2, -1, 3, 1, 2, -1};
    std::vector<double> B = {2, 8, -5, 4, 2, 8, 1, -7, 6};

    //     { 3  2 -5 }       { 2  8 -5 }         { 5   10  -10 }
    // A = { 2 -1  3 } , B = { 4  2  8 } --> C = { 6    1   11 }
    //     { 1  2 -1 }       { 1 -7  6 }         { 2   -5    5 }

    std::vector<double> C, ans = {5, 10, -10, 6, 1, 11, 2, -5, 5};
    int n = 3;

    if (rank == 0) {
        ASSERT_NO_THROW({C = getSequentialSquareMatrixAddition(n, A, B);});
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                EXPECT_NEAR(C[j + i * n], ans[j + i * n], eps);
            }
        }
    }
}

TEST(getSequentialSquareMatrixSubtraction, Test_square_matrix_subtraction) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> A = {3, 2, -5, 2, -1, 3, 1, 2, -1};
    std::vector<double> B = {2, 8, -5, 4, 2, 8, 1, -7, 6};

    //     { 3  2 -5 }       { 2  8 -5 }         {  1  -6   0 }
    // A = { 2 -1  3 } , B = { 4  2  8 } --> C = { -2  -3  -5 }
    //     { 1  2 -1 }       { 1 -7  6 }         {  0   9  -7 }

    std::vector<double> C, ans = {1, -6, 0, -2, -3, -5, 0, 9, -7};
    int n = 3;

    if (rank == 0) {
        ASSERT_NO_THROW({C = getSequentialSquareMatrixSubtraction(n, A, B);});
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                EXPECT_NEAR(C[j + i * n], ans[j + i * n], eps);
            }
        }
    }
}

TEST(getSequentialSquareMatrixMultiplication, Test_square_matrix_multiplication) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> A = {3, 2, -5, 2, -1, 3, 1, 2, -1};
    std::vector<double> B = {2, 8, -5, 4, 2, 8, 1, -7, 6};

    //     { 3  2 -5 }       { 2  8 -5 }         { 9   63  -29 }
    // A = { 2 -1  3 } , B = { 4  2  8 } --> C = { 3   -7    0 }
    //     { 1  2 -1 }       { 1 -7  6 }         { 9   19    5 }

    std::vector<double> C, ans = {9, 63, -29, 3, -7, 0, 9, 19, 5};
    int n = 3;

    if (rank == 0) {
        ASSERT_NO_THROW({C = getSequentialSquareMatrixMultiplication(n, A, B);});
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                EXPECT_NEAR(C[j + i * n], ans[j + i * n], eps);
            }
        }
    }
}

TEST(getNewSize, Test_new_size_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 5, ans = 8;

    if (rank == 0) {
        EXPECT_EQ(getNewSize(n), ans);
    }
}

TEST(getNewSize, Test_new_size_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 16, ans = 16;

    if (rank == 0) {
        EXPECT_EQ(getNewSize(n), ans);
    }
}

TEST(getExpandSquareMatrix, Test_get_expand_square_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> A = {3, 2, -5, 2, -1, 3, 1, 2, -1};

    //     { 3  2 -5 }
    // A = { 2 -1  3 }
    //     { 1  2 -1 }

    std::vector<double> B, ans = {3, 2, -5, 0, 2, -1, 3, 0, 1, 2, -1, 0, 0, 0, 0, 0};
    int n = 3, new_size;

    //       { 3  2 -5  0}
    // ans = { 2 -1  3  0}
    //       { 1  2 -1  0}
    //       { 0  0  0  0}

    if (rank == 0) {
        new_size = getNewSize(n);
        ASSERT_NO_THROW({B = getExpandSquareMatrix(n, A);});
        int size = static_cast<int>(B.size());
        EXPECT_EQ(size, new_size * new_size);
        for (int i = 0; i < new_size; i++) {
            for (int j = 0; j < new_size; j++) {
                EXPECT_EQ(ans[j + i * new_size], B[j + i * new_size]);
            }
        }
    }
}

TEST(getFourMatrixBlocks, Test_get_four_matrix_blocks) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 4, new_size = 2;
    std::vector<double> A = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7};
    std::vector<double> A11 = {1, 2, 5, 6}, A12 = {3, 4, 7, 8};
    std::vector<double> A21 = {9, 1, 4, 5}, A22 = {2, 3, 6, 7};

    //     { 1 2 3 4 }
    // A = { 5 6 7 8 } , A11 = { 1 2 } , A12 = { 3 4 } , A21 = { 9 1 } , A22 = { 2 3 }
    //     { 9 1 2 3 }         { 5 6 }         { 7 8 }         { 4 5 }         { 6 7 }
    //     { 4 5 6 7 }

    std::vector<double> B11, B12, B21, B22;

    if (rank == 0) {
        ASSERT_NO_THROW({getFourMatrixBlocks(n, A, &B11, &B12, &B21, &B22);});
        for (int i = 0; i < new_size; i++) {
            for (int j = 0; j < new_size; j++) {
                EXPECT_EQ(A11[j + i * new_size], B11[j + i * new_size]);
                EXPECT_EQ(A12[j + i * new_size], B12[j + i * new_size]);
                EXPECT_EQ(A21[j + i * new_size], B21[j + i * new_size]);
                EXPECT_EQ(A22[j + i * new_size], B22[j + i * new_size]);
            }
        }
    }
}

TEST(getJoinBlocksIntoMatrix, Test_join_blocks_into_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 4, new_size = 2;
    std::vector<double> A = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7};
    std::vector<double> A11 = {1, 2, 5, 6}, A12 = {3, 4, 7, 8};
    std::vector<double> A21 = {9, 1, 4, 5}, A22 = {2, 3, 6, 7};

    //     { 1 2 3 4 }
    // A = { 5 6 7 8 } , A11 = { 1 2 } , A12 = { 3 4 } , A21 = { 9 1 } , A22 = { 2 3 }
    //     { 9 1 2 3 }         { 5 6 }         { 7 8 }         { 4 5 }         { 6 7 }
    //     { 4 5 6 7 }

    std::vector<double> B;

    if (rank == 0) {
        ASSERT_NO_THROW({B = getJoinBlocksIntoMatrix(new_size, A11, A12, A21, A22);});
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                EXPECT_EQ(A[j + i * n], B[j + i * n]);
            }
        }
    }
}

TEST(getSequentialStrassenMatrixMultiplication, Test_sequential_strassen_matrix_multiplication_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> A = {3, 2, -5, 2, -1, 3, 1, 2, -1};
    std::vector<double> B = {2, 8, -5, 4, 2, 8, 1, -7, 6};

    //     { 3  2 -5 }       { 2  8 -5 }         { 9   63  -29 }
    // A = { 2 -1  3 } , B = { 4  2  8 } --> C = { 3   -7    0 }
    //     { 1  2 -1 }       { 1 -7  6 }         { 9   19    5 }

    std::vector<double> C, ans = {9, 63, -29, 0, 3, -7, 0, 0, 9, 19, 5, 0, 0, 0, 0, 0};
    int n = 3;

    if (rank == 0) {
        ASSERT_NO_THROW({C = getSequentialStrassenMatrixMultiplication(n, A, B);});
        for (int i = 0; i < n + 1; i++) {
            for (int j = 0; j < n + 1; j++) {
                EXPECT_NEAR(C[j + i * n], ans[j + i * n], eps);
            }
        }
    }
}

TEST(getSequentialStrassenMatrixMultiplication, Test_sequential_strassen_matrix_multiplication_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 256;
    std::vector<double> A = getRandomMatrix(n, 0.0, 5.0);
    std::vector<double> B = getRandomMatrix(n, 0.0, 5.0);

    std::vector<double> C, ans;

    if (rank == 0) {
        ASSERT_NO_THROW({ans = getSequentialSquareMatrixMultiplication(n, A, B);});
        ASSERT_NO_THROW({C = getSequentialStrassenMatrixMultiplication(n, A, B);});
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                EXPECT_NEAR(C[j + i * n], ans[j + i * n], eps);
            }
        }
    }
}

TEST(getParallelStrassenMatrixMultiplication, Test_parallel_strassen_matrix_multiplication_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 128;
    std::vector<double> A = getRandomMatrix(n, 0.0, 5.0);
    std::vector<double> B = getRandomMatrix(n, 0.0, 5.0);

    std::vector<double> C, ans;

    ASSERT_NO_THROW({C = getParallelStrassenMatrixMultiplication(n, A, B);});

    if (rank == 0) {
        ASSERT_NO_THROW({ans = getSequentialSquareMatrixMultiplication(n, A, B);});
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                EXPECT_NEAR(C[j + i * n], ans[j + i * n], eps);
            }
        }
    }
}

TEST(getParallelStrassenMatrixMultiplication, Test_parallel_strassen_matrix_multiplication_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 240;
    std::vector<double> A = getRandomMatrix(n, 0.0, 5.0);
    std::vector<double> B = getRandomMatrix(n, 0.0, 5.0);

    std::vector<double> C, ans;

    ASSERT_NO_THROW({C = getParallelStrassenMatrixMultiplication(n, A, B);});

    if (rank == 0) {
        ASSERT_NO_THROW({ans = getSequentialSquareMatrixMultiplication(n, A, B);});
        ans = getExpandSquareMatrix(n, ans);
        int new_size = getNewSize(n);

        for (int i = 0; i < new_size; i++) {
            for (int j = 0; j < new_size; j++) {
                EXPECT_NEAR(C[j + i * n], ans[j + i * n], eps);
            }
        }
    }
}

TEST(getParallelStrassenMatrixMultiplication, Test_parallel_strassen_matrix_multiplication_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 256;
    std::vector<double> A = getRandomMatrix(n, 0.0, 5.0);
    std::vector<double> B = getRandomMatrix(n, 0.0, 5.0);

    std::vector<double> C, ans;

    ASSERT_ANY_THROW({C = getParallelStrassenMatrixMultiplication(n - 1, A, B);});
}

/*TEST(getParallelStrassenMatrixMultiplication, Test_time_sequential_mult_and_parallel_strassen)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 512;
    std::vector<double> A = getRandomMatrix(n, 0.0, 5.0);
    std::vector<double> B = getRandomMatrix(n, 0.0, 5.0);

    std::vector<double> C, ans; 
    double t1, t2, t3, t4;

    t1 = MPI_Wtime();
    ASSERT_NO_THROW({C = getParallelStrassenMatrixMultiplication(n, A, B);});
    t2 = MPI_Wtime();

    if (rank == 0) {
        t3 = MPI_Wtime();
        ASSERT_NO_THROW({ans = getSequentialSquareMatrixMultiplication(n, A, B);});
        t4 = MPI_Wtime();

        std::cout << "Parallel strassen: " << t2 - t1 << " sec" << std::endl;
        std::cout << "Sequential mult: " << t4 - t3 << " sec" << std::endl;
        
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                EXPECT_NEAR(C[j + i * n], ans[j + i * n], eps);
            }
        }
    }
}

TEST(getParallelStrassenMatrixMultiplication, Test_time_sequential_strassen_and_parallel_strassen)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 512;
    std::vector<double> A = getRandomMatrix(n, 0.0, 5.0);
    std::vector<double> B = getRandomMatrix(n, 0.0, 5.0);

    std::vector<double> C, ans; 
    double t1, t2, t3, t4;

    t1 = MPI_Wtime();
    ASSERT_NO_THROW({C = getParallelStrassenMatrixMultiplication(n, A, B);});
    t2 = MPI_Wtime();

    if (rank == 0) {
        t3 = MPI_Wtime();
        ASSERT_NO_THROW({ans = getSequentialStrassenMatrixMultiplication(n, A, B);});
        t4 = MPI_Wtime();

        std::cout << "Parallel strassen: " << t2 - t1 << " sec" << std::endl;
        std::cout << "Sequential strassen: " << t4 - t3 << " sec" << std::endl;
        
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                EXPECT_NEAR(C[j + i * n], ans[j + i * n], eps);
            }
        }
    }
}*/

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

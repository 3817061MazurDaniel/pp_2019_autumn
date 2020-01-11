// Copyright 2019 Kudryashov Nikita
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./fox_algorithm.h"

TEST(FOX, throw_size_not_equal) {
    const unsigned int N_1 = 10;
    const unsigned int N_2 = 20;
    std::vector<double> a(N_1 * N_1);
    std::vector<double> b(N_2 * N_2);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        ASSERT_ANY_THROW(fox_mult(a, N_1, b, N_2));
    }
}

TEST(FOX, small_zero_matrix) {
    const unsigned int N = 2;  // Matrix size.
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    bool equal = true;
    std::vector<double> a(N * N);
    std::vector<double> b(N * N);
    std::vector<double> c;
    for (unsigned int i = 0; i < N; i++) {
        for (unsigned int j = 0; j < N; j++) {
            a[i*N+j] = 0.0;
            b[i*N+j] = 0.0;
        }
    }
    c = fox_mult(a, N, b, N);
    if (rank == 0) {
        for (unsigned int i = 0; i < N; i++) {
            for (unsigned int j = 0; j < N; j++) {
                if (c[i*N+j] != 0) {
                    equal = false;
                }
            }
        }
        ASSERT_EQ(true, equal);
    }
}

TEST(FOX, big_zero_matrix) {
    const unsigned int N = 100;  // Matrix size.
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    bool equal = true;
    std::vector<double> a(N * N);
    std::vector<double> b(N * N);
    std::vector<double> c;
    for (unsigned int i = 0; i < N; i++) {
        for (unsigned int j = 0; j < N; j++) {
            a[i*N+j] = 0.0;
            b[i*N+j] = 0.0;
        }
    }
    c = fox_mult(a, N, b, N);
    if (rank == 0) {
        for (unsigned int i = 0; i < N; i++) {
            for (unsigned int j = 0; j < N; j++) {
                if (c[i*N+j] != 0.0) {
                    equal = false;
                }
            }
        }
        ASSERT_EQ(true, equal);
    }
}

TEST(FOX, small_non_zero_matrix) {
    const unsigned int N = 2;  // Matrix size.
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    bool equal = true;
    std::vector<double> a(N * N);
    std::vector<double> b(N * N);
    std::vector<double> res_seq;
    std::vector<double> res_fox;
    int k1 = 0;
    int k2 = 4;
    for (unsigned int i = 0; i < N; i++) {
        for (unsigned int j = 0; j < N; j++) {
            a[i*N+j] = k1++;
            b[i*N+j] = k2++;
        }
    }

    res_seq = subtask_matr_mult(a, N, b, N);
    res_fox = fox_mult(a, N, b, N);

    if (rank == 0) {
        for (unsigned int i = 0; i < N; i++) {
            for (unsigned int j = 0; j < N; j++) {
                if (res_seq[i*N+j] != res_fox[i*N+j]) {
                    equal = false;
                }
            }
        }
        ASSERT_EQ(true, equal);
    }
}

TEST(FOX, meduim_non_zero_matr_1) {
    const unsigned int N = 7;  // Matrix size.
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    bool equal = true;
    std::vector<double> a(N * N);
    std::vector<double> b(N * N);
    std::vector<double> res_seq;
    std::vector<double> res_fox;
    int k1 = 0;
    int k2 = 4;
    for (unsigned int i = 0; i < N; i++) {
        for (unsigned int j = 0; j < N; j++) {
            a[i*N+j] = k1++;
            b[i*N+j] = k2++;
        }
    }

    res_seq = subtask_matr_mult(a, N, b, N);
    res_fox = fox_mult(a, N, b, N);

    if (rank == 0) {
        for (unsigned int i = 0; i < N; i++) {
            for (unsigned int j = 0; j < N; j++) {
                if (res_seq[i*N+j] != res_fox[i*N+j]) {
                    equal = false;
                }
            }
        }
        ASSERT_EQ(true, equal);
    }
}

TEST(FOX, meduim_non_zero_matr_2) {
    const unsigned int N = 15;  // Matrix size.
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    bool equal = true;
    std::vector<double> a(N * N);
    std::vector<double> b(N * N);
    std::vector<double> res_seq;
    std::vector<double> res_fox;
    int k1 = 0;
    int k2 = 4;
    for (unsigned int i = 0; i < N; i++) {
        for (unsigned int j = 0; j < N; j++) {
            a[i*N+j] = k1++;
            b[i*N+j] = k2++;
        }
    }

    res_seq = subtask_matr_mult(a, N, b, N);
    res_fox = fox_mult(a, N, b, N);

    if (rank == 0) {
        for (unsigned int i = 0; i < N; i++) {
            for (unsigned int j = 0; j < N; j++) {
                if (res_seq[i*N+j] != res_fox[i*N+j]) {
                    equal = false;
                }
            }
        }
        ASSERT_EQ(true, equal);
    }
}

TEST(FOX, big_non_zero_matr_1) {
    const unsigned int N = 75;  // Matrix size.
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    bool equal = true;
    std::vector<double> a(N * N);
    std::vector<double> b(N * N);
    std::vector<double> res_seq;
    std::vector<double> res_fox;
    int k1 = 0;
    int k2 = 4;
    for (unsigned int i = 0; i < N; i++) {
        for (unsigned int j = 0; j < N; j++) {
            a[i*N+j] = k1++;
            b[i*N+j] = k2++;
        }
    }

    res_seq = subtask_matr_mult(a, N, b, N);
    res_fox = fox_mult(a, N, b, N);

    if (rank == 0) {
        for (unsigned int i = 0; i < N; i++) {
            for (unsigned int j = 0; j < N; j++) {
                if (res_seq[i*N+j] != res_fox[i*N+j]) {
                    equal = false;
                }
            }
        }
        ASSERT_EQ(true, equal);
    }
}

TEST(FOX, big_non_zero_matr_2) {
    const unsigned int N = 103;  // Matrix size.
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    bool equal = true;
    std::vector<double> a(N * N);
    std::vector<double> b(N * N);
    std::vector<double> res_seq;
    std::vector<double> res_fox;
    int k1 = 0;
    int k2 = 4;
    for (unsigned int i = 0; i < N; i++) {
        for (unsigned int j = 0; j < N; j++) {
            a[i*N+j] = k1++;
            b[i*N+j] = k2++;
        }
    }

    res_seq = subtask_matr_mult(a, N, b, N);
    res_fox = fox_mult(a, N, b, N);

    if (rank == 0) {
        for (unsigned int i = 0; i < N; i++) {
            for (unsigned int j = 0; j < N; j++) {
                if (res_seq[i*N+j] != res_fox[i*N+j]) {
                    equal = false;
                }
            }
        }
        ASSERT_EQ(true, equal);
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

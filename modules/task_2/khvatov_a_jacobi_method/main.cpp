// Copyright 2019 Khvatov Alexander
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>

#include "./jacobi_method.h"

TEST(Jacobi_Method, Seq_Jacobi_No_Throw) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (0 == rank) {
        double epsilon = 0.1;
        int size = 4;
        double *A = new double[size * size]{ 20.9, 1.2, 2.1, 0.9, 1.2, 21.2, 1.5, 2.5, 2.1,
         1.5, 19.8, 1.2, 0.9, 2.5, 1.3, 32.1};
        double *b = new double[size]{ 21.7, 27.46, 28.76, 49.72 };
        double *v;

        EXPECT_NO_THROW(v = seq_jacobi_method(A, b, size, epsilon));

        delete[] A;
        delete[] b;
        delete[] v;
    }
}

TEST(Jacobi_Method, Seq_Jacobi_Method_Is_Correct) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (0 == rank) {
        double epsilon = 0.1;
        int size = 4;
        double *A = new double[size * size]{ 20.9, 1.2, 2.1, 0.9, 1.2, 21.2, 1.5, 2.5, 2.1,
         1.5, 19.8, 1.2, 0.9, 2.5, 1.3, 32.1};
        double *b = new double[size]{ 21.7, 27.46, 28.76, 49.72 };
        double *result = new double[size]{0.8, 1.002, 1.208, 1.377};
        double *v;

        v = seq_jacobi_method(A, b, size, epsilon);

        for (int i = 0; i < size; i++) {
            EXPECT_NEAR(result[i], v[i], epsilon);
        }

        delete[] A;
        delete[] b;
        delete[] result;
        delete[] v;
    }
}

TEST(Jacobi_Method, Par_Jacobi_Method_No_Throw) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double epsilon = 0.1;
    int size = 4;
    double *A = new double[size * size]{ 20.9, 1.2, 2.1, 0.9, 1.2, 21.2, 1.5, 2.5, 2.1,
        1.5, 19.8, 1.2, 0.9, 2.5, 1.3, 32.1};
    double *b = new double[size]{ 21.7, 27.46, 28.76, 49.72 };
    double *v;

    EXPECT_NO_THROW(v = par_jacobi_method(A, b, size, epsilon));

    delete[] A;
    delete[] b;
    delete[] v;
}

TEST(Jacobi_Method, Par_Jacobi_Method_Is_Correct) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double epsilon = 0.1;
    int size = 4;
    double *A = new double[size * size]{ 20.9, 1.2, 2.1, 0.9, 1.2, 21.2, 1.5, 2.5, 2.1,
        1.5, 19.8, 1.2, 0.9, 2.5, 1.3, 32.1};
    double *b = new double[size]{ 21.7, 27.46, 28.76, 49.72 };
    double *result = new double[size]{0.8, 1.002, 1.208, 1.377};
    double *v;

    v = par_jacobi_method(A, b, size, epsilon);

    if (0 == rank) {
        for (int i = 0; i < size; i++) {
            EXPECT_NEAR(result[i], v[i], epsilon);
        }
    }

    delete[] A;
    delete[] b;
    delete[] result;
    delete[] v;
}

TEST(Jacobi_Method, Par_Jacobi_Equal_Seq_Jacobi) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double epsilon = 0.1;
    int size = 4;
    double *A = new double[size * size]{ 20.9, 1.2, 2.1, 0.9, 1.2, 21.2, 1.5, 2.5, 2.1,
        1.5, 19.8, 1.2, 0.9, 2.5, 1.3, 32.1};
    double *b = new double[size]{ 21.7, 27.46, 28.76, 49.72 };
    double *v1 = NULL;
    double *v2 = NULL;

    v1 = par_jacobi_method(A, b, size, epsilon);

    if (0 == rank) {
        v2 = seq_jacobi_method(A, b, size, epsilon);

        for (int i = 0; i < size; i++) {
            EXPECT_NEAR(v1[i], v2[i], epsilon);
        }
    }

    delete[] A;
    delete[] b;
    delete[] v1;
    delete[] v2;
}

TEST(Jacobi_Method, Exec_time) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double epsilon = 0.1;
    int size = 4;
    double *A = new double[size * size]{ 20.9, 1.2, 2.1, 0.9, 1.2, 21.2, 1.5, 2.5, 2.1,
        1.5, 19.8, 1.2, 0.9, 2.5, 1.3, 32.1};
    double *b = new double[size]{ 21.7, 27.46, 28.76, 49.72 };

    double start = MPI_Wtime();
    par_jacobi_method(A, b, size, epsilon);
    double end = MPI_Wtime();

    if (0 == rank) {
        std::cout << "Exec time = " << end - start << std::endl;
    }
    delete[] A;
    delete[] b;
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

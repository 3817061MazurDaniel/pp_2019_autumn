// Copyright 2019 Mityagina Daria
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include "../../../modules/task_3/mityagina_d_quick_sort/quick_sort_p.h"

void testing_lab(int size) {
    int rank;
    // double t1, t2;
    std::vector<int> _vector(size), result_my(size), result(size), copy(size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        _vector = getRandomVector(size);
    }
    // t1 = MPI_Wtime();
    result = main_work(_vector, size);
    // t2 = MPI_Wtime();
    if (rank == 0) {
      // std::cout << "Parallel " << t2 - t1 << "\n";
      // t1 = MPI_Wtime();
      quick_s(_vector, 0, size - 1);
      sort(_vector.begin(), _vector.end());
      // t2 = MPI_Wtime();
      // std::cout << "Not parallel " << t2 - t1 << "\n";
      result_my = _vector;
    }
    if (rank == 0) {
        ASSERT_EQ(result, result_my);
    }
}

TEST(Quick_Sort_MPI, Test_On_Size_1) {
    int size = 1;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_5) {
    int size = 5;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_7) {
    int size = 7;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_9) {
    int size = 9;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_91) {
    int size = 91;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_200) {
    int size = 200;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_1000) {
    int size = 1000;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_2000) {
    int size = 2000;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Negative_matrix_size) {
    int size = -10;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_ANY_THROW(std::vector<int> _vector(size, 0));
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
    MPI_Finalize();
}

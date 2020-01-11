  // Copyright 2019 Boganov Sergey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "./gauss_method_horizontal_scheme.h"

TEST(getSequentialGauss, Test_error_size_martix_sequential_gauss) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> m = {1, 2, 3, 4, 5, 6};

    // 1 2 | 3
    // 4 5 | 6

    // -1 2

    std::vector<double> temp;

    if (rank == 0) {
        ASSERT_ANY_THROW({temp = getSequentialGauss(m, 5);});
    }
}

TEST(getSequentialGauss, Test_2_x_2_martix_sequential_gauss) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> m = {1, 2, 3, 4, 5, 6};

    // 1 2 | 3
    // 4 5 | 6

    // -1 2

    std::vector<double> temp;

    if (rank == 0) {
        ASSERT_NO_THROW({temp = getSequentialGauss(m, 2);});
        EXPECT_NEAR(temp[0], -1.0, 1e-6);
        EXPECT_NEAR(temp[1], 2.0, 1e-6);
        EXPECT_TRUE(Ax_b_check(m, 2, temp));
    }
}

TEST(getSequentialGauss, Test_3_x_3_martix_sequential_gauss) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> m = {3, 2, -5, -1, 2, -1, 3, 13, 1, 2, -1, 9};

    // 3  2 -5 | -1
    // 2 -1  3 | 13
    // 1  2 -1 |  9

    // 3 5 4

    std::vector<double> temp;

    if (rank == 0) {
        ASSERT_NO_THROW({temp = getSequentialGauss(m, 3);});
        EXPECT_NEAR(temp[0], 3.0, 1e-6);
        EXPECT_NEAR(temp[1], 5.0, 1e-6);
        EXPECT_NEAR(temp[2], 4.0, 1e-6);
        EXPECT_TRUE(Ax_b_check(m, 3, temp));
    }
}

TEST(getSequentialGauss, Test_4_x_4_martix_sequential_gauss) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> m = {1, 0, 0, 0, 6, 0, 1, 0, 0, 3, 0, 0, 1, 0, 8, 0, 0, 0, 1, 2};

    // 1 0 0 0| 6
    // 0 1 0 0| 3
    // 0 0 1 0| 8
    // 0 0 0 1| 2

    // 6 3 8 2

    std::vector<double> temp;

    if (rank == 0) {
        ASSERT_NO_THROW({temp = getSequentialGauss(m, 4);});
        EXPECT_NEAR(temp[0], 6.0, 1e-6);
        EXPECT_NEAR(temp[1], 3.0, 1e-6);
        EXPECT_NEAR(temp[2], 8.0, 1e-6);
        EXPECT_NEAR(temp[3], 2.0, 1e-6);
        EXPECT_TRUE(Ax_b_check(m, 4, temp));
    }
}

TEST(getSequentialGauss, Test_incompatible_system_1_sequential_gauss) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> m = {1, 4, 7, 5, 2, 5, 8, 2, 3, 6, 9, 8};

    // 1 4 7 | 5
    // 2 5 8 | 2
    // 3 6 9 | 8

    // Система несовместна

    std::vector<double> temp;

    if (rank == 0) {
        ASSERT_ANY_THROW({temp = getSequentialGauss(m, 3);});
    }
}

TEST(getSequentialGauss, Test_incompatible_system_2_sequential_gauss) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> m = {1, 2, 3, 1, 2, 7};

    // 1 2 | 3
    // 1 2 | 7

    // Система несовместна

    std::vector<double> temp;

    if (rank == 0) {
        ASSERT_ANY_THROW({temp = getSequentialGauss(m, 2);});
    }
}

TEST(getSequentialGauss, Test_infinite_number_of_solutions_sequential_gauss) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> m = {2, 5, 3, 2, 5, 3};

    // 2 5 | 3
    // 2 5 | 3

    // Система имеет бесконечное множество решений

    std::vector<double> temp;

    if (rank == 0) {
        ASSERT_ANY_THROW({temp = getSequentialGauss(m, 2);});
    }
}

TEST(getSequentialMaxElem, Test_find_max_elem_and_index) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> m = {1, 2, 3, 4, 5, 6};

    // 1 2 | 3
    // 4 5 | 6

    // max_elem = 4, row_index = 1

    if (rank == 0) {
        double max_elem = 0;
        unsigned int row_index = 0, one = 1;

        max_elem = getSequentialMaxElem(m, 2, 0, 2, 0, &row_index);

        EXPECT_NEAR(max_elem, 4.0, 1e-6);
        EXPECT_EQ(row_index, one);
    }
}

TEST(findRank, Test_findRank_5_x_5) {
    // Пусть матрица 5х5, выбран элемент, стоящий в 4 строке.
    // Нам нужен rank процесса, в локальную матрицу которого входит этот элемент.
    // Если процессов 2, то матрица будет разбита на 2: первая матрица включает первые 2 строки,
    // вторая матрица включает последние 3 строки.
    // Значит, элемент стоящий в 4 строке должен находится в локальной матрице процесса с рангом 1.
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        EXPECT_EQ(findRank(5, 2, 2, 1, 3), 1);
    }
}

TEST(findRank, Test_findRank_10_x_10) {
    // Пусть матрица 10х10, выбран элемент, стоящий в 7 строке.
    // Нам нужен rank процесса, в локальную матрицу которого входит этот элемент.
    // Если процессов 3, то матрица будет разбита на 3 матрицы.
    // Первые 2 матрицы будут иметь по 3 строки, а 3-я 4 строки.
    // Значит, элемент стоящий в 7 строке должен находится в локальной матрице процесса с рангом 2.
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        EXPECT_EQ(findRank(9, 3, 3, 1, 6), 2);
    }
}

TEST(getParallelGauss, Test_2_x_2_martix_parallel_gauss) {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> m = {1, 2, 3, 4, 5, 6};

    // 1 2 | 3
    // 4 5 | 6

    // -1 2

    int n = 2;

    std::vector<double> temp;

    ASSERT_NO_THROW({temp = getParallelGauss(m, 2);});

    if (rank == n - 1) {
        EXPECT_NEAR(temp[0], -1.0, 1e-6);
        EXPECT_NEAR(temp[1], 2.0, 1e-6);
        EXPECT_TRUE(Ax_b_check(m, 2, temp));
    }
}

TEST(getParallelGauss, Test_3_x_3_martix_parallel_gauss) {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> m = {3, 2, -5, -1, 2, -1, 3, 13, 1, 2, -1, 9};

    // 3  2 -5 | -1
    // 2 -1  3 | 13
    // 1  2 -1 |  9

    // 3 5 4

    int n = 3;

    std::vector<double> temp;

    ASSERT_NO_THROW({temp = getParallelGauss(m, 3);});

    if (rank == n - 1) {
        EXPECT_NEAR(temp[0], 3.0, 1e-6);
        EXPECT_NEAR(temp[1], 5.0, 1e-6);
        EXPECT_NEAR(temp[2], 4.0, 1e-6);
        EXPECT_TRUE(Ax_b_check(m, 3, temp));
    }
}

TEST(getParallelGauss, Test_4_x_4_martix_parallel_gauss) {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> m = {1, 0, 0, 0, 6, 0, 1, 0, 0, 3, 0, 0, 1, 0, 8, 0, 0, 0, 1, 2};

    // 1 0 0 0| 6
    // 0 1 0 0| 3
    // 0 0 1 0| 8
    // 0 0 0 1| 2

    // 6 3 8 2

    int n = 4;

    std::vector<double> temp;

    ASSERT_NO_THROW({temp = getParallelGauss(m, 4);});

    if (rank == n - 1) {
        EXPECT_NEAR(temp[0], 6.0, 1e-6);
        EXPECT_NEAR(temp[1], 3.0, 1e-6);
        EXPECT_NEAR(temp[2], 8.0, 1e-6);
        EXPECT_NEAR(temp[3], 2.0, 1e-6);
        EXPECT_TRUE(Ax_b_check(m, 4, temp));
    }
}

TEST(getParralelGauss, Test_error_size_martix_parallel_gauss) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> m = {1, 2, 3, 4, 5, 6};

    // 1 2 | 3
    // 4 5 | 6

    // -1 2

    std::vector<double> temp;

    ASSERT_ANY_THROW({temp = getParallelGauss(m, 5);});
}

TEST(getParallelGauss, Test_incompatible_system_1_parallel_gauss) {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> m = {1, 4, 7, 5, 2, 5, 8, 2, 3, 6, 9, 8};

    // 1 4 7 | 5
    // 2 5 8 | 2
    // 3 6 9 | 8

    // Система несовместна

    std::vector<double> temp;

    ASSERT_ANY_THROW({temp = getParallelGauss(m, 3);});
}

TEST(getParallelGauss, Test_incompatible_system_2_parallel_gauss) {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> m = {1, 2, 3, 1, 2, 7};

    // 1 2 | 3
    // 1 2 | 7

    // Система несовместна

    std::vector<double> temp;

    ASSERT_ANY_THROW({temp = getParallelGauss(m, 2);});
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

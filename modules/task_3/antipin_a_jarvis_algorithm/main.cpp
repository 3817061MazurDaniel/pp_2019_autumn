// Copyright 2019 Antipin Alexander
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "./jarvis_algorithm.h"

TEST(jarvis_algorithm, can_generate_random_field_of_points) {
    std::vector<point> field(10);
    ASSERT_NO_THROW(getRandomFieldOfPoints(&field, 25, 5, 25, 5));
}

TEST(jarvis_algorithm, can_get_convex_hill_by_sequential_method_0) {
    std::vector<point> field(10);
    field[0].setX(15); field[0].setY(6);
    field[1].setX(20); field[1].setY(19);
    field[2].setX(9); field[2].setY(20);
    field[3].setX(9); field[3].setY(23);
    field[4].setX(17); field[4].setY(15);
    field[5].setX(21); field[5].setY(5);
    field[6].setX(15); field[6].setY(24);
    field[7].setX(18); field[7].setY(23);
    field[8].setX(23); field[8].setY(13);
    field[9].setX(13); field[9].setY(9);
    std::vector<int> res;
    ASSERT_NO_THROW(getSequentialSolution(field, &res));
}

TEST(jarvis_algorithm, can_get_convex_hill_by_sequential_method_0_correct) {
    std::vector<point> field(10);
    field[0].setX(14); field[0].setY(14);
    field[1].setX(19); field[1].setY(19);
    field[2].setX(12); field[2].setY(14);
    field[3].setX(10); field[3].setY(15);
    field[4].setX(6); field[4].setY(20);
    field[5].setX(20); field[5].setY(16);
    field[6].setX(8); field[6].setY(11);
    field[7].setX(17); field[7].setY(6);
    field[8].setX(19); field[8].setY(22);
    field[9].setX(15); field[9].setY(13);
    std::vector<int> res;
    getSequentialSolution(field, &res);
    EXPECT_EQ(res[0], 4);
    EXPECT_EQ(res[1], 6);
    EXPECT_EQ(res[2], 7);
    EXPECT_EQ(res[3], 5);
    EXPECT_EQ(res[4], 8);
}

TEST(jarvis_algorithm, can_get_convex_hill_by_sequential_method_1) {
    std::vector<point> field(10);
    field[0].setX(22); field[0].setY(9);
    field[1].setX(7); field[1].setY(19);
    field[2].setX(10); field[2].setY(18);
    field[3].setX(20); field[3].setY(23);
    field[4].setX(20); field[4].setY(18);
    field[5].setX(13); field[5].setY(5);
    field[6].setX(23); field[6].setY(10);
    field[7].setX(23); field[7].setY(22);
    field[8].setX(7); field[8].setY(21);
    field[9].setX(23); field[9].setY(23);
    std::vector<int> res;
    ASSERT_NO_THROW(getSequentialSolution(field, &res));
}

TEST(jarvis_algorithm, can_get_convex_hill_by_sequential_method_2) {
    std::vector<point> field(10);
    field[0].setX(9); field[0].setY(8);
    field[1].setX(18); field[1].setY(13);
    field[2].setX(19); field[2].setY(11);
    field[3].setX(23); field[3].setY(9);
    field[6].setX(18); field[6].setY(24);
    field[5].setX(9); field[5].setY(15);
    field[4].setX(19); field[4].setY(22);
    field[7].setX(19); field[7].setY(15);
    field[8].setX(23); field[8].setY(14);
    field[9].setX(24); field[9].setY(22);
    std::vector<int> res;
    ASSERT_NO_THROW(getSequentialSolution(field, &res));
}

TEST(jarvis_algorithm, can_get_convex_hill_by_sequential_method_3) {
    std::vector<point> field(10);
    field[0].setX(14); field[0].setY(9);
    field[1].setX(5); field[1].setY(6);
    field[2].setX(16); field[2].setY(12);
    field[3].setX(5); field[3].setY(22);
    field[4].setX(10); field[4].setY(10);
    field[5].setX(19); field[5].setY(18);
    field[6].setX(5); field[6].setY(17);
    field[7].setX(20); field[7].setY(15);
    field[8].setX(5); field[8].setY(6);
    field[9].setX(20); field[9].setY(6);
    std::vector<int> res;
    ASSERT_NO_THROW(getSequentialSolution(field, &res));
}

TEST(jarvis_algorithm, can_get_convex_hull_by_sequential_method_4) {
    std::vector<point> field(10);
    getRandomFieldOfPoints(&field, 25, 5, 25, 5);
    std::vector<int> res;
    /*for (int i = 0; i < 10; ++i) {
        std::cout << field[i].getX() << " " << field[i].getY() << std::endl;
    }*/
    ASSERT_NO_THROW(getSequentialSolution(field, &res));
}

TEST(jarvis_algorithm, can_get_convex_hill_by_parallel_method_0) {
    std::vector<point> field(10);
    field[0].setX(14); field[0].setY(14);
    field[1].setX(19); field[1].setY(19);
    field[2].setX(12); field[2].setY(14);
    field[3].setX(10); field[3].setY(15);
    field[4].setX(6); field[4].setY(20);
    field[5].setX(20); field[5].setY(16);
    field[6].setX(8); field[6].setY(11);
    field[7].setX(17); field[7].setY(6);
    field[8].setX(19); field[8].setY(22);
    field[9].setX(15); field[9].setY(13);
    std::vector<int> res;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ASSERT_NO_THROW(getParallelSolution(field, &res));
}

TEST(jarvis_algorithm, can_get_convex_hill_by_parallel_method_0_correct) {
    std::vector<point> field(10);
    field[0].setX(14); field[0].setY(14);
    field[1].setX(19); field[1].setY(19);
    field[2].setX(12); field[2].setY(14);
    field[3].setX(10); field[3].setY(15);
    field[4].setX(6); field[4].setY(20);
    field[5].setX(20); field[5].setY(16);
    field[6].setX(8); field[6].setY(11);
    field[7].setX(17); field[7].setY(6);
    field[8].setX(19); field[8].setY(22);
    field[9].setX(15); field[9].setY(13);
    std::vector<int> res;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    getParallelSolution(field, &res);
    if (rank == 0) {
        EXPECT_EQ(res[0], 4);
        EXPECT_EQ(res[1], 6);
        EXPECT_EQ(res[2], 7);
        EXPECT_EQ(res[3], 5);
        EXPECT_EQ(res[4], 8);
    }
}

TEST(jarvis_algorithm, can_get_convex_hull_by_parallel_method_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<point> field(10);
    double* arrX = new double[10];
    double* arrY = new double[10];
    if (rank == 0) {
        getRandomFieldOfPoints(&field, 25, 5, 25, 5);
        for (int i = 0; i < 10; ++i) {
            arrX[i] = field[i].getX();
            arrY[i] = field[i].getY();
            // std::cout << field[i].getX() << " " << field[i].getY() << std::endl;
        }
    }
    MPI_Bcast(arrX, 10, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(arrY, 10, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    std::vector<int> res;
    if (rank != 0) {
        for (int i = 0; i < 10; ++i) {
            field[i].setX(arrX[i]);
            field[i].setY(arrY[i]);
        }
    }
    ASSERT_NO_THROW(getParallelSolution(field, &res));
    /*if (rank == 0) {
        std::vector<int> res2;
        getSequentialSolution(field, res2);
        for (int i = 0; i < res.size(); ++i) {
            std::cout << res[i] << " ";
        }
        std::cout << std::endl;
        for (int i = 0; i < res2.size(); ++i) {
            std::cout << res2[i] << " ";
        }
    }*/
}

TEST(jarvis_algorithm, can_get_convex_hull_by_parallel_method_1_correct) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<point> field(10);
    double* arrX = new double[10];
    double* arrY = new double[10];
    if (rank == 0) {
        getRandomFieldOfPoints(&field, 25, 5, 25, 5);
        for (int i = 0; i < 10; ++i) {
            arrX[i] = field[i].getX();
            arrY[i] = field[i].getY();
            // std::cout << field[i].getX() << " " << field[i].getY() << std::endl;
        }
    }
    MPI_Bcast(arrX, 10, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(arrY, 10, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    std::vector<int> res;
    if (rank != 0) {
        for (int i = 0; i < 10; ++i) {
            field[i].setX(arrX[i]);
            field[i].setY(arrY[i]);
        }
    }
    getParallelSolution(field, &res);
    if (rank == 0) {
        std::vector<int> res2;
        getSequentialSolution(field, &res2);
        for (unsigned int i = 0; i < res.size(); ++i) {
            EXPECT_EQ(res[i], res2[i]);
        }
    }
}

/*TEST(jarvis_algorithm, time_check) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<point> field(1000000);
    double* arrX = new double[1000000];
    double* arrY = new double[1000000];
    if (rank == 0) {
        getRandomFieldOfPoints(&field, 1000, 5, 1000, 5);
        for (int i = 0; i < 1000000; ++i) {
            arrX[i] = field[i].getX();
            arrY[i] = field[i].getY();
            // std::cout << field[i].getX() << " " << field[i].getY() << std::endl;
        }
    }
    MPI_Bcast(arrX, 1000000, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(arrY, 1000000, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    std::vector<int> res;
    if (rank != 0) {
        for (int i = 0; i < 1000000; ++i) {
            field[i].setX(arrX[i]);
            field[i].setY(arrY[i]);
        }
    }
    double start = MPI_Wtime();
    getParallelSolution(field, &res);
    double end = MPI_Wtime();
    if (rank == 0) {
        std::cout << "Time of parallel method - " << end - start << std::endl;
        std::vector<int> res2;
        double st = MPI_Wtime();
        getSequentialSolution(field, &res2);
        double ed = MPI_Wtime();
        std::cout << "Time of sequential method - " << ed - st << std::endl;
    }
}*/

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    srand(time(NULL));
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}

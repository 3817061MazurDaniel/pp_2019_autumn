// Copyright 2019 Obolenskiy Arseniy
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <random>
#include "./gaussian_image_filtering.h"

TEST(Gaussian_Image_Filtering_MPI, Test_10x5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Pixel> a;
    const int rows = 10;
    const int cols = 5;

    if (rank == 0) {
        a = getRandomMatrix(rows, cols);
    }

    std::vector <Pixel> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <Pixel> seqAnswer = solveSequential(a, rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                Pixel p = seqAnswer[i * cols + j];
                Pixel q = answer[i * cols + j];
                EXPECT_FALSE(p.r != q.r || p.g != q.g || p.b != q.b);
            }
        }
    }
}

TEST(Gaussian_Image_Filtering_MPI, Test_15x15) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Pixel> a;
    const int rows = 15;
    const int cols = 15;

    if (rank == 0) {
        a = getRandomMatrix(rows, cols);
    }

    std::vector <Pixel> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <Pixel> seqAnswer = solveSequential(a, rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                Pixel p = seqAnswer[i * cols + j];
                Pixel q = answer[i * cols + j];
                EXPECT_FALSE(p.r != q.r || p.g != q.g || p.b != q.b);
            }
        }
    }
}

TEST(Gaussian_Image_Filtering_MPI, Test_1x50) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Pixel> a;
    const int rows = 1;
    const int cols = 50;

    if (rank == 0) {
        a = getRandomMatrix(rows, cols);
    }

    std::vector <Pixel> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <Pixel> seqAnswer = solveSequential(a, rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                Pixel p = seqAnswer[i * cols + j];
                Pixel q = answer[i * cols + j];
                EXPECT_FALSE(p.r != q.r || p.g != q.g || p.b != q.b);
            }
        }
    }
}

TEST(Gaussian_Image_Filtering_MPI, Test_50x1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Pixel> a;
    const int rows = 50;
    const int cols = 1;

    if (rank == 0) {
        a = getRandomMatrix(rows, cols);
    }

    std::vector <Pixel> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <Pixel> seqAnswer = solveSequential(a, rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                Pixel p = seqAnswer[i * cols + j];
                Pixel q = answer[i * cols + j];
                EXPECT_FALSE(p.r != q.r || p.g != q.g || p.b != q.b);
            }
        }
    }
}

TEST(Gaussian_Image_Filtering_MPI, Test_2x50) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Pixel> a;
    const int rows = 2;
    const int cols = 50;

    if (rank == 0) {
        a = getRandomMatrix(rows, cols);
    }

    std::vector <Pixel> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <Pixel> seqAnswer = solveSequential(a, rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                Pixel p = seqAnswer[i * cols + j];
                Pixel q = answer[i * cols + j];
                EXPECT_FALSE(p.r != q.r || p.g != q.g || p.b != q.b);
            }
        }
    }
}

TEST(Gaussian_Image_Filtering_MPI, Test_50x2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Pixel> a;
    const int rows = 50;
    const int cols = 2;

    if (rank == 0) {
        a = getRandomMatrix(rows, cols);
    }

    std::vector <Pixel> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <Pixel> seqAnswer = solveSequential(a, rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                Pixel p = seqAnswer[i * cols + j];
                Pixel q = answer[i * cols + j];
                EXPECT_FALSE(p.r != q.r || p.g != q.g || p.b != q.b);
            }
        }
    }
}

TEST(Gaussian_Image_Filtering_MPI, Test_3x50) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Pixel> a;
    const int rows = 3;
    const int cols = 50;

    if (rank == 0) {
        a = getRandomMatrix(rows, cols);
    }

    std::vector <Pixel> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <Pixel> seqAnswer = solveSequential(a, rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                Pixel p = seqAnswer[i * cols + j];
                Pixel q = answer[i * cols + j];
                EXPECT_FALSE(p.r != q.r || p.g != q.g || p.b != q.b);
            }
        }
    }
}

TEST(Gaussian_Image_Filtering_MPI, Test_50x3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Pixel> a;
    const int rows = 50;
    const int cols = 3;

    if (rank == 0) {
        a = getRandomMatrix(rows, cols);
    }

    std::vector <Pixel> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <Pixel> seqAnswer = solveSequential(a, rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                Pixel p = seqAnswer[i * cols + j];
                Pixel q = answer[i * cols + j];
                EXPECT_FALSE(p.r != q.r || p.g != q.g || p.b != q.b);
            }
        }
    }
}

TEST(Gaussian_Image_Filtering_MPI, Test_4x50) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Pixel> a;
    const int rows = 4;
    const int cols = 50;

    if (rank == 0) {
        a = getRandomMatrix(rows, cols);
    }

    std::vector <Pixel> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <Pixel> seqAnswer = solveSequential(a, rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                Pixel p = seqAnswer[i * cols + j];
                Pixel q = answer[i * cols + j];
                EXPECT_FALSE(p.r != q.r || p.g != q.g || p.b != q.b);
            }
        }
    }
}

TEST(Gaussian_Image_Filtering_MPI, Test_50x4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Pixel> a;
    const int rows = 50;
    const int cols = 4;

    if (rank == 0) {
        a = getRandomMatrix(rows, cols);
    }

    std::vector <Pixel> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <Pixel> seqAnswer = solveSequential(a, rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                Pixel p = seqAnswer[i * cols + j];
                Pixel q = answer[i * cols + j];
                EXPECT_FALSE(p.r != q.r || p.g != q.g || p.b != q.b);
            }
        }
    }
}

TEST(Gaussian_Image_Filtering_MPI, Test_5x50) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Pixel> a;
    const int rows = 5;
    const int cols = 50;

    if (rank == 0) {
        a = getRandomMatrix(rows, cols);
    }

    std::vector <Pixel> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <Pixel> seqAnswer = solveSequential(a, rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                Pixel p = seqAnswer[i * cols + j];
                Pixel q = answer[i * cols + j];
                EXPECT_FALSE(p.r != q.r || p.g != q.g || p.b != q.b);
            }
        }
    }
}

TEST(Gaussian_Image_Filtering_MPI, Test_50x5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Pixel> a;
    const int rows = 50;
    const int cols = 5;

    if (rank == 0) {
        a = getRandomMatrix(rows, cols);
    }

    std::vector <Pixel> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <Pixel> seqAnswer = solveSequential(a, rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                Pixel p = seqAnswer[i * cols + j];
                Pixel q = answer[i * cols + j];
                EXPECT_FALSE(p.r != q.r || p.g != q.g || p.b != q.b);
            }
        }
    }
}

TEST(Gaussian_Image_Filtering_MPI, Test_small_random_image) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Pixel> a;
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(1, 10);
    const int rows = dist(gen);
    const int cols = dist(gen);

    if (rank == 0) {
        a = getRandomMatrix(rows, cols);
    }

    std::vector <Pixel> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <Pixel> seqAnswer = solveSequential(a, rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                Pixel p = seqAnswer[i * cols + j];
                Pixel q = answer[i * cols + j];
                EXPECT_FALSE(p.r != q.r || p.g != q.g || p.b != q.b);
            }
        }
    }
}

TEST(Gaussian_Image_Filtering_MPI, Test_random_image) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Pixel> a;
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(1, 100);
    const int rows = dist(gen);
    const int cols = dist(gen);

    if (rank == 0) {
        a = getRandomMatrix(rows, cols);
    }

    std::vector <Pixel> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <Pixel> seqAnswer = solveSequential(a, rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                Pixel p = seqAnswer[i * cols + j];
                Pixel q = answer[i * cols + j];
                EXPECT_FALSE(p.r != q.r || p.g != q.g || p.b != q.b);
            }
        }
    }
}

TEST(Gaussian_Image_Filtering_MPI, Test_large_random_image) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Pixel> a;
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(1, 1000);
    const int rows = dist(gen);
    const int cols = dist(gen);

    if (rank == 0) {
        a = getRandomMatrix(rows, cols);
    }

    std::vector <Pixel> answer = solveParallel(a, rows, cols);
    if (rank == 0) {
        std::vector <Pixel> seqAnswer = solveSequential(a, rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                Pixel p = seqAnswer[i * cols + j];
                Pixel q = answer[i * cols + j];
                EXPECT_FALSE(p.r != q.r || p.g != q.g || p.b != q.b);
            }
        }
    }
}

TEST(Gaussian_Image_Filtering_MPI, Checks_Incorrect_Matrix_Size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Pixel> a;
    const int rows = 10;
    const int cols = 5;

    ASSERT_ANY_THROW(std::vector <Pixel> answer = solveParallel(a, rows, cols));
}

TEST(Gaussian_Image_Filtering_MPI, Checks_Empty_Image_Case) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Pixel> a;
    const int rows = 0;
    const int cols = 0;

    ASSERT_ANY_THROW(std::vector <Pixel> answer = solveParallel(a, rows, cols));
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

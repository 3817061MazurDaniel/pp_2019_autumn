// Copyright 2019 Devlikamov Vladislav

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_3/devlikamov_v_linear_filter/linear_filter.h"

TEST(MPI_Linear_filter, test1) {
    int n = 5, m = 5;
    ASSERT_NO_THROW(linear_filter_with_gauss(n, m));
}

TEST(MPI_Linear_filter, test2) {
    int n = 5, m = 2;
    ASSERT_NO_THROW(linear_filter_with_gauss(n, m));
}

TEST(MPI_Linear_filter, test3) {
    int n = 4, m = 7;
    ASSERT_NO_THROW(linear_filter_with_gauss(n, m));
}

TEST(MPI_Linear_filter, test4) {
    int n = 4, m = 4;
    rgb* input = createMatrix(n, m);
    ASSERT_NO_THROW(GaussianBlurs(input, n, m));
}

TEST(MPI_Linear_filter, test5) {
    int n = 10, m = 9;
    rgb* input = createMatrix1(n, m);
    ASSERT_NO_THROW(GaussianBlurs(input, n, m));
}

TEST(MPI_Linear_filter, test6) {
    int n = 10, m = 9;
    rgb* input = createMatrix1(n, m);
    GaussianBlurs* gb = new GaussianBlurs(input, n, m);
    ASSERT_NO_THROW(gb -> process());
}

TEST(MPI_Linear_filter, test7) {
    int n = 3, m = 9;
    rgb* input = createMatrix1(n, m);
    GaussianBlurs* gb = new GaussianBlurs(input, n, m);
    ASSERT_NO_THROW(gb -> process());
}

TEST(MPI_Linear_filter, test8) {
    int n = 5, m = 5;
    rgb* result = linear_filter_with_gauss(n, m);
    int ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (static_cast<int>(result[i*m + j].red) < 0 || static_cast<int>(result[i*m + j].red) > 256) {
                ans = ans + 1;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (static_cast<int>(result[i*m + j].green) < 0 || static_cast<int>(result[i*m + j].green) > 256) {
                ans = ans + 1;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (static_cast<int>(result[i*m + j].blue) < 0 || static_cast<int>(result[i*m + j].blue) > 256) {
                ans = ans + 1;
            }
        }
    }
    EXPECT_EQ(ans, 0);
}

TEST(MPI_Linear_filter, test9) {
    int n = 10, m = 10;
    rgb* result = linear_filter_with_gauss(n, m);
    int ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (static_cast<int>(result[i*m + j].red) < 0 || static_cast<int>(result[i*m + j].red) > 256) {
                ans = ans + 1;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (static_cast<int>(result[i*m + j].green) < 0 || static_cast<int>(result[i*m + j].green) > 256) {
                ans = ans + 1;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (static_cast<int>(result[i*m + j].blue) < 0 || static_cast<int>(result[i*m + j].blue) > 256) {
                ans = ans + 1;
            }
        }
    }
    EXPECT_EQ(ans, 0);
}

TEST(MPI_Linear_filter, test10) {
    int n = 10, m = 5;
    rgb* result = linear_filter_with_gauss(n, m);
    int ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (static_cast<int>(result[i*m + j].red) < 0 || static_cast<int>(result[i*m + j].red) > 256) {
                ans = ans + 1;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (static_cast<int>(result[i*m + j].green) < 0 || static_cast<int>(result[i*m + j].green) > 256) {
                ans = ans + 1;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (static_cast<int>(result[i*m + j].blue) < 0 || static_cast<int>(result[i*m + j].blue) > 256) {
                ans = ans + 1;
            }
        }
    }
    EXPECT_EQ(ans, 0);
}


TEST(MPI_Linear_filter, test11) {
    int n = 5, m = 10;
    rgb* result = linear_filter_with_gauss(n, m);
    int ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (static_cast<int>(result[i*m + j].red) < 0 || static_cast<int>(result[i*m + j].red) > 256) {
                ans = ans + 1;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (static_cast<int>(result[i*m + j].green) < 0 || static_cast<int>(result[i*m + j].green) > 256) {
                ans = ans + 1;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (static_cast<int>(result[i*m + j].blue) < 0 || static_cast<int>(result[i*m + j].blue) > 256) {
                ans = ans + 1;
            }
        }
    }
    EXPECT_EQ(ans, 0);
}

TEST(MPI_Linear_filter, test12) {
    int n = 10, m = 10;
    rgb* result = linear_filter_with_gauss(n, m, 1);
    int ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (static_cast<int>(result[i*m + j].red) < 0 || static_cast<int>(result[i*m + j].red) > 256) {
                ans = ans + 1;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (static_cast<int>(result[i*m + j].green) < 0 || static_cast<int>(result[i*m + j].green) > 256) {
                ans = ans + 1;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (static_cast<int>(result[i*m + j].blue) < 0 || static_cast<int>(result[i*m + j].blue) > 256) {
                ans = ans + 1;
            }
        }
    }
    EXPECT_EQ(ans, 0);
}

TEST(MPI_Linear_filter, test13) {
    int rank;
    int numberOfTasks;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfTasks);
    int n = 3, m = 3;
    rgb *input = new rgb[n*m];
    int k = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            input[k].red = 128;
            input[k].green = 128;
            input[k].blue = 128;
            ++k;
        }
    }
    rgb* result = linear_filter_with_gauss(n, m);
    if (rank == 0) {
        int ans = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (static_cast<int>(result[i*m + j].red) - static_cast<int>(input[i*m + j].red) > 1) {
                    ans = ans + 1;
                }
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (static_cast<int>(result[i*m + j].green) - static_cast<int>(input[i*m + j].green) > 1) {
                    ans = ans + 1;
                }
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (static_cast<int>(result[i*m + j].blue) - static_cast<int>(input[i*m + j].blue) > 1) {
                    ans = ans + 1;
                }
            }
        }
        EXPECT_EQ(ans, 0);
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


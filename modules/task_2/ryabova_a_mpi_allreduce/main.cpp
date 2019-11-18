// Copyright 2019 Ryabova Alyona
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include "./mpi_allreduce.h"

TEST(mpi_allreduce, return_MPI_ERR_COUNT) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int senbuf = 0;
    int recvbuf = 0;

    ASSERT_EQ(MPI_Allreduce_c(&senbuf, &recvbuf, -1, MPI_INT, MPI_SUM, MPI_COMM_WORLD), MPI_ERR_COUNT);
}

TEST(mpi_allreduce, return_MPI_ERR_TYPE) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int senbuf = 0;
    int recvbuf = 0;

    ASSERT_EQ(MPI_Allreduce_c(&senbuf, &recvbuf, 1, MPI_CHAR, MPI_SUM, MPI_COMM_WORLD), MPI_ERR_TYPE);
}

TEST(mpi_allreduce, return_MPI_ERR_OP) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int senbuf = 0;
    int recvbuf = 0;

    ASSERT_EQ(MPI_Allreduce_c(&senbuf, &recvbuf, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD), MPI_ERR_OP);
}


TEST(mpi_allreduce, correctly_calculate_sum_for_int) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int senbuf = 0;
    int recvbuf = 0;
    int sum = 0;

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i)
            senbuf = i;
    }
    MPI_Allreduce_c(&senbuf, &recvbuf, 1, MPI_INT,
        MPI_SUM, MPI_COMM_WORLD);
    for (int i = 0; i < size; i++) {
            sum += i;
    }

    // Assert
    ASSERT_EQ(recvbuf, sum);
}

TEST(mpi_allreduce, correctly_calculate_prod_for_int) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int senbuf = 0;
    int recvbuf = 0;
    int prod = 1;

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i)
            senbuf = i+1;
    }
    MPI_Allreduce_c(&senbuf, &recvbuf, 1, MPI_INT,
        MPI_PROD, MPI_COMM_WORLD);
    for (int i = 1; i <= size; i++) {
        prod *= i;
    }

    // Assert
    ASSERT_EQ(recvbuf, prod);
}

TEST(mpi_allreduce, correctly_calculate_max_for_int) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int senbuf = 0;
    int recvbuf = 0;
    int max = size - 1;

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i)
            senbuf = i;
    }
    MPI_Allreduce_c(&senbuf, &recvbuf, 1, MPI_INT,
        MPI_MAX, MPI_COMM_WORLD);

    // Assert
    ASSERT_EQ(recvbuf, max);
}

TEST(mpi_allreduce, correctly_calculate_min_for_int) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int senbuf = 0;
    int recvbuf = 0;
    int min = 0;

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i)
            senbuf = i;
    }
    MPI_Allreduce_c(&senbuf, &recvbuf, 1, MPI_INT,
        MPI_MIN, MPI_COMM_WORLD);

    // Assert
    ASSERT_EQ(recvbuf, min);
}

TEST(mpi_allreduce, correctly_calculate_sum_for_int_array) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int * senbuf = new int[5];
    int * recvbuf = new int[5];
    int sum = 0;

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i) {
            for (int j = 0; j < 5; j++) {
                senbuf[j] = i + 1;
            }
        }
    }
    MPI_Allreduce_c(senbuf, recvbuf, 5, MPI_INT,
        MPI_SUM, MPI_COMM_WORLD);
    for (int i = 0; i < size; i++)
        sum += i + 1;

    // Assert
    for (int i = 0; i < 5; i++)
        ASSERT_EQ(recvbuf[i], sum);
}

TEST(mpi_allreduce, correctly_calculate_prod_for_int_array) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int* senbuf = new int[5];
    int* recvbuf = new int[5];
    int prod = 1;

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i) {
            for (int j = 0; j < 5; j++) {
                senbuf[j] = i + 1;
            }
        }
    }
    MPI_Allreduce_c(senbuf, recvbuf, 5, MPI_INT,
        MPI_PROD, MPI_COMM_WORLD);
    for (int j = 0; j < size; j++)
        prod *= j + 1;

    // Assert
    for (int i = 0; i < 5; i++)
        ASSERT_EQ(recvbuf[i], prod);
}

TEST(mpi_allreduce, correctly_calculate_max_for_int_array) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int* senbuf = new int[5];
    int* recvbuf = new int[5];
    int max = size;

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i) {
            for (int j = 0; j < 5; j++) {
                senbuf[j] = i + 1;
            }
        }
    }
    MPI_Allreduce_c(senbuf, recvbuf, 5, MPI_INT,
        MPI_MAX, MPI_COMM_WORLD);

    // Assert
    for (int i = 0; i < 5; i++)
        ASSERT_EQ(recvbuf[i], max);
}

TEST(mpi_allreduce, correctly_calculate_min_for_int_array) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int* senbuf = new int[5];
    int* recvbuf = new int[5];
    int min = 1;

    // Act

    for (int i = 0; i < size; i++) {
        if (rank == i) {
            for (int j = 0; j < 5; j++) {
                senbuf[j] = i + 1;
            }
        }
    }
    MPI_Allreduce_c(senbuf, recvbuf, 5, MPI_INT,
        MPI_MIN, MPI_COMM_WORLD);

    // Assert
    for (int i = 0; i < 5; i++)
        ASSERT_EQ(recvbuf[i], min);
}

TEST(mpi_allreduce, correctly_calculate_sum_for_float) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    float senbuf = 0.0f;
    float recvbuf = 0.0f;
    float sum = static_cast<float>(size) * 1.0f;

    // Act
    for (int i = 0; i < size; i++) {
        senbuf = static_cast<float>(1.);
    }
    MPI_Allreduce_c(&senbuf, &recvbuf, 1, MPI_FLOAT,
        MPI_SUM, MPI_COMM_WORLD);

    // Assert
    ASSERT_EQ(recvbuf, sum);
}

TEST(mpi_allreduce, correctly_calculate_prod_for_float) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    float senbuf = 0.0f;
    float recvbuf = 0.0f;

    // Act
    for (int i = 0; i < size; i++) {
        senbuf = static_cast <float>(1.);
    }
    MPI_Allreduce_c(&senbuf, &recvbuf, 1, MPI_FLOAT,
        MPI_PROD, MPI_COMM_WORLD);

    // Assert
    ASSERT_EQ(recvbuf, static_cast <float>(1.));
}

TEST(mpi_allreduce, correctly_calculate_max_for_float) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    float senbuf = 0.0f;
    float recvbuf = 0.0f;
    float max = static_cast <float>(size) - 1.0f;

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i)
            senbuf = static_cast <float>(i) * 1.0f;
    }
    MPI_Allreduce_c(&senbuf, &recvbuf, 1, MPI_FLOAT,
        MPI_MAX, MPI_COMM_WORLD);

    // Assert
    ASSERT_EQ(recvbuf, max);
}

TEST(mpi_allreduce, correctly_calculate_min_for_float) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    float senbuf = 0.0f;
    float recvbuf = 0.0f;
    float min = 0.1f;

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i)
            senbuf = 0.1f + static_cast <float>(i) * 0.1f;
    }
    MPI_Allreduce_c(&senbuf, &recvbuf, 1, MPI_FLOAT,
        MPI_MIN, MPI_COMM_WORLD);

    // Assert
    ASSERT_EQ(recvbuf, min);
}

TEST(mpi_allreduce, correctly_calculate_sum_for_float_array) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    float* senbuf = new float[5];
    float* recvbuf = new float[5];
    float sum = static_cast <float>(size);

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i) {
            for (int j = 0; j < 5; j++) {
                senbuf[j] = 1.0f;
            }
        }
    }
    MPI_Allreduce_c(senbuf, recvbuf, 5, MPI_FLOAT,
        MPI_SUM, MPI_COMM_WORLD);

    // Assert
    for (int i = 0; i < 5; i++)
        ASSERT_EQ(recvbuf[i], sum);
}

TEST(mpi_allreduce, correctly_calculate_prod_for_float_array) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    float* senbuf = new float[5];
    float* recvbuf = new float[5];
    float prod = 2.0f;

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i) {
            for (int j = 0; j < 5; j++) {
                senbuf[j] = 2.0f;
            }
        }
    }
    MPI_Allreduce_c(senbuf, recvbuf, 5, MPI_FLOAT,
        MPI_PROD, MPI_COMM_WORLD);
    for (int j = 0; j < size-1; j++) {
        prod *= 2.0f;
    }

    // Assert
    for (int i = 0; i < 5; i++)
        ASSERT_EQ(recvbuf[i], prod);
}

TEST(mpi_allreduce, correctly_calculate_max_for_float_array) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    float* senbuf = new float[5];
    float* recvbuf = new float[5];
    float max = static_cast <float>(size) - 1.0f;

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i) {
            for (int j = 0; j < 5; j++) {
                senbuf[j] = static_cast <float>(i);
            }
        }
    }
    MPI_Allreduce_c(senbuf, recvbuf, 5, MPI_FLOAT,
        MPI_MAX, MPI_COMM_WORLD);

    // Assert
    for (int i = 0; i < 5; i++)
        ASSERT_EQ(recvbuf[i], max);
}

TEST(mpi_allreduce, correctly_calculate_min_for_float_array) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    float* senbuf = new float[5];
    float* recvbuf = new float[5];
    float min = 1.0f;

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i) {
            for (int j = 0; j < 5; j++) {
                senbuf[j] = static_cast <float>(i) + 1.0f;
            }
        }
    }
    MPI_Allreduce_c(senbuf, recvbuf, 5, MPI_FLOAT,
        MPI_MIN, MPI_COMM_WORLD);

    // Assert
    for (int i = 0; i < 5; i++)
        ASSERT_EQ(recvbuf[i], min);
}

TEST(mpi_allreduce, correctly_calculate_sum_for_double) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double senbuf = 0.0;
    double recvbuf = 0.0;
    double sum = static_cast <double>(size);

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i)
            senbuf = 1.0;
    }
    MPI_Allreduce_c(&senbuf, &recvbuf, 1, MPI_DOUBLE,
        MPI_SUM, MPI_COMM_WORLD);

    // Assert
    ASSERT_EQ(recvbuf, sum);
}

TEST(mpi_allreduce, correctly_calculate_prod_for_double) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double senbuf = 0.0;
    double recvbuf = 0.0;
    double prod = 2.0;

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i)
            senbuf = 2.0;
    }
    MPI_Allreduce_c(&senbuf, &recvbuf, 1, MPI_DOUBLE,
        MPI_PROD, MPI_COMM_WORLD);
    for (int i = 0; i < size-1; i++) {
        prod *= 2.0;
    }

    // Assert
    ASSERT_EQ(recvbuf, prod);
}

TEST(mpi_allreduce, correctly_calculate_max_for_double) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double senbuf = 0.0;
    double recvbuf = 0.0;
    double max = static_cast <double>(size) - 1.0;

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i)
            senbuf = static_cast <double>(i);
    }
    MPI_Allreduce_c(&senbuf, &recvbuf, 1, MPI_DOUBLE,
        MPI_MAX, MPI_COMM_WORLD);

    // Assert
    ASSERT_EQ(recvbuf, max);
}

TEST(mpi_allreduce, correctly_calculate_min_for_double) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double senbuf = 0.0;
    double recvbuf = 0.0;
    double min = 0.1;

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i)
            senbuf = 0.1 + static_cast <double>(i) * 0.1;
    }
    MPI_Allreduce_c(&senbuf, &recvbuf, 1, MPI_DOUBLE,
        MPI_MIN, MPI_COMM_WORLD);

    // Assert
    ASSERT_EQ(recvbuf, min);
}

TEST(mpi_allreduce, correctly_calculate_sum_for_double_array) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double* senbuf = new double[5];
    double* recvbuf = new double[5];
    double sum = static_cast <double>(size);

    // Act

    for (int i = 0; i < size; i++) {
        if (rank == i) {
            for (int j = 0; j < 5; j++) {
                senbuf[j] = 1.0;
            }
        }
    }
    MPI_Allreduce_c(senbuf, recvbuf, 5, MPI_DOUBLE,
        MPI_SUM, MPI_COMM_WORLD);

    // Assert
    for (int i = 0; i < 5; i++)
        ASSERT_EQ(recvbuf[i], sum);
}

TEST(mpi_allreduce, correctly_calculate_prod_for_double_array) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double* senbuf = new double[5];
    double* recvbuf = new double[5];
    double prod = 2.0;

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i) {
            for (int j = 0; j < 5; j++) {
                senbuf[j] = 2.0;
            }
        }
    }
    MPI_Allreduce_c(senbuf, recvbuf, 5, MPI_DOUBLE,
        MPI_PROD, MPI_COMM_WORLD);
    for (int j = 0; j < size-1; j++)
        prod *= 2.0;

    // Assert
    for (int i = 0; i < 5; i++)
        ASSERT_EQ(recvbuf[i], prod);
}

TEST(mpi_allreduce, correctly_calculate_max_for_double_array) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double* senbuf = new double[5];
    double* recvbuf = new double[5];
    double max = static_cast <double>(size) - 1.0;

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i) {
            for (int j = 0; j < 5; j++) {
                senbuf[j] = static_cast <double>(i);
            }
        }
    }
    MPI_Allreduce_c(senbuf, recvbuf, 5, MPI_DOUBLE,
        MPI_MAX, MPI_COMM_WORLD);

    // Assert
    for (int i = 0; i < 5; i++)
        ASSERT_EQ(recvbuf[i], max);
}

TEST(mpi_allreduce, correctly_calculate_min_for_double_array) {
    // Arrange
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double* senbuf = new double[5];
    double* recvbuf = new double[5];
    double min = 1.0;

    // Act
    for (int i = 0; i < size; i++) {
        if (rank == i) {
            for (int j = 0; j < 5; j++) {
                senbuf[j] = static_cast <double>(i) + 1.0;
            }
        }
    }
    MPI_Allreduce_c(senbuf, recvbuf, 5, MPI_DOUBLE,
        MPI_MIN, MPI_COMM_WORLD);

    // Assert
    for (int i = 0; i < 5; i++)
        ASSERT_EQ(recvbuf[i], min);
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

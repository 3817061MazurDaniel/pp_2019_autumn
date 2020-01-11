// Copyright 2019 Astafeva Irina

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>
#include "../../../modules/task_2/astafeva_i_mpi_reduce/mpi_reduce.h"

TEST(MPI_Reduce_User, No_Throw_With_Normal_Parameters_To_Int) {
    int rank, size;
    int bufsize = 10;
    std::vector<int> sendbuf(bufsize);
    std::vector<int> recvbuf(bufsize);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = gen() % 101;
    }

    ASSERT_NO_THROW(MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD));
    ASSERT_NO_THROW(MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD));
    ASSERT_NO_THROW(MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD));
    ASSERT_NO_THROW(MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD));
}

TEST(MPI_Reduce_User, No_Throw_With_Normal_Parameters_To_Double) {
    int rank, size;
    int bufsize = 10;
    std::vector<double> sendbuf(bufsize);
    std::vector<double> recvbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    ASSERT_NO_THROW(MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD));
    ASSERT_NO_THROW(MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD));
    ASSERT_NO_THROW(MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD));
    ASSERT_NO_THROW(MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_DOUBLE, MPI_PROD, 0, MPI_COMM_WORLD));
}

TEST(MPI_Reduce_User, No_Throw_With_Normal_Parameters_To_Float) {
    int rank, size;
    int bufsize = 10;
    std::vector<float> sendbuf(bufsize);
    std::vector<float> recvbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    ASSERT_NO_THROW(MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD));
    ASSERT_NO_THROW(MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD));
    ASSERT_NO_THROW(MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD));
    ASSERT_NO_THROW(MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_FLOAT, MPI_PROD, 0, MPI_COMM_WORLD));
}

TEST(MPI_Reduce_User, Do_Throw_With_Nullptr_Buffer_To_Int) {
    int rank, size;
    int bufsize = 10;
    std::vector<int> sendbuf1(bufsize);
    int * sendbuf2 = nullptr;
    std::vector<int> recvbuf1(bufsize);
    int * recvbuf2 = nullptr;
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf1[i] = gen() % 101;
    }

    ASSERT_EQ(MPI_Reduce_User(&sendbuf1[0], recvbuf2, bufsize, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_BUFFER);
    ASSERT_EQ(MPI_Reduce_User(sendbuf2, &recvbuf1[0], bufsize, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_BUFFER);
}

TEST(MPI_Reduce_User, Do_Throw_With_Nullptr_Buffer_To_Double) {
    int rank, size;
    int bufsize = 10;
    std::vector<double> sendbuf1(bufsize);
    double * sendbuf2 = nullptr;
    std::vector<double> recvbuf1(bufsize);
    double * recvbuf2 = nullptr;
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf1[i] = urd(gen);
    }

    ASSERT_EQ(MPI_Reduce_User(&sendbuf1[0], recvbuf2, bufsize, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_BUFFER);
    ASSERT_EQ(MPI_Reduce_User(sendbuf2, &recvbuf1[0], bufsize, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_BUFFER);
}

TEST(MPI_Reduce_User, Do_Throw_With_Nullptr_Buffer_To_Float) {
    int rank, size;
    int bufsize = 10;
    std::vector<float> sendbuf1(bufsize);
    float * sendbuf2 = nullptr;
    std::vector<float> recvbuf1(bufsize);
    float * recvbuf2 = nullptr;
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf1[i] = urd(gen);
    }

    ASSERT_EQ(MPI_Reduce_User(&sendbuf1[0], recvbuf2, bufsize, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_BUFFER);
    ASSERT_EQ(MPI_Reduce_User(sendbuf2, &recvbuf1[0], bufsize, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_BUFFER);
}

TEST(MPI_Reduce_User, Do_Throw_With_Count_Less_Zero_To_Int) {
    int rank, size;
    int bufsize = 10;
    std::vector<int> sendbuf(bufsize);
    std::vector<int> recvbuf(bufsize);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = gen() % 101;
    }

    ASSERT_EQ(MPI_Reduce_User(&sendbuf[0], &recvbuf[0], -10, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_COUNT);
}

TEST(MPI_Reduce_User, Do_Throw_With_Count_Less_Zero_To_Double) {
    int rank, size;
    int bufsize = 10;
    std::vector<double> sendbuf(bufsize);
    std::vector<double> recvbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    ASSERT_EQ(MPI_Reduce_User(&sendbuf[0], &recvbuf[0], -10, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_COUNT);
}

TEST(MPI_Reduce_User, Do_Throw_With_Count_Less_Zero_To_Float) {
    int rank, size;
    int bufsize = 10;
    std::vector<float> sendbuf(bufsize);
    std::vector<float> recvbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    ASSERT_EQ(MPI_Reduce_User(&sendbuf[0], &recvbuf[0], -10, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_COUNT);
}

TEST(MPI_Reduce_User, Do_Throw_With_Wrong_Type) {
    int rank, size;
    int bufsize = 10;
    std::vector<int> sendbuf(bufsize);
    std::vector<int> recvbuf(bufsize);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = gen() % 101;
    }

    ASSERT_EQ(MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_LONG, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_TYPE);
}

TEST(MPI_Reduce_User, Max_Works_Right_With_Int) {
    int rank, size;
    int bufsize = 10;
    std::vector<int> sendbuf(bufsize);
    std::vector<int> recvbuf(bufsize);
    std::vector<int> checkbuf(bufsize);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = gen() % 101;
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        ASSERT_EQ(recvbuf, checkbuf);
    }
}

TEST(MPI_Reduce_User, Min_Works_Right_With_Int) {
    int rank, size;
    int bufsize = 10;
    std::vector<int> sendbuf(bufsize);
    std::vector<int> recvbuf(bufsize);
    std::vector<int> checkbuf(bufsize);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = gen() % 101;
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        ASSERT_EQ(recvbuf, checkbuf);
    }
}

TEST(MPI_Reduce_User, Sum_Works_Right_With_Int) {
    int rank, size;
    int bufsize = 10;
    std::vector<int> sendbuf(bufsize);
    std::vector<int> recvbuf(bufsize);
    std::vector<int> checkbuf(bufsize);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = gen() % 101;
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_EQ(recvbuf, checkbuf);
    }
}

TEST(MPI_Reduce_User, Prod_Works_Right_With_Int) {
    int rank, size;
    int bufsize = 10;
    std::vector<int> sendbuf(bufsize);
    std::vector<int> recvbuf(bufsize);
    std::vector<int> checkbuf(bufsize);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = gen() % 101;
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        ASSERT_EQ(recvbuf, checkbuf);
    }
}

TEST(MPI_Reduce_User, Int_Max_Works_Right_With_Any_Root) {
    int rank, size;
    int root;
    int bufsize = 10;
    std::vector<int> sendbuf(bufsize);
    std::vector<int> recvbuf(bufsize);
    std::vector<int> checkbuf(bufsize);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size > 2) {
        root = size - 2;
    } else {
        root = 0;
    }

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = gen() % 101;
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_INT, MPI_MAX, root, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_INT, MPI_MAX, root, MPI_COMM_WORLD);
    if (rank == root) {
        ASSERT_EQ(recvbuf, checkbuf);
    }
}

TEST(MPI_Reduce_User, Int_Min_Works_Right_With_Any_Root) {
    int rank, size;
    int root;
    int bufsize = 10;
    std::vector<int> sendbuf(bufsize);
    std::vector<int> recvbuf(bufsize);
    std::vector<int> checkbuf(bufsize);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size > 2) {
        root = size - 2;
    } else {
        root = 0;
    }

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = gen() % 101;
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_INT, MPI_MIN, root, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_INT, MPI_MIN, root, MPI_COMM_WORLD);
    if (rank == root) {
        ASSERT_EQ(recvbuf, checkbuf);
    }
}

TEST(MPI_Reduce_User, Int_Sum_Works_Right_With_Any_Root) {
    int rank, size;
    int root;
    int bufsize = 10;
    std::vector<int> sendbuf(bufsize);
    std::vector<int> recvbuf(bufsize);
    std::vector<int> checkbuf(bufsize);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size > 2) {
        root = size - 2;
    } else {
        root = 0;
    }

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = gen() % 101;
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
    if (rank == root) {
        ASSERT_EQ(recvbuf, checkbuf);
    }
}

TEST(MPI_Reduce_User, Int_Prod_Works_Right_With_Any_Root) {
    int rank, size;
    int root;
    int bufsize = 10;
    std::vector<int> sendbuf(bufsize);
    std::vector<int> recvbuf(bufsize);
    std::vector<int> checkbuf(bufsize);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size > 2) {
        root = size - 2;
    } else {
        root = 0;
    }

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = gen() % 101;
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_INT, MPI_PROD, root, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_INT, MPI_PROD, root, MPI_COMM_WORLD);
    if (rank == root) {
        ASSERT_EQ(recvbuf, checkbuf);
    }
}

TEST(MPI_Reduce_User, Max_Works_Right_With_Float) {
    int rank, size;
    int bufsize = 10;
    std::vector<float> sendbuf(bufsize);
    std::vector<float> recvbuf(bufsize);
    std::vector<float> checkbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < bufsize; i++) {
            ASSERT_FLOAT_EQ(recvbuf[i], checkbuf[i]);
        }
    }
}

TEST(MPI_Reduce_User, Min_Works_Right_With_Float) {
    int rank, size;
    int bufsize = 10;
    std::vector<float> sendbuf(bufsize);
    std::vector<float> recvbuf(bufsize);
    std::vector<float> checkbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < bufsize; i++) {
            ASSERT_FLOAT_EQ(recvbuf[i], checkbuf[i]);
        }
    }
}

TEST(MPI_Reduce_User, Sum_Works_Right_With_Float) {
    int rank, size;
    int bufsize = 10;
    std::vector<float> sendbuf(bufsize);
    std::vector<float> recvbuf(bufsize);
    std::vector<float> checkbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < bufsize; i++) {
            ASSERT_FLOAT_EQ(recvbuf[i], checkbuf[i]);
        }
    }
}

TEST(MPI_Reduce_User, Prod_Works_Right_With_Float) {
    int rank, size;
    int bufsize = 10;
    std::vector<float> sendbuf(bufsize);
    std::vector<float> recvbuf(bufsize);
    std::vector<float> checkbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_FLOAT, MPI_PROD, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_FLOAT, MPI_PROD, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < bufsize; i++) {
            ASSERT_FLOAT_EQ(recvbuf[i], checkbuf[i]);
        }
    }
}

TEST(MPI_Reduce_User, Float_Max_Works_Right_With_Any_Root) {
    int rank, size;
    int root = 0;
    int bufsize = 10;
    std::vector<float> sendbuf(bufsize);
    std::vector<float> recvbuf(bufsize);
    std::vector<float> checkbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size > 2) {
        root = size - 2;
    }

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_FLOAT, MPI_MAX, root, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_FLOAT, MPI_MAX, root, MPI_COMM_WORLD);

    if (rank == root) {
        for (int i = 0; i < bufsize; i++) {
            ASSERT_FLOAT_EQ(recvbuf[i], checkbuf[i]);
        }
    }
}

TEST(MPI_Reduce_User, Float_Min_Works_Right_With_Any_Root) {
    int rank, size;
    int root = 0;
    int bufsize = 10;
    std::vector<float> sendbuf(bufsize);
    std::vector<float> recvbuf(bufsize);
    std::vector<float> checkbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size > 2) {
        root = size - 2;
    }

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_FLOAT, MPI_MIN, root, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_FLOAT, MPI_MIN, root, MPI_COMM_WORLD);

    if (rank == root) {
        for (int i = 0; i < bufsize; i++) {
            ASSERT_FLOAT_EQ(recvbuf[i], checkbuf[i]);
        }
    }
}

TEST(MPI_Reduce_User, Float_Sum_Works_Right_With_Any_Root) {
    int rank, size;
    int root = 0;
    int bufsize = 10;
    std::vector<float> sendbuf(bufsize);
    std::vector<float> recvbuf(bufsize);
    std::vector<float> checkbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size > 2) {
        root = size - 2;
    }

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_FLOAT, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_FLOAT, MPI_SUM, root, MPI_COMM_WORLD);

    if (rank == root) {
        for (int i = 0; i < bufsize; i++) {
            ASSERT_FLOAT_EQ(recvbuf[i], checkbuf[i]);
        }
    }
}

TEST(MPI_Reduce_User, Float_Prod_Works_Right_With_Any_Root) {
    int rank, size;
    int root = 0;
    int bufsize = 10;
    std::vector<float> sendbuf(bufsize);
    std::vector<float> recvbuf(bufsize);
    std::vector<float> checkbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size > 2) {
        root = size - 2;
    }

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_FLOAT, MPI_PROD, root, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_FLOAT, MPI_PROD, root, MPI_COMM_WORLD);

    if (rank == root) {
        for (int i = 0; i < bufsize; i++) {
            ASSERT_FLOAT_EQ(recvbuf[i], checkbuf[i]);
        }
    }
}

TEST(MPI_Reduce_User, Max_Works_Right_With_Double) {
    int rank, size;
    int bufsize = 10;
    std::vector<double> sendbuf(bufsize);
    std::vector<double> recvbuf(bufsize);
    std::vector<double> checkbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < bufsize; i++) {
            ASSERT_DOUBLE_EQ(recvbuf[i], checkbuf[i]);
        }
    }
}

TEST(MPI_Reduce_User, Min_Works_Right_With_Double) {
    int rank, size;
    int bufsize = 10;
    std::vector<double> sendbuf(bufsize);
    std::vector<double> recvbuf(bufsize);
    std::vector<double> checkbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < bufsize; i++) {
            ASSERT_DOUBLE_EQ(recvbuf[i], checkbuf[i]);
        }
    }
}

TEST(MPI_Reduce_User, Sum_Works_Right_With_Double) {
    int rank, size;
    int bufsize = 10;
    std::vector<double> sendbuf(bufsize);
    std::vector<double> recvbuf(bufsize);
    std::vector<double> checkbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < bufsize; i++) {
            ASSERT_DOUBLE_EQ(recvbuf[i], checkbuf[i]);
        }
    }
}

TEST(MPI_Reduce_User, Prod_Works_Right_With_Double) {
    int rank, size;
    int bufsize = 10;
    std::vector<double> sendbuf(bufsize);
    std::vector<double> recvbuf(bufsize);
    std::vector<double> checkbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_DOUBLE, MPI_PROD, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_DOUBLE, MPI_PROD, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < bufsize; i++) {
            ASSERT_DOUBLE_EQ(recvbuf[i], checkbuf[i]);
        }
    }
}

TEST(MPI_Reduce_User, Double_Max_Works_Right_With_Any_Root) {
    int rank, size;
    int root = 0;
    int bufsize = 10;
    std::vector<double> sendbuf(bufsize);
    std::vector<double> recvbuf(bufsize);
    std::vector<double> checkbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size > 2) {
        root = size - 2;
    }

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_DOUBLE, MPI_MAX, root, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_DOUBLE, MPI_MAX, root, MPI_COMM_WORLD);

    if (rank == root) {
        for (int i = 0; i < bufsize; i++) {
            ASSERT_DOUBLE_EQ(recvbuf[i], checkbuf[i]);
        }
    }
}

TEST(MPI_Reduce_User, Double_Min_Works_Right_With_Any_Root) {
    int rank, size;
    int root = 0;
    int bufsize = 10;
    std::vector<double> sendbuf(bufsize);
    std::vector<double> recvbuf(bufsize);
    std::vector<double> checkbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size > 2) {
        root = size - 2;
    }

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_DOUBLE, MPI_MIN, root, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_DOUBLE, MPI_MIN, root, MPI_COMM_WORLD);

    if (rank == root) {
        for (int i = 0; i < bufsize; i++) {
            ASSERT_DOUBLE_EQ(recvbuf[i], checkbuf[i]);
        }
    }
}

TEST(MPI_Reduce_User, Double_Sum_Works_Right_With_Any_Root) {
    int rank, size;
    int root = 0;
    int bufsize = 10;
    std::vector<double> sendbuf(bufsize);
    std::vector<double> recvbuf(bufsize);
    std::vector<double> checkbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size > 2) {
        root = size - 2;
    }

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);

    if (rank == root) {
        for (int i = 0; i < bufsize; i++) {
            ASSERT_DOUBLE_EQ(recvbuf[i], checkbuf[i]);
        }
    }
}

TEST(MPI_Reduce_User, Double_Prod_Works_Right_With_Any_Root) {
    int rank, size;
    int root = 0;
    int bufsize = 10;
    std::vector<double> sendbuf(bufsize);
    std::vector<double> recvbuf(bufsize);
    std::vector<double> checkbuf(bufsize);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size > 2) {
        root = size - 2;
    }

    for (int i = 0; i < bufsize; i++) {
        sendbuf[i] = urd(gen);
    }

    MPI_Reduce_User(&sendbuf[0], &recvbuf[0], bufsize, MPI_DOUBLE, MPI_PROD, root, MPI_COMM_WORLD);
    MPI_Reduce(&sendbuf[0], &checkbuf[0], bufsize, MPI_DOUBLE, MPI_PROD, root, MPI_COMM_WORLD);

    if (rank == root) {
        for (int i = 0; i < bufsize; i++) {
            ASSERT_DOUBLE_EQ(recvbuf[i], checkbuf[i]);
        }
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

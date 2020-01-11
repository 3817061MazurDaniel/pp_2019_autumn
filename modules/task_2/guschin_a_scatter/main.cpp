// Copyright 2019 Guschin Alexander
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <string>
#include <vector>
#include "../../../modules/task_2/guschin_a_scatter/scatter.h"

TEST(scatter, throw_when_negative_root) {
  int root = -1;
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<int> p(1);
  int dest[3];
  EXPECT_EQ(MPI_Scatter_bin(&p[0], 1, MPI_INT, &dest[0], 1, MPI_INT, root,
                            MPI_COMM_WORLD),
            MPI_ERR_COUNT);
}

TEST(scatter, throw_when_different_size) {
  int root = 0;
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<int> p(1);
  int dest[3];
  EXPECT_EQ(MPI_Scatter_bin(&p[0], 1, MPI_INT, &dest[0], 99, MPI_INT, root,
                            MPI_COMM_WORLD),
            MPI_ERR_COUNT);
}

TEST(scatter, can_compare_bin_scatter_with_MPI_Scatter) {
  int mes_size = 3;
  int root = 0;
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<int> p(mes_size * size);
  std::vector<int> d1(mes_size * size);
  std::vector<int> d2(mes_size * size);
  std::vector<int> dest(mes_size);
  if (rank == root) {
    for (int i = 0; i < mes_size * size; ++i) p[i] = i;
  }
  MPI_Scatter_bin(&p[0], mes_size, MPI_INT, &dest[0], mes_size, MPI_INT, root,
                  MPI_COMM_WORLD);
  MPI_Gather(&dest[0], mes_size, MPI_INT, &d1[0], mes_size, MPI_INT, root,
             MPI_COMM_WORLD);

  MPI_Scatter(&p[0], mes_size, MPI_INT, &dest[0], mes_size, MPI_INT, root,
              MPI_COMM_WORLD);
  MPI_Gather(&dest[0], mes_size, MPI_INT, &d2[0], mes_size, MPI_INT, root,
             MPI_COMM_WORLD);

  if (rank == root) {
    EXPECT_EQ(d1, d2);
  }
}

TEST(scatter, can_scatter_and_gather_double) {
  int root = 0;
  int mes_size = 3;
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<double> p(mes_size * size);
  std::vector<double> d(mes_size * size);
  std::vector<double> dest(mes_size);
  if (rank == root) {
    for (int i = 0; i < mes_size * size; ++i) p[i] = i + 1.0 / (i + 1);
  }
  MPI_Scatter_custom(&p[0], mes_size, MPI_DOUBLE, &dest[0], mes_size,
                     MPI_DOUBLE, root, MPI_COMM_WORLD);
  MPI_Gather(&dest[0], mes_size, MPI_DOUBLE, &d[0], mes_size, MPI_DOUBLE, root,
             MPI_COMM_WORLD);

  if (rank == root) {
    EXPECT_EQ(p, d);
  }
}

TEST(scatter, can_scatter_and_gather_int) {
  int mes_size = 3;
  int root = 0;
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<int> p(mes_size * size);
  std::vector<int> d(mes_size * size);
  std::vector<int> dest(mes_size);
  if (rank == root) {
    for (int i = 0; i < mes_size * size; ++i) p[i] = i;
  }
  MPI_Scatter_custom(&p[0], mes_size, MPI_INT, &dest[0], mes_size, MPI_INT,
                     root, MPI_COMM_WORLD);
  MPI_Gather(&dest[0], mes_size, MPI_INT, &d[0], mes_size, MPI_INT, root,
             MPI_COMM_WORLD);

  if (rank == root) {
    EXPECT_EQ(p, d);
  }
}

TEST(scatter, can_scatter_and_gather_float) {
  int mes_size = 3;
  int root = 0;
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<float> p(mes_size * size);
  std::vector<float> d(mes_size * size);
  std::vector<float> dest(mes_size);
  if (rank == root) {
    for (int i = 0; i < mes_size * size; ++i) p[i] = i + 1.0 / (i + 1);
  }
  MPI_Scatter_custom(&p[0], mes_size, MPI_FLOAT, &dest[0], mes_size, MPI_FLOAT,
                     root, MPI_COMM_WORLD);
  MPI_Gather(&dest[0], mes_size, MPI_FLOAT, &d[0], mes_size, MPI_FLOAT, root,
             MPI_COMM_WORLD);

  if (rank == root) {
    EXPECT_EQ(p, d);
  }
}

TEST(scatter, can_scatter_bin_and_gather_int) {
  int mes_size = 3;
  int root = 0;
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<int> p(mes_size * size);
  std::vector<int> d(mes_size * size);
  std::vector<int> dest(mes_size);
  if (rank == root) {
    for (int i = 0; i < mes_size * size; ++i) p[i] = i;
  }
  MPI_Scatter_bin(&p[0], mes_size, MPI_INT, &dest[0], mes_size, MPI_INT, root,
                  MPI_COMM_WORLD);
  MPI_Gather(&dest[0], mes_size, MPI_INT, &d[0], mes_size, MPI_INT, root,
             MPI_COMM_WORLD);

  if (rank == root) {
    EXPECT_EQ(p, d);
  }
}

TEST(scatter, can_scatter_bin_and_gather_double) {
  int mes_size = 3;
  int root = 0;
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<double> p(mes_size * size);
  std::vector<double> d(mes_size * size);
  std::vector<double> dest(mes_size);
  if (rank == root) {
    for (int i = 0; i < mes_size * size; ++i) p[i] = i + 1.0 / (i + 1);
  }
  MPI_Scatter_bin(&p[0], mes_size, MPI_DOUBLE, &dest[0], mes_size, MPI_DOUBLE,
                  root, MPI_COMM_WORLD);
  MPI_Gather(&dest[0], mes_size, MPI_DOUBLE, &d[0], mes_size, MPI_DOUBLE, root,
             MPI_COMM_WORLD);

  if (rank == root) {
    EXPECT_EQ(p, d);
  }
}

TEST(scatter, can_scatter_bin_and_gather_float) {
  int mes_size = 3;
  int root = 0;
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<float> p(mes_size * size);
  std::vector<float> d(mes_size * size);
  std::vector<float> dest(mes_size);
  if (rank == root) {
    for (int i = 0; i < mes_size * size; ++i) p[i] = i + 1.0 / (i + 1);
  }
  MPI_Scatter_bin(&p[0], mes_size, MPI_FLOAT, &dest[0], mes_size, MPI_FLOAT,
                  root, MPI_COMM_WORLD);
  MPI_Gather(&dest[0], mes_size, MPI_FLOAT, &d[0], mes_size, MPI_FLOAT, root,
             MPI_COMM_WORLD);

  if (rank == root) {
    EXPECT_EQ(p, d);
  }
}

TEST(scatter, time) {
  int vec_size = 100;
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<int> p(vec_size);

  if (rank == 0) {
    Get_rand(&p[0], vec_size);
  }
  double time_f1, time_s1, time_f2, time_s2;
  time_f1 = MPI_Wtime();
  int sum1 = Vector_sum_bin(p);
  time_s1 = MPI_Wtime();
  if (rank == 0) std::cout << "bin time " << time_s1 - time_f1 << std::endl;

  time_f2 = MPI_Wtime();
  int sum2 = Vector_sum(p);
  time_s2 = MPI_Wtime();
  if (rank == 0) std::cout << "st time " << time_s2 - time_f2 << std::endl;

  if (rank == 0) {
    EXPECT_EQ(sum1, sum2);
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

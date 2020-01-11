// Copyright 2019 Silenko Dmitrii

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_2/silenko_d_image_smoothing/image_smoothing.h"

TEST(image_smoothing_mpi, test_on_my_matrix) {
  double** mas = new double*[3];
  for (int i = 0; i < 3; i++)
    mas[i] = new double[3];
  mas[0][0] = 0;
  mas[0][1] = 1;
  mas[0][2] = 0;
  mas[1][0] = 1;
  mas[1][1] = 1;
  mas[1][2] = 1;
  mas[2][0] = 0;
  mas[2][1] = 1;
  mas[2][2] = 0;

  double** res;
  double** result = new double*[3];
  for (int i = 0; i < 3; i++)
    result[i] = new double[3];
  result[0][0] = 1;
  result[0][1] = 0.6;
  result[0][2] = 1;
  result[1][0] = 0.6;
  result[1][1] = 0.5;
  result[1][2] = 0.6;
  result[2][0] = 1;
  result[2][1] = 0.6;
  result[2][2] = 1;
  res = ImageSmoothing(mas, 3, 3);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        EXPECT_EQ(res[i][j], result[i][j]);
  }
}

TEST(image_smoothing_mpi, test_on_my_rect_matrix_1) {
  int n = 4;
  int m = 6;
  double** mas = new double*[n];
  for (int i = 0; i < n; i++)
    mas[i] = new double[m];
  mas[0][0] = 0.5;
  mas[0][1] = 1;
  mas[0][2] = 0;
  mas[0][3] = 0;
  mas[0][4] = 1;
  mas[0][5] = 0.5;
  mas[1][0] = 1;
  mas[1][1] = 1;
  mas[1][2] = 0;
  mas[1][3] = 0;
  mas[1][4] = 1;
  mas[1][5] = 1;
  mas[2][0] = 0;
  mas[2][1] = 0;
  mas[2][2] = 0;
  mas[2][3] = 0;
  mas[2][4] = 0;
  mas[2][5] = 0;
  mas[3][0] = 0.5;
  mas[3][1] = 1;
  mas[3][2] = 0;
  mas[3][3] = 0;
  mas[3][4] = 1;
  mas[3][5] = 0.5;

  double** res;
  double** result = new double*[n];
  for (int i = 0; i < n; i++)
    result[i] = new double[m];
  result[0][0] = 1;
  result[0][1] = 0.5;
  result[0][2] = 0.4;
  result[0][3] = 0.4;
  result[0][4] = 0.5;
  result[0][5] = 1;
  result[1][0] = 0.5;
  result[1][1] = 0.3125;
  result[1][2] = 0.25;
  result[1][3] = 0.25;
  result[1][4] = 0.3125;
  result[1][5] = 0.5;
  result[2][0] = 0.7;
  result[2][1] = 0.4375;
  result[2][2] = 0.25;
  result[2][3] = 0.25;
  result[2][4] = 0.4375;
  result[2][5] = 0.7;
  result[3][0] = 0.33;
  result[3][1] = 0.1;
  result[3][2] = 0.2;
  result[3][3] = 0.2;
  result[3][4] = 0.1;
  result[3][5] = 0.33;
  res = ImageSmoothing(mas, n, m);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++)
        EXPECT_NEAR(res[i][j], result[i][j], 0.01);
  }
}

TEST(image_smoothing_mpi, test_on_my_rect_matrix_2) {
  int n = 7;
  int m = 3;
  double** mas = new double*[n];
  for (int i = 0; i < n; i++)
    mas[i] = new double[m];
  mas[0][0] = 0.5;
  mas[0][1] = 1;
  mas[0][2] = 0;
  mas[1][0] = 1;
  mas[1][1] = 1;
  mas[1][2] = 0;
  mas[2][0] = 0;
  mas[2][1] = 0;
  mas[2][2] = 0;
  mas[3][0] = 1;
  mas[3][1] = 0.5;
  mas[3][2] = 1;
  mas[4][0] = 0;
  mas[4][1] = 0;
  mas[4][2] = 0;
  mas[5][0] = 0;
  mas[5][1] = 1;
  mas[5][2] = 1;
  mas[6][0] = 0;
  mas[6][1] = 1;
  mas[6][2] = 0.5;

  double** res;
  double** result = new double*[n];
  for (int i = 0; i < n; i++)
    result[i] = new double[m];
  result[0][0] = 1;
  result[0][1] = 0.5;
  result[0][2] = 0.66;
  result[1][0] = 0.5;
  result[1][1] = 0.3125;
  result[1][2] = 0.4;
  result[2][0] = 0.7;
  result[2][1] = 0.5625;
  result[2][2] = 0.5;
  result[3][0] = 0.1;
  result[3][1] = 0.25;
  result[3][2] = 0.1;
  result[4][0] = 0.5;
  result[4][1] = 0.5625;
  result[4][2] = 0.7;
  result[5][0] = 0.4;
  result[5][1] = 0.3125;
  result[5][2] = 0.5;
  result[6][0] = 0.66;
  result[6][1] = 0.5;
  result[6][2] = 1;
  res = ImageSmoothing(mas, n, m);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++)
        EXPECT_NEAR(res[i][j], result[i][j], 0.01);
  }
}

TEST(image_smoothing_mpi, test_on_my_rand_matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 60;
  int m = 90;
  double** mas = getRandomMas(n, m);
  double** result;

  if (rank == 0) {
    result = new double*[n];
    for (int i = 0; i < n; i++)
      result[i] = new double[m];
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) {
        int count = 0;
        double middle = 0;
        for (int l = -1; l < 2; l++)
          for (int ll = -1; ll < 2; ll++) {
            int tmp1 = j + l;
            if (tmp1 < 0)
              continue;
            if (tmp1 >= m)
              continue;
            int tmp2 = i + ll;
            if (tmp2 < 0)
              continue;
            if (tmp2 >= (n))
              continue;
            if ((tmp1 == j) && (tmp2 == i))
              continue;
            middle += mas[tmp2][tmp1];
            count++;
          }
        result[i][j] = middle / count;
      }
  }

  double** res;
  res = ImageSmoothing(mas, n, m);

  if (rank == 0) {
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++)
        EXPECT_NEAR(res[i][j], result[i][j], 0.01);
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

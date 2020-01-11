// Copyright 2019 Koltyushkina Yanina

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <math.h>
#include <random>
#include "../../../modules/task_3/koltyushkina_ya_convex_hull_jarvis/convex_hull_jarvis.h"

TEST(Convex_Hull_Jarvis_mpi, test_one_point) {
  double** mas = new double*[1];
  double** result = new double*[2];
  double** myres = new double*[2];
  for (int i = 0; i < 1; i++) {
    mas[i] = new double[2];
  }
  for (int i = 0; i < 2; i++) {
    myres[i] = new double[2];
    result[i] = new double[2];
  }
  mas[0][0] = 0;
  mas[0][1] = 0;

  myres[1][0] = 0;
  myres[1][1] = 0;
  result = ConvexHull(mas, 1);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    for (int i = 1; i < 2; i++) {
      for (int j = 0; j < 2; j++)
        EXPECT_EQ(result[i][j], myres[i][j]);
    }
  }
}

TEST(Convex_Hull_Jarvis_mpi, test_triangle_point) {
  double** mas = new double*[4];
  for (int i = 0; i < 4; i++)
    mas[i] = new double[2];
  mas[0][0] = 0;
  mas[0][1] = 0;
  mas[1][0] = 2;
  mas[1][1] = 0;
  mas[2][0] = 2;
  mas[2][1] = 1;
  mas[3][0] = 2;
  mas[3][1] = 2;

  double** result = new double*[5];
  double** myres = new double*[5];
  for (int i = 0; i < 5; i++) {
    myres[i] = new double[2];
    result[i] = new double[2];
  }
  myres[1][0] = 0;
  myres[1][1] = 0;
  myres[2][0] = 2;
  myres[2][1] = 0;
  myres[3][0] = 2;
  myres[3][1] = 2;

  result = ConvexHull(mas, 4);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    int s = static_cast<int>(result[0][0]);
    for (int i = 1; i < s + 1; i++) {
      for (int j = 0; j < 2; j++) {
        EXPECT_EQ(myres[i][j], result[i][j]);
      }
    }
  }
}

TEST(Convex_Hull_Jarvis_mpi, test_triangle_points) {
  double** mas = new double*[6];
  for (int i = 0; i < 6; i++)
    mas[i] = new double[2];
  mas[0][0] = 0;
  mas[0][1] = 0;
  mas[1][0] = 2;
  mas[1][1] = 0;
  mas[2][0] = 1;
  mas[2][1] = 1;
  mas[3][0] = 2;
  mas[3][1] = 2;
  mas[4][0] = 2;
  mas[4][1] = 1;
  mas[5][0] = 2;
  mas[5][1] = 1.75;

  double** result = new double*[7];
  double** myres = new double*[7];
  for (int i = 0; i < 7; i++) {
    myres[i] = new double[2];
    result[i] = new double[2];
  }
  myres[1][0] = 0;
  myres[1][1] = 0;
  myres[2][0] = 2;
  myres[2][1] = 0;
  myres[3][0] = 2;
  myres[3][1] = 2;

  result = ConvexHull(mas, 6);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    int s = static_cast<int>(result[0][0]);
    for (int i = 1; i < s + 1; i++) {
      for (int j = 0; j < 2; j++) {
        EXPECT_EQ(myres[i][j], result[i][j]);
      }
    }
  }
}
//
TEST(Convex_Hull_Jarvis_mpi, test_square_points) {
  double** mas = new double*[7];
  for (int i = 0; i < 7; i++)
    mas[i] = new double[2];
  mas[0][0] = 0;
  mas[0][1] = 0;
  mas[1][0] = 1;
  mas[1][1] = 0;
  mas[2][0] = 1;
  mas[2][1] = 1;
  mas[3][0] = 0;
  mas[3][1] = 1;
  mas[4][0] = 0.25;
  mas[4][1] = 0;
  mas[5][0] = 0;
  mas[5][1] = 0.75;
  mas[6][0] = 0.75;
  mas[6][1] = 1;

  double** result = new double*[8];
  double** myres = new double*[8];
  for (int i = 0; i < 7; i++) {
    myres[i] = new double[2];
    result[i] = new double[2];
  }
  myres[1][0] = 0;
  myres[1][1] = 0;
  myres[2][0] = 1;
  myres[2][1] = 0;
  myres[3][0] = 1;
  myres[3][1] = 1;
  myres[4][0] = 0;
  myres[4][1] = 1;
  result = ConvexHull(mas, 7);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    int s = static_cast<int>(result[0][0]);
    for (int i = 1; i < s + 1; i++) {
      for (int j = 0; j < 2; j++) {
        EXPECT_EQ(myres[i][j], result[i][j]);
      }
    }
  }
}

TEST(Convex_Hull_Jarvis_mpi, test_straight) {
  double** mas = new double*[10];
  for (int i = 0; i < 10; i++)
    mas[i] = new double[2];
  for (int i = -2; i < 8; i++) {
    mas[i + 2][0] = i;
    mas[i + 2][1] = i;
  }


  double** result = new double*[11];
  double** myres = new double*[11];
  for (int i = 0; i < 11; i++) {
    myres[i] = new double[2];
    result[i] = new double[2];
  }
  myres[1][0] = -2;
  myres[1][1] = -2;
  myres[2][0] = 7;
  myres[2][1] = 7;

  result = ConvexHull(mas, 10);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    int s = static_cast<int>(result[0][0]);
    for (int i = 1; i < s + 1; i++) {
      for (int j = 0; j < 2; j++) {
        EXPECT_EQ(myres[i][j], result[i][j]);
      }
    }
  }
}

TEST(Convex_Hull_Jarvis_mpi, test_straight_point) {
  double** mas = new double*[11];
  for (int i = 0; i < 11; i++)
    mas[i] = new double[2];
  for (int i = -2; i < 8; i++) {
    mas[i + 2][0] = i;
    mas[i + 2][1] = i;
  }
  mas[10][0] = 4;
  mas[10][1] = 7;

  double** result = new double*[12];
  double** myres = new double*[12];
  for (int i = 0; i < 12; i++) {
    myres[i] = new double[2];
    result[i] = new double[2];
  }
  myres[1][0] = -2;
  myres[1][1] = -2;
  myres[2][0] = 7;
  myres[2][1] = 7;
  myres[3][0] = 4;
  myres[3][1] = 7;


  result = ConvexHull(mas, 11);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    int s = static_cast<int>(result[0][0]);
    for (int i = 1; i < s + 1; i++) {
      for (int j = 0; j < 2; j++) {
        EXPECT_EQ(myres[i][j], result[i][j]);
      }
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

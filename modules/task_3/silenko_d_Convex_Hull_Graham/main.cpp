// Copyright 2019 Silenko Dmitrii

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <math.h>
#include <stack>
#include "../../../modules/task_3/silenko_d_Convex_Hull_Graham/Convex_Hull_Graham.h"

TEST(Convex_Hull_Graham_mpi, test_on_one_point) {
  double** mas = new double*[1];
  for (int i = 0; i < 1; i++)
    mas[i] = new double[2];
  mas[0][0] = 1;
  mas[0][1] = 1;

  std::stack<double*> res;
  std::stack<double*> result;
  double* tmp = new double[2];
  tmp[0] = 1;
  tmp[1] = 1;
  result.push(tmp);

  res = ConvSort(mas, 1);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    for (int i = 0; i < 1; i++) {
      tmp = res.top();
      double * tmp2 = result.top();
      for (int j = 0; j < 2; j++)
        EXPECT_EQ(tmp[j], tmp2[j]);
      res.pop();
      result.pop();
    }
  }
  delete[] tmp;
}

TEST(Convex_Hull_Graham_mpi, test_on_my_points) {
  double** mas = new double*[10];
  for (int i = 0; i < 10; i++)
    mas[i] = new double[2];
  mas[0][0] = -2;
  mas[0][1] = 1;
  mas[1][0] = -1;
  mas[1][1] = 3;
  mas[2][0] = -1;
  mas[2][1] = -3;
  mas[3][0] = 0;
  mas[3][1] = 0;
  mas[4][0] = 0;
  mas[4][1] = 4;
  mas[5][0] = 2;
  mas[5][1] = -2;
  mas[6][0] = 4;
  mas[6][1] = 4;
  mas[7][0] = 6;
  mas[7][1] = 3;
  mas[8][0] = 6;
  mas[8][1] = -3;
  mas[9][0] = 8;
  mas[9][1] = 1;

  std::stack<double*> res;
  std::stack<double*> result;
  double* tmp = new double[2];
  tmp[0] = -1;
  tmp[1] = 3;
  result.push(tmp);
  double* tmp0 = new double[2];
  tmp0[0] = 0;
  tmp0[1] = 4;
  result.push(tmp0);
  double* tmp9 = new double[2];
  tmp9[0] = 4;
  tmp9[1] = 4;
  result.push(tmp9);
  double* tmp8 = new double[2];
  tmp8[0] = 6;
  tmp8[1] = 3;
  result.push(tmp8);
  double* tmp7 = new double[2];
  tmp7[0] = 8;
  tmp7[1] = 1;
  result.push(tmp7);
  double* tmp6 = new double[2];
  tmp6[0] = 6;
  tmp6[1] = -3;
  result.push(tmp6);
  double* tmp5 = new double[2];
  tmp5[0] = -1;
  tmp5[1] = -3;
  result.push(tmp5);
  double* tmp4 = new double[2];
  tmp4[0] = -2;
  tmp4[1] = 1;
  result.push(tmp4);

  res = ConvSort(mas, 10);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    int s = result.size();
    for (int i = 0; i < s; i++) {
      tmp = res.top();
      double* tmp2 = result.top();
      for (int j = 0; j < 2; j++)
        EXPECT_EQ(tmp[j], tmp2[j]);
      res.pop();
      result.pop();
    }
  }
  delete[] tmp;
  delete[] tmp8;
  delete[] tmp0;
  delete[] tmp9;
  delete[] tmp7;
  delete[] tmp6;
  delete[] tmp5;
  delete[] tmp4;
}

TEST(Convex_Hull_Graham_mpi, test_on_my_points_1) {
  double** mas = new double*[10];
  for (int i = 0; i < 10; i++)
    mas[i] = new double[2];
  mas[0][0] = -3;
  mas[0][1] = 3;
  mas[1][0] = 0;
  mas[1][1] = 3;
  mas[2][0] = 0;
  mas[2][1] = -1;
  mas[3][0] = -3;
  mas[3][1] = 1;
  mas[4][0] = 4;
  mas[4][1] = 3;
  mas[5][0] = 5;
  mas[5][1] = -1;
  mas[6][0] = -3;
  mas[6][1] = -1;
  mas[7][0] = 2;
  mas[7][1] = 3;
  mas[8][0] = 5;
  mas[8][1] = 2;
  mas[9][0] = 2;
  mas[9][1] = -1;

  std::stack<double*> res;
  std::stack<double*> result;
  double* tmp = new double[2];
  tmp[0] = -3;
  tmp[1] = 3;
  result.push(tmp);
  double* tmp0 = new double[2];
  tmp0[0] = 4;
  tmp0[1] = 3;
  result.push(tmp0);
  double* tmp9 = new double[2];
  tmp9[0] = 5;
  tmp9[1] = 2;
  result.push(tmp9);
  double* tmp8 = new double[2];
  tmp8[0] = 5;
  tmp8[1] = -1;
  result.push(tmp8);
  double* tmp7 = new double[2];
  tmp7[0] = -3;
  tmp7[1] = -1;
  result.push(tmp7);

  res = ConvSort(mas, 10);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    int s = result.size();
    for (int i = 0; i < s; i++) {
      tmp = res.top();
      double* tmp2 = result.top();
      for (int j = 0; j < 2; j++)
        EXPECT_EQ(tmp[j], tmp2[j]);
      res.pop();
      result.pop();
    }
  }
  delete[] tmp;
  delete[] tmp8;
  delete[] tmp0;
  delete[] tmp9;
  delete[] tmp7;
}

TEST(Convex_Hull_Graham_mpi, test_on_one_line) {
  double** mas = new double*[12];
  for (int i = 0; i < 12; i++)
    mas[i] = new double[2];
  mas[0][0] = -1;
  mas[0][1] = 0;
  mas[1][0] = 3;
  mas[1][1] = 0;
  mas[2][0] = 5.5;
  mas[2][1] = 0;
  mas[3][0] = -2.5;
  mas[3][1] = 0;
  mas[4][0] = -7;
  mas[4][1] = 0;
  mas[5][0] = 19;
  mas[5][1] = 0;
  mas[6][0] = 1;
  mas[6][1] = 0;
  mas[7][0] = -25;
  mas[7][1] = 0;
  mas[8][0] = 9;
  mas[8][1] = 0;
  mas[9][0] = 25;
  mas[9][1] = 0;
  mas[10][0] = 6;
  mas[10][1] = 0;
  mas[11][0] = 17;
  mas[11][1] = 0;

  std::stack<double*> res;
  std::stack<double*> result;
  double* tmp = new double[2];
  tmp[0] = 25;
  tmp[1] = 0;
  result.push(tmp);
  double* tmp0 = new double[2];
  tmp0[0] = -25;
  tmp0[1] = 0;
  result.push(tmp0);

  res = ConvSort(mas, 12);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    int s = result.size();
    for (int i = 0; i < s; i++) {
      tmp = res.top();
      double * tmp2 = result.top();
      for (int j = 0; j < 2; j++)
        EXPECT_EQ(tmp[j], tmp2[j]);
      res.pop();
      result.pop();
    }
  }
  delete[] tmp;
  delete[] tmp0;
}

TEST(Convex_Hull_Graham_mpi, test_on_my_random_points) {
  int count = 40;
  double** mas = new double*[count];
  for (int i = 0; i < count; i++)
    mas[i] = new double[2];
  mas = getRandomMas(count);
  double** newmas = new double*[count];
  for (int i = 0; i < count; i++) {
    newmas[i] = new double[2];
    for (int j = 0; j < 2; j++)
      newmas[i][j] = mas[i][j];
  }

  std::stack<double*> res;
  std::stack<double*> tmp_result;
  std::stack<double*> result;
  double start_solo = 0;
  double end_solo = 0;
  double* tmp = NULL;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    start_solo = MPI_Wtime();
    double* c = new double[2];
    c[0] = mas[0][0];
    c[1] = mas[0][1];
    int m = 0;

    for (int i = 1; i < count; i++) {
      if (mas[i][0] < c[0]) {
        c[0] = mas[i][0];
        c[1] = mas[i][1];
        m = i;
      } else {
        if (mas[i][0] == c[0]) {
          if (mas[i][1] < c[1]) {
            c[0] = mas[i][0];
            c[1] = mas[i][1];
            m = i;
          }
        }
      }
    }

    tmp = mas[0];
    mas[0] = mas[m];
    mas[m] = tmp;
    m = 0;
    for (int i = 1; i < count; i++)
      for (int j = 0; j < 2; j++)
        mas[i][j] = mas[i][j] - c[j];
    mas[0][0] = 0;
    mas[0][1] = 0;

    double* r = new double[count];
    double* fi = new double[count];
    r[0] = 0;
    fi[0] = 0;
    for (int i = 1; i < count; i++) {
      r[i] = pow((mas[i][0] * mas[i][0]) + (mas[i][1] * mas[i][1]), 0.5);
      fi[i] = atan(mas[i][1] / mas[i][0]);
    }

    QuickSort(mas, 1, count - 1, fi, r);

    tmp_result.push(mas[0]);
    tmp_result.push(mas[1]);
    for (int i = 2; i < count; i++) {
      double* last;
      double* beforelast;
      while (1) {
        last = tmp_result.top();
        tmp_result.pop();
        beforelast = tmp_result.top();
        tmp_result.push(last);

        double d = det(last, beforelast, mas[i], 1);
        if (d > 0) {
          tmp_result.push(mas[i]);
          break;
        } else {
          if (tmp_result.size() >= 3) {
            tmp_result.pop();
          } else {
            tmp_result.pop();
            tmp_result.push(mas[i]);
            break;
          }
        }
      }
    }

    int s = tmp_result.size();
    for (int i = 0; i < s; i++) {
      double* temp = tmp_result.top();
      temp[0] += c[0];
      temp[1] += c[1];
      result.push(temp);
      tmp_result.pop();
    }
    end_solo = MPI_Wtime();
  }

  MPI_Barrier(MPI_COMM_WORLD);

  double start = MPI_Wtime();
  res = ConvSort(newmas, count);
  double end = MPI_Wtime();

  if (rank == 0) {
    std::cout << "Runtime solo = " << end_solo - start_solo << "\n";
    std::cout << "Runtime multi = " << end - start << "\n";
    int s = result.size();
    for (int i = 0; i < s; i++) {
      tmp = res.top();
      double * tmp2 = result.top();
      for (int j = 0; j < 2; j++)
        EXPECT_EQ(tmp[j], tmp2[j]);
      res.pop();
      result.pop();
    }
  }
  delete[] tmp;
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

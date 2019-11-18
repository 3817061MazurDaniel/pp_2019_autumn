// Copyright 2019 Myshkin Andrew
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <random>
#include <ctime>
#include "./broadcast.h"


TEST(Parrallel_Broadcast_MPI, Test_With_Int_n_Random) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int countA = 50;
  int max;
  int maxzero;
  int result;

  int* arrayInt = reinterpret_cast<int*>(malloc(sizeof(int) * countA));

  if (rank == 0) {
    arrayInt = getRandomArrayInt(countA);

    maxzero = getMaxArrayInt(arrayInt, countA);
    max = maxzero - 1;  // fake max of Array

    myBroadcast(arrayInt, countA, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    myBroadcast(arrayInt, countA, MPI_INT, 0, MPI_COMM_WORLD);
    max = getMaxArrayInt(arrayInt, countA);
  }

  MPI_Reduce(&max, &result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

  free(arrayInt);
  if (rank == 0) {
    ASSERT_GE(maxzero, result);
  }
}


TEST(Parrallel_Broadcast_MPI, Test_With_Double_n_Random) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int countA = 50;
  double max;
  double maxzero;
  double result;

  double* arrayDouble = reinterpret_cast<double*>(malloc(sizeof(double) * countA));

  if (rank == 0) {
    arrayDouble = getRandomArrayDouble(countA);

    maxzero = getMaxArrayDouble(arrayDouble, countA);
    max = maxzero - 1;  // fake max of Array

    myBroadcast(arrayDouble, countA, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  } else {
    myBroadcast(arrayDouble, countA, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    max = getMaxArrayDouble(arrayDouble, countA);
  }

  MPI_Reduce(&max, &result, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  free(arrayDouble);
  if (rank == 0) {
    ASSERT_GE(maxzero, result);
  }
}


TEST(Parrallel_Broadcast_MPI, Test_With_Float_n_Random) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int countA = 50;
  float max;
  float maxzero;
  float result;

  float* arrayFloat = reinterpret_cast<float*>(malloc(sizeof(float) * countA));

  if (rank == 0) {
    arrayFloat = getRandomArrayFloat(countA);

    maxzero = getMaxArrayFloat(arrayFloat, countA);
    max = maxzero - 1;  // fake max of Array

    myBroadcast(arrayFloat, countA, MPI_FLOAT, 0, MPI_COMM_WORLD);
  } else {
    myBroadcast(arrayFloat, countA, MPI_FLOAT, 0, MPI_COMM_WORLD);
    max = getMaxArrayFloat(arrayFloat, countA);
  }

  MPI_Reduce(&max, &result, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);

  free(arrayFloat);
  if (rank == 0) {
    ASSERT_GE(maxzero, result);
  }
}


TEST(Parrallel_Broadcast_MPI, Test_With_Size_1_Int) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int buffer;
  if (rank == 0) {
    buffer = 67;
    myBroadcast(&buffer, 1, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    myBroadcast(&buffer, 1, MPI_INT, 0, MPI_COMM_WORLD);
    ASSERT_EQ(buffer, 67);
  }
}


TEST(Parrallel_Broadcast_MPI, Test_With_Size_1_Double) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double buffer;
  if (rank == 0) {
    buffer = 150.5;
    myBroadcast(&buffer, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  } else {
    myBroadcast(&buffer, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    ASSERT_EQ(buffer, 150.5);
  }
}


TEST(Parrallel_Broadcast_MPI, Test_With_Size_1_Float) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  float buffer;
  if (rank == 0) {
    buffer = 82.4;
    myBroadcast(&buffer, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  } else {
    myBroadcast(&buffer, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    ASSERT_EQ(buffer, static_cast<float>(82.4));
  }
}


TEST(Parrallel_Broadcast_MPI, Test_With_Consistent_Array) {
  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_rank(MPI_COMM_WORLD, &size);

  int countA = 70;
  int arrayInt[70];

  if (rank == 0) {
    for (int i = 0; i < countA; i++) {
      arrayInt[i] = i + 5;
    }

    myBroadcast(arrayInt, countA, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    myBroadcast(arrayInt, countA, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < countA; i++) {
      ASSERT_EQ(arrayInt[i], i + 5);
    }
  }
}


TEST(Parrallel_Broadcast_MPI, Test_With_Any_Throw) {
  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_rank(MPI_COMM_WORLD, &size);

  int countA = 0;
  double arrayDouble[3];
  arrayDouble[0] = 3.5;
  arrayDouble[1] = 12.7;
  arrayDouble[2] = 21.3;

  if (rank == 0) {
    ASSERT_ANY_THROW(myBroadcast(arrayDouble, countA, MPI_DOUBLE, 0, MPI_COMM_WORLD));
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

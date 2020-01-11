// Copyright 2019 Khruleva Anastasia

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>
#include <iostream>
#include "../../../modules/task_2/khruleva_reduce/reduce_all.h"


TEST(MPI_Reduce_Gather, check_nok_with_nullptr_buffer_to_int) {
  int ProcRank, ProcNum;
  int NumBuf = 10;
  std::vector<int> sendbuf1(NumBuf);
  int * sendbuf2 = nullptr;
  std::vector<int> recvbuf1(NumBuf);
  int * recvbuf2 = nullptr;
  std::mt19937 random_generator;
  random_generator.seed(static_cast<unsigned int>(time(0)));
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  for (int i = 0; i < NumBuf; i++) {
    sendbuf1[i] = random_generator() % 101;
  }

  ASSERT_EQ(MPI_Reduce_Gather(&sendbuf1[0], recvbuf2, NumBuf, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_BUFFER);
  ASSERT_EQ(MPI_Reduce_Gather(sendbuf2, &recvbuf1[0], NumBuf, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_BUFFER);
}

TEST(MPI_Reduce_Gather, check_ok_normal_work_to_int) {
  int ProcRank, ProcNum;
  int NumBuf = 10;
  std::vector<int> sbuf(NumBuf);
  std::vector<int> rbuf(NumBuf);
  std::mt19937 random_generator;
  random_generator.seed(static_cast<unsigned int>(time(0)));
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  for (int i = 0; i < NumBuf; i++) {
    sbuf[i] = random_generator() % 101;
  }

  ASSERT_NO_THROW(MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD));
  ASSERT_NO_THROW(MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD));
  ASSERT_NO_THROW(MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD));
  ASSERT_NO_THROW(MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD));
}

TEST(MPI_Reduce_Gather, check_nok_if_count_is_negative_to_int) {
  int ProcRank, ProcNum;
  int NumBuf = 10;
  std::vector<int> sbuf(NumBuf);
  std::vector<int> rbuf(NumBuf);
  std::mt19937 random_generator;
  random_generator.seed(static_cast<unsigned int>(time(0)));
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  for (int i = 0; i < NumBuf; i++) {
    sbuf[i] = random_generator() % 101;
  }

  ASSERT_EQ(MPI_Reduce_Gather(&sbuf[0], &rbuf[0], -10, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_COUNT);
}

TEST(MPI_Reduce_Gather, check_can_max_work_with_int) {
  int ProcRank, ProcNum;
  int NumBuf = 10;
  std::vector<int> sbuf(NumBuf);
  std::vector<int> rbuf(NumBuf);
  std::vector<int> buf(NumBuf);
  std::mt19937 random_generator;
  random_generator.seed(static_cast<unsigned int>(time(0)));
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  for (int i = 0; i < NumBuf; i++) {
    sbuf[i] = random_generator() % 101;
  }

  MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&sbuf[0], &buf[0], NumBuf, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  if (ProcRank == 0) {
    ASSERT_EQ(rbuf, buf);
  }
}

TEST(MPI_Reduce_Gather, check_can_min_work_with_int) {
  int ProcRank, ProcNum;
  int NumBuf = 10;
  std::vector<int> sbuf(NumBuf);
  std::vector<int> rbuf(NumBuf);
  std::vector<int> buf(NumBuf);
  std::mt19937 random_generator;
  random_generator.seed(static_cast<unsigned int>(time(0)));
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  for (int i = 0; i < NumBuf; i++) {
    sbuf[i] = random_generator() % 101;
  }

  MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
  MPI_Reduce(&sbuf[0], &buf[0], NumBuf, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
  if (ProcRank == 0) {
    ASSERT_EQ(rbuf, buf);
  }
}

TEST(MPI_Reduce_Gather, check_can_sum_work_with_int) {
  int ProcRank, ProcNum;
  int NumBuf = 10;
  std::vector<int> sbuf(NumBuf);
  std::vector<int> rbuf(NumBuf);
  std::vector<int> buf(NumBuf);
  std::mt19937 random_generator;
  random_generator.seed(static_cast<unsigned int>(time(0)));
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  for (int i = 0; i < NumBuf; i++) {
    sbuf[i] = random_generator() % 101;
  }

  MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Reduce(&sbuf[0], &buf[0], NumBuf, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  if (ProcRank == 0) {
    ASSERT_EQ(rbuf, buf);
  }
}

TEST(MPI_Reduce_Gather, check_can_prod_work_with_int) {
  int ProcRank, ProcNum;
  int NumBuf = 10;
  std::vector<int> sbuf(NumBuf);
  std::vector<int> rbuf(NumBuf);
  std::vector<int> buf(NumBuf);
  std::mt19937 random_generator;
  random_generator.seed(static_cast<unsigned int>(time(0)));
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  for (int i = 0; i < NumBuf; i++) {
    sbuf[i] = random_generator() % 101;
  }

  MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
  MPI_Reduce(&sbuf[0], &buf[0], NumBuf, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
  if (ProcRank == 0) {
    ASSERT_EQ(rbuf, buf);
  }
}

TEST(MPI_Reduce_Gather, check_ok_normal_work_to_double) {
  int ProcRank, ProcNum;
  int NumBuf = 10;
  std::vector<double> sbuf(NumBuf);
  std::vector<double> rbuf(NumBuf);
  std::mt19937 random_generator(time(0));
  std::uniform_real_distribution<> urd(0, 100);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  for (int i = 0; i < NumBuf; i++) {
    sbuf[i] = urd(random_generator);
  }

  ASSERT_NO_THROW(MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD));
  ASSERT_NO_THROW(MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD));
  ASSERT_NO_THROW(MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD));
  ASSERT_NO_THROW(MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_DOUBLE, MPI_PROD, 0, MPI_COMM_WORLD));
}

TEST(MPI_Reduce_Gather, check_nok_with_nullptr_buffer_to_double) {
  int ProcRank, ProcNum;
  int NumBuf = 10;
  std::vector<double> sendbuf1(NumBuf);
  double * sendbuf2 = nullptr;
  std::vector<double> recvbuf1(NumBuf);
  double * recvbuf2 = nullptr;
  std::mt19937 random_generator(time(0));
  std::uniform_real_distribution<> urd(0, 100);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  for (int i = 0; i < NumBuf; i++) {
    sendbuf1[i] = urd(random_generator);
  }

  ASSERT_EQ(MPI_Reduce_Gather(&sendbuf1[0], recvbuf2, NumBuf, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_BUFFER);
  ASSERT_EQ(MPI_Reduce_Gather(sendbuf2, &recvbuf1[0], NumBuf, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_BUFFER);
}

TEST(MPI_Reduce_Gather, check_nok_if_count_is_negative_to_double) {
  int ProcRank, ProcNum;
  int NumBuf = 10;
  std::vector<double> sbuf(NumBuf);
  std::vector<double> rbuf(NumBuf);
  std::mt19937 random_generator(time(0));
  std::uniform_real_distribution<> urd(0, 100);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  for (int i = 0; i < NumBuf; i++) {
    sbuf[i] = urd(random_generator);
  }

  ASSERT_EQ(MPI_Reduce_Gather(&sbuf[0], &rbuf[0], -10, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_COUNT);
}

TEST(MPI_Reduce_Gather, check_ok_normal_work_to_float) {
  int ProcRank, ProcNum;
  int NumBuf = 10;
  std::vector<float> sbuf(NumBuf);
  std::vector<float> rbuf(NumBuf);
  std::mt19937 random_generator(time(0));
  std::uniform_real_distribution<> urd(0, 100);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  for (int i = 0; i < NumBuf; i++) {
    sbuf[i] = urd(random_generator);
  }

  ASSERT_NO_THROW(MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD));
  ASSERT_NO_THROW(MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD));
  ASSERT_NO_THROW(MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD));
  ASSERT_NO_THROW(MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_FLOAT, MPI_PROD, 0, MPI_COMM_WORLD));
}

TEST(MPI_Reduce_Gather, check_nok_with_nullptr_buffer_to_float) {
  int ProcRank, ProcNum;
  int NumBuf = 10;
  std::vector<float> sendbuf1(NumBuf);
  float * sendbuf2 = nullptr;
  std::vector<float> recvbuf1(NumBuf);
  float * recvbuf2 = nullptr;
  std::mt19937 random_generator(time(0));
  std::uniform_real_distribution<> urd(0, 100);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  for (int i = 0; i < NumBuf; i++) {
    sendbuf1[i] = urd(random_generator);
  }

  ASSERT_EQ(MPI_Reduce_Gather(&sendbuf1[0], recvbuf2, NumBuf, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_BUFFER);
  ASSERT_EQ(MPI_Reduce_Gather(sendbuf2, &recvbuf1[0], NumBuf, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_BUFFER);
}

TEST(MPI_Reduce_Gather, check_nok_if_count_is_negative_to_float) {
  int ProcRank, ProcNum;
  int NumBuf = 10;
  std::vector<float> sbuf(NumBuf);
  std::vector<float> rbuf(NumBuf);
  std::mt19937 random_generator(time(0));
  std::uniform_real_distribution<> urd(0, 100);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  for (int i = 0; i < NumBuf; i++) {
    sbuf[i] = urd(random_generator);
  }

  ASSERT_EQ(MPI_Reduce_Gather(&sbuf[0], &rbuf[0], -10, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_COUNT);
}

TEST(MPI_Reduce_Gather, check_ok_if_type_is_wrong) {
  int ProcRank, ProcNum;
  int NumBuf = 10;
  std::vector<int> sbuf(NumBuf);
  std::vector<int> rbuf(NumBuf);
  std::mt19937 random_generator;
  random_generator.seed(static_cast<unsigned int>(time(0)));
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  for (int i = 0; i < NumBuf; i++) {
    sbuf[i] = random_generator() % 101;
  }

  ASSERT_EQ(MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_LONG, MPI_MAX, 0, MPI_COMM_WORLD), MPI_ERR_TYPE);
}

TEST(MPI_Reduce_Gather, check_max_with_all_roots) {
  int ProcRank, ProcNum;
  int root;
  int NumBuf = 10;
  std::vector<int> sbuf(NumBuf);
  std::vector<int> rbuf(NumBuf);
  std::vector<int> buf(NumBuf);
  std::mt19937 random_generator;
  random_generator.seed(static_cast<unsigned int>(time(0)));
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  if (ProcNum > 2) {
    root = ProcNum - 2;
  } else {
    root = 0;
  }

  for (int i = 0; i < NumBuf; i++) {
    sbuf[i] = random_generator() % 101;
  }

  MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_INT, MPI_MAX, root, MPI_COMM_WORLD);
  MPI_Reduce(&sbuf[0], &buf[0], NumBuf, MPI_INT, MPI_MAX, root, MPI_COMM_WORLD);
  if (ProcRank == root) {
    ASSERT_EQ(rbuf, buf);
  }
}

TEST(MPI_Reduce_Gather, check_min_with_all_roots) {
  int ProcRank, ProcNum;
  int root;
  int NumBuf = 10;
  std::vector<int> sbuf(NumBuf);
  std::vector<int> rbuf(NumBuf);
  std::vector<int> buf(NumBuf);
  std::mt19937 random_generator;
  random_generator.seed(static_cast<unsigned int>(time(0)));
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  if (ProcNum > 2) {
    root = ProcNum - 2;
  } else {
    root = 0;
  }

  for (int i = 0; i < NumBuf; i++) {
    sbuf[i] = random_generator() % 101;
  }

  MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_INT, MPI_MIN, root, MPI_COMM_WORLD);
  MPI_Reduce(&sbuf[0], &buf[0], NumBuf, MPI_INT, MPI_MIN, root, MPI_COMM_WORLD);
  if (ProcRank == root) {
    ASSERT_EQ(rbuf, buf);
  }
}

TEST(MPI_Reduce_Gather, check_sum_with_all_roots) {
  int ProcRank, ProcNum;
  int root;
  int NumBuf = 10;
  std::vector<int> sbuf(NumBuf);
  std::vector<int> rbuf(NumBuf);
  std::vector<int> buf(NumBuf);
  std::mt19937 random_generator;
  random_generator.seed(static_cast<unsigned int>(time(0)));
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  if (ProcNum > 2) {
    root = ProcNum - 2;
  } else {
    root = 0;
  }

  for (int i = 0; i < NumBuf; i++) {
    sbuf[i] = random_generator() % 101;
  }

  MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
  MPI_Reduce(&sbuf[0], &buf[0], NumBuf, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
  if (ProcRank == root) {
    ASSERT_EQ(rbuf, buf);
  }
}

TEST(MPI_Reduce_Gather, check_prod_with_all_roots) {
  int ProcRank, ProcNum;
  int root;
  int NumBuf = 10;
  std::vector<int> sbuf(NumBuf);
  std::vector<int> rbuf(NumBuf);
  std::vector<int> buf(NumBuf);
  std::mt19937 random_generator;
  random_generator.seed(static_cast<unsigned int>(time(0)));
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  if (ProcNum > 2) {
    root = ProcNum - 2;
  } else {
    root = 0;
  }

  for (int i = 0; i < NumBuf; i++) {
    sbuf[i] = random_generator() % 101;
  }

  MPI_Reduce_Gather(&sbuf[0], &rbuf[0], NumBuf, MPI_INT, MPI_PROD, root, MPI_COMM_WORLD);
  MPI_Reduce(&sbuf[0], &buf[0], NumBuf, MPI_INT, MPI_PROD, root, MPI_COMM_WORLD);
  if (ProcRank == root) {
    ASSERT_EQ(rbuf, buf);
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

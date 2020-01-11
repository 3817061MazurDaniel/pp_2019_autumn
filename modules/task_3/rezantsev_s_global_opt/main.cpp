// Copyright 2019 Rezantsev Sergey
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "../../../modules/task_3/rezantsev_s_global_opt/global_opt.h"

double f1(double x, double y) { return std::pow(x, 2) + std::pow(y - 1, 2); }
double f2(double x, double y) { return exp(2 * x) * (x + y * y - 2 * y); }
double f3(double x, double y) {
  return x * x * y * y + x * x / 2 + y * y / 2 + x * y + 1;
}
double f4(double x, double y) {
  return std::pow(x, 2) + x * y + std::pow(y, 2) - 4 * std::log(x) -
         10 * std::log(y);
}

TEST(global_opt, test_f1) {
  int rank;
  result r;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double (*fptr)(double, double) = f1;
  GlobalOptimazer glob(-5, 5, -5, 5, fptr);
  glob.globalCalculation();
  if (rank == 0) {
    r = glob.res;
    result res = {std::abs(r.x), std::abs(r.y - 1.0), 0};
    ASSERT_TRUE(res.x <= 0.1);
    ASSERT_TRUE(res.y <= 0.1);
  }
}

TEST(global_opt, test_f1_on_oy) {
  int rank;
  result r;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double (*fptr)(double, double) = f1;
  GlobalOptimazer glob(0, 5, 0, 5, fptr);
  glob.globalCalculation();
  if (rank == 0) {
    r = glob.res;
    result res = {std::abs(r.x), std::abs(r.y - 1.0), 0};
    ASSERT_TRUE(res.x <= 0.1);
    ASSERT_TRUE(res.y <= 0.1);
  }
}

TEST(global_opt, test_f1_on_ox) {
  int rank;
  result r;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double (*fptr)(double, double) = f1;
  GlobalOptimazer glob(-5, 1, 5, 1, fptr);
  glob.globalCalculation();
  if (rank == 0) {
    r = glob.res;
    result res = {std::abs(r.x), std::abs(r.y - 1.0), 0};
    ASSERT_TRUE(res.x <= 0.1);
    ASSERT_TRUE(res.y <= 0.1);
  }
}

TEST(global_opt, test_f2) {
  int rank;
  result r;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double (*fptr)(double, double) = f2;
  GlobalOptimazer glob(-5, 5, -5, 5, fptr);
  glob.globalCalculation();
  if (rank == 0) {
    r = glob.res;
    result res = {std::abs(r.x - 0.5), std::abs(r.y - 1.0), 0};
    ASSERT_TRUE(res.x <= 0.1);
    ASSERT_TRUE(res.y <= 0.1);
  }
}

TEST(global_opt, test_f3) {
  int rank;
  result r;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double (*fptr)(double, double) = f3;
  GlobalOptimazer glob(-5, 5, -5, 5, fptr);
  // double startPar = MPI_Wtime();
  glob.globalCalculation();
  // double endPar = MPI_Wtime();
  if (rank == 0) {
    r = glob.res;
    // GlobalOptimazer g(-5, 5, -5, 5, fptr);
    // double startSeq = MPI_Wtime();
    // g.OrdinaryCalculation();
    // double endSeq = MPI_Wtime();
    // std::cout << "Time seq: " << endSeq - startSeq << std::endl;
    // std::cout << "Time par: " << endPar - startPar << std::endl;
    result res = {std::abs(r.x), std::abs(r.y), 0};
    ASSERT_TRUE(res.x <= 0.1);
    ASSERT_TRUE(res.y <= 0.1);
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

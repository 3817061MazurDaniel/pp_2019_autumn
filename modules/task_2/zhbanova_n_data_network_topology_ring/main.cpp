// Copyright 2019 Zhbanova Nadezhda

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./data_network_topology_ring.h"

TEST(data_network_topology_ring, exceptionIfRankFromLessThanZero) {
  std::vector<int> vec;
  int rankFrom = -1;
  int rankTo = 1;

  ASSERT_THROW(sendVector(rankFrom, rankTo, vec), std::runtime_error);
}

TEST(data_network_topology_ring, exceptionIfRankFromMoreThanNumOfProc) {
  std::vector<int> vec;
  int rankTo = 1;
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int rankFrom = size;
  ASSERT_THROW(sendVector(rankFrom, rankTo, vec), std::runtime_error);
}

TEST(data_network_topology_ring, exceptionIfRankToLessThanZero) {
  std::vector<int> vec;
  int rankFrom = 0;
  int rankTo = -1;

  ASSERT_THROW(sendVector(rankFrom, rankTo, vec), std::runtime_error);
}

TEST(data_network_topology_ring, exceptionIfRankToMoreThanNumOfProc) {
  std::vector<int> vec;
  int rankFrom = 0;
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int rankTo = size;
  ASSERT_THROW(sendVector(rankFrom, rankTo, vec), std::runtime_error);
}

TEST(data_network_topology_ring, exceptionToSendItself) {
  std::vector<int> vec;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rankFrom = rank;
  int rankTo = rank;

  ASSERT_THROW(sendVector(rankFrom, rankTo, vec), std::runtime_error);
}

TEST(data_network_topology_ring, sendFromFirstToLast) {   // all processes are involved
  // prepare data
  std::vector<int> vec;
  int rankFrom = 0;
  int rank, size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size > 1) {
    int rankTo = size - 1;

    if (rank == rankFrom) {
      vec = randomInputVector(VEC_SIZE);
    }

    // execute
    std::vector<int> resultVec = sendVector(rankFrom, rankTo, vec);

    // check data
    resultVec = transferDataToCheck(rank, rankFrom, rankTo, resultVec);
    if (rank == rankFrom) {
      for (unsigned int i = 0; i < resultVec.size(); i++) {
        ASSERT_EQ(resultVec[i], vec[i]);
      }
    }
  }
}

TEST(data_network_topology_ring, sendVia0Proc) {   // all processes are involved
  // prepare data
  std::vector<int> vec;
  int rank, size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size > 1) {
    int rankFrom = size / 2;
    int rankTo = rankFrom - 1;

    if (rank == rankFrom) {
      vec = randomInputVector(VEC_SIZE);
    }

    // execute
    std::vector<int> resultVec = sendVector(rankFrom, rankTo, vec);

    // check data
    resultVec = transferDataToCheck(rank, rankFrom, rankTo, resultVec);
    if (rank == rankFrom) {
      for (unsigned int i = 0; i < resultVec.size(); i++) {
        ASSERT_EQ(resultVec[i], vec[i]);
      }
    }
  }
}

TEST(data_network_topology_ring, sendViaNotAllProcFromLessTo) {
  // prepare data
  std::vector<int> vec;
  int rank, size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size > 1) {
    int rankFrom = 0;
    int rankTo = size / 2;

    if (rank == rankFrom) {
      vec = randomInputVector(VEC_SIZE);
    }

    // execute
    std::vector<int> resultVec = sendVector(rankFrom, rankTo, vec);

    // check data
    resultVec = transferDataToCheck(rank, rankFrom, rankTo, resultVec);
    if (rank == rankFrom) {
      for (unsigned int i = 0; i < resultVec.size(); i++) {
        ASSERT_EQ(resultVec[i], vec[i]);
      }
    }
  }
}

TEST(data_network_topology_ring, sendViaNotAllProcFromMoreTo) {
  // prepare data
  std::vector<int> vec;
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size > 1) {
    int rankFrom = size - 1;
    int rankTo = 0;

    if (rank == rankFrom) {
      vec = randomInputVector(VEC_SIZE);
    }

    // execute
    std::vector<int> resultVec = sendVector(rankFrom, rankTo, vec);

    // check data
    resultVec = transferDataToCheck(rank, rankFrom, rankTo, resultVec);
    if (rank == rankFrom) {
      for (unsigned int i = 0; i < resultVec.size(); i++) {
        ASSERT_EQ(resultVec[i], vec[i]);
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




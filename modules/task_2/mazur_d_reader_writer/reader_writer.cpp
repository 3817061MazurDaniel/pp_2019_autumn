// Copyright 2019 Mazur Daniil
#include <mpi.h>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <random>
#include "../../../modules/task_2/mazur_d_reader_writer/reader_writer.h"

const int readRequest = 1;
const int finishRead = 2;
const int writeRequest = 3;
const int block = 4;
const int done = 5;
const int magic = 6;

int startTask(int writersCount) {
  int rank, size;
  int request = 6;
  // int index = 0;
  // int readersCount = 0;
  // int readyToRecieve = 1;
  int onResponse = 0;
  // int steps = 0;

  MPI_Status mpiStatus;
  // MPI_Request mpiRequest;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Barrier(MPI_COMM_WORLD);
  if (writersCount < size - 2 && writersCount > 0) {
    if (rank == 0) {
      int accsess = 0;
      int readersCount = 0;
      int localMagic = 0;
      int data = 0;
      ++data;
      // std::cout << "\n" << "The server has been started" << std::endl;
      // std::cout << "Writers count = " << writersCount << std::endl;
      while (localMagic != (size - 1)) {
        MPI_Recv(&request, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &mpiStatus);
        // std::cout << "\n" << "Proc" << mpiStatus.MPI_SOURCE << "asked" << std::endl;
        if (request == writeRequest) {
          if (accsess == 0) {
            accsess = 1;
            data = mpiStatus.MPI_SOURCE;
            // std::cout << "\n" << "Proc" << mpiStatus.MPI_SOURCE << "writes" << std::endl;
            MPI_Send(&done, 1, MPI_INT, mpiStatus.MPI_SOURCE, 0, MPI_COMM_WORLD);
            accsess = 0;
          } else {
            MPI_Send(&block, 1, MPI_INT, mpiStatus.MPI_SOURCE, 0, MPI_COMM_WORLD);
            // std::cout << "\n" << "Proc" << mpiStatus.MPI_SOURCE << "rejected. Works readers" << std::endl;
          }
        } else if (request == readRequest) {
          if (accsess == 1) {
            MPI_Send(&block, 1, MPI_INT, mpiStatus.MPI_SOURCE, 0, MPI_COMM_WORLD);
            // std::cout << "\n" << "Proc" << mpiStatus.MPI_SOURCE << "rejected to read. Works writer" << std::endl;
          } else if (accsess == 0) {
            accsess = 2;
            ++readersCount;
            // std::cout << "\n" << "Proc" << mpiStatus.MPI_SOURCE << "read" << std::endl;
            MPI_Send(&done, 1, MPI_INT, mpiStatus.MPI_SOURCE, 0, MPI_COMM_WORLD);
          } else if (accsess == 2) {
            ++readersCount;
            MPI_Send(&done, 1, MPI_INT, mpiStatus.MPI_SOURCE, 0, MPI_COMM_WORLD);
          }
        } else if (request == finishRead) {
          --readersCount;
          // std::cout << "\n" << "Proc" << mpiStatus.MPI_SOURCE << "read" << std::endl;
          if (readersCount == 0)
            accsess = 0;
          MPI_Send(&done, 1, MPI_INT, mpiStatus.MPI_SOURCE, 0, MPI_COMM_WORLD);
        } else if (request == magic) {
          ++localMagic;
          // std::cout << "\n" << "Proc" << mpiStatus.MPI_SOURCE << "finish work" << std::endl;
        }
        // std::cout << "localmagic = " << localMagic << std::endl;
      }
    }
    if (rank > 0 && rank <= writersCount) {
      int procMagic = 0;
      while (procMagic == 0) {
        MPI_Send(&writeRequest, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&onResponse, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &mpiStatus);
        if (onResponse == done) {
          procMagic = 1;
          MPI_Send(&magic, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
      }
    }
    if (rank > writersCount&& rank < size) {
      int procMagic = 0;
      while (procMagic == 0) {
        MPI_Send(&readRequest, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&onResponse, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &mpiStatus);
        if (onResponse == done) {
          MPI_Send(&finishRead, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
          MPI_Recv(&onResponse, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &mpiStatus);
          if (onResponse == done) {
            procMagic = 1;
            MPI_Send(&magic, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
          }
        }
      }
    }
  } else {
    throw - 1;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  return 0;
}

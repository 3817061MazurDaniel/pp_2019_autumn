// Copyright 2019 Mazur Daniil
#include <mpi.h>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <random>
#include "../../../modules/task_2/mazur_d_reader_writer/reader_writer.h"

const int server = 0;
const int smth = 155;
const int readRequest = 1;
const int finishRead = 2;
const int writeRequest = 3;


int startTask(int writersCount) {
  int rank, size;
  int data = 1;
  int request = 6;
  int index = 0;
  int readersCount = 0;
  int readyToRecieve = 1;
  int onResponse = 0;
  int steps = 0;

  MPI_Status mpiStatus;
  MPI_Request mpiRequest;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (writersCount < size - 2 && writersCount > 0) {
    if (rank == 0) {
      std::cout << "\n" << "The server has been started" << std::endl;
      std::cout << "Writers count = " << writersCount << std::endl;

      while (true) {
        if (readyToRecieve == 1) {
          MPI_Irecv(&request, 1, MPI_INT, MPI_ANY_SOURCE, smth, MPI_COMM_WORLD, &mpiRequest);
          readyToRecieve = 0;
        }
        if (!readyToRecieve) {
          MPI_Test(&mpiRequest, &index, &mpiStatus);
          if ((index) && (request == writeRequest)) {
            if (!readersCount) {
              onResponse = 1;
              MPI_Send(&onResponse, 1, MPI_INT, mpiStatus.MPI_SOURCE, server, MPI_COMM_WORLD);
              std::cout << "\n" << "************WORKS WRITER************" << std::endl;
              std::cout << "Process " << mpiStatus.MPI_SOURCE << " is writing" << std::endl;
              MPI_Recv(&data, 1, MPI_INT, MPI_ANY_SOURCE, mpiStatus.MPI_SOURCE, MPI_COMM_WORLD, &mpiStatus);
              std::cout << "data = " << data << std::endl;
              readyToRecieve = 1;
            } else {
                onResponse = 0;
                MPI_Send(&onResponse, 1, MPI_INT, mpiStatus.MPI_SOURCE, server, MPI_COMM_WORLD);
                std::cout << "\n" << "************WORKS READER************" << std::endl;
                std::cout << "Process " << mpiStatus.MPI_SOURCE << " : ACCESS DENIED" << std::endl;
                readyToRecieve = 1;
            }
          }
          if ((index) && (request == readRequest)) {
            std::cout << "\n" << "************************************" << std::endl;
            std::cout << "Process " << mpiStatus.MPI_SOURCE << " reading..." << std::endl;
            readersCount++;
            MPI_Isend(&data, 1, MPI_INT, mpiStatus.MPI_SOURCE, readRequest, MPI_COMM_WORLD, &mpiRequest);
            std::cout << "Server sent data to " << mpiStatus.MPI_SOURCE << ", data = " << data << std::endl;
            std::cout << "Current readers count: " << readersCount << std::endl;
            readyToRecieve = 1;
            ++steps;
          }
          if ((index) && (request == finishRead)) {
            readersCount--;
            std::cout << "\n" << "************************************" << std::endl;
            std::cout << "Process " << mpiStatus.MPI_SOURCE << " finishes reading " << std::endl;
            std::cout << "Current readers count: " << readersCount << std::endl;
            readyToRecieve = 1;
          }
        }
        if ((steps > 0) && (readersCount == 0)) break;
      }
    }

    if (rank > 0 && rank <= writersCount) {
      request = writeRequest;
      data = rank;
      {
        MPI_Send(&request, 1, MPI_INT, server, smth, MPI_COMM_WORLD);
        MPI_Recv(&onResponse, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &mpiStatus);
        if (onResponse) {
          MPI_Send(&data, 1, MPI_INT, server, rank, MPI_COMM_WORLD);
        }
      }
    }

    if (rank > writersCount&& rank < size) {
      {
        request = readRequest;
        MPI_Send(&request, 1, MPI_INT, server, smth, MPI_COMM_WORLD);
        MPI_Recv(&data, 1, MPI_INT, server, readRequest, MPI_COMM_WORLD, &mpiStatus);
        request = finishRead;
        MPI_Send(&request, 1, MPI_INT, server, smth, MPI_COMM_WORLD);
        }
    }
  } else {
    throw -1;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  return 0;
}

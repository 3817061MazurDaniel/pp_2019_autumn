// Copyright 2019 Mazur Daniil
#include "mpi.h"
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
const int request = 4;
const int queueRequest = 5;
using namespace std;


int startTask(int writersCount)
{
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
      cout << "\n" << "The server has been started" << endl;
      cout << "Writers count = " << writersCount << endl;

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
              cout << "\n" << "************WORKS WRITER************" << endl;
              cout << "Process " << mpiStatus.MPI_SOURCE << " is writing" << endl;
              MPI_Recv(&data, 1, MPI_INT, MPI_ANY_SOURCE, mpiStatus.MPI_SOURCE, MPI_COMM_WORLD, &mpiStatus);
              cout << "data = " << data << endl;
              readyToRecieve = 1;
            }
            else {
              onResponse = 0;
              MPI_Send(&onResponse, 1, MPI_INT, mpiStatus.MPI_SOURCE, server, MPI_COMM_WORLD);
              cout << "\n" << "************WORKS READER************" << endl;
              cout << "Process " << mpiStatus.MPI_SOURCE << " : ACCESS DENIED" << endl;
              readyToRecieve = 1;
            }
          }
          if ((index) && (request == readRequest)) {
            cout << "\n" << "************************************" << endl;
            cout << "Process " << mpiStatus.MPI_SOURCE << " reading..." << endl;
            readersCount++;
            MPI_Isend(&data, 1, MPI_INT, mpiStatus.MPI_SOURCE, readRequest, MPI_COMM_WORLD, &mpiRequest);
            cout << "Server sent data to " << mpiStatus.MPI_SOURCE << ", data = " << data << endl;
            cout << "Current readers count: " << readersCount << endl;
            readyToRecieve = 1;
            ++steps;
          }
          if ((index) && (request == finishRead)) {
            readersCount--;
            cout << "\n" << "************************************" << endl;
            cout << "Process " << mpiStatus.MPI_SOURCE << " finishes reading " << endl;
            cout << "Current readers count: " << readersCount << endl;
            readyToRecieve = 1;
          }
        }
        if ((steps > 0) && (readersCount == 0)) break;

      }
    }

    if (rank > 0 && rank <= writersCount) {
      request = writeRequest;
      std::mt19937 gen(time(0));
      std::uniform_int_distribution<> check(0, 100);

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
      std::mt19937 gen(time(0));
      std::uniform_int_distribution<> check(0, 100);
      {
        request = readRequest;
        MPI_Send(&request, 1, MPI_INT, server, smth, MPI_COMM_WORLD);
        MPI_Recv(&data, 1, MPI_INT, server, readRequest, MPI_COMM_WORLD, &mpiStatus);
        request = finishRead;
        MPI_Send(&request, 1, MPI_INT, server, smth, MPI_COMM_WORLD);
        }
    }
  }
  else throw - 1;

  MPI_Barrier(MPI_COMM_WORLD);
  return 0;
}
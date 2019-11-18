// Copyright 2019 Sokolov Andrey
#include <mpi.h>
#include <iostream>
#include <random>
#include <numeric>
#include <algorithm>
#include <list>
#include "../../../modules/task_2/sokolov_a_dining_philosophers/dining_philosophers.h"

#define FORK_REQUEST 1
#define FORK_RESPONSE 2
#define FORK_RELEASE 3

void startPilosopher(int* result, const int &_number, int* _mas, int comm_size, int mode, int size) {
  int *mas   {_mas};
  int in_buffer[1];
  int out_buffer[1];

  MPI_Status status;

  for (int i = 0; i < size; ++i) {
    MPI_Send(out_buffer, 1, MPI_INT, 0, FORK_REQUEST, MPI_COMM_WORLD);
    MPI_Recv(in_buffer, 1, MPI_INT, 0, FORK_RESPONSE, MPI_COMM_WORLD, &status);
    if (mode == -1) {
      *result += mas[i];
    } else if (mode == -2) {
      *result *= mas[i];
    } else {
      *result = mas[i];
    }

    MPI_Send(out_buffer, 1, MPI_INT, 0, FORK_RELEASE, MPI_COMM_WORLD);
  }
}

void startTable(int comm_size, int size) {
  int philosopher   {0};
  int in_buffer[1]  {0};
  int out_buffer[1] {0};

  MPI_Status status;
  std::list<int> queue;

  bool *fork = new bool[comm_size - 1];

  for (int i = 0; i < comm_size - 1; ++i) {
    fork[i] = true;
  }

  for (int i = 0; i < (comm_size - 1) * size * 2; ++i) {
    MPI_Recv(in_buffer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    philosopher = status.MPI_SOURCE;

    if (status.MPI_TAG == FORK_REQUEST) {
      if (fork[philosopher % (comm_size - 1)] == true && fork[philosopher - 1] == true) {
        fork[philosopher % (comm_size - 1)] = false;
        fork[philosopher - 1] = false;
        MPI_Send(out_buffer, 1, MPI_REAL, philosopher, FORK_RESPONSE, MPI_COMM_WORLD);
      } else {
        queue.push_back(philosopher);
      }
    } else if (status.MPI_TAG == FORK_RELEASE) {
      fork[philosopher % (comm_size - 1)] = true;
      fork[philosopher - 1] = true;

      for (size_t i = 0U; i < queue.size(); ++i) {
        philosopher = queue.front();
        if (fork[philosopher % (comm_size - 1)] == true && fork[philosopher - 1] == true) {
          fork[philosopher % (comm_size - 1)] = false;
          fork[philosopher - 1] = false;
          MPI_Send(out_buffer, 1, MPI_INT, philosopher, FORK_RESPONSE, MPI_COMM_WORLD);

          queue.remove(philosopher);
        }
      }
    }
  }
  delete[]fork;
}

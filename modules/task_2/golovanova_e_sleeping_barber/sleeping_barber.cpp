// Copyright 2019 Golovanova Elena
#include <mpi.h>
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include "../../../modules/task_2/golovanova_e_sleeping_barber/sleeping_barber.h"

void CutTime(double visitorTime) {
  double CutTime = MPI_Wtime();
  double vt = visitorTime / 1000;
  while (MPI_Wtime() - CutTime < vt) {
  }
}

void Barber(double visitorTime, int size) {
    std::cout << "Barber works" << std::endl;
    int flag1 = 0;
    while (flag1 != (size - 1)) {
      MPI_Status status;
      MPI_Recv(&visitorTime, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
      std::cout << "I am barber. I am cutting for " << visitorTime << std::endl;
      CutTime(visitorTime);
      flag1++;
      std::cout << "I am end" << std::endl;
    }
}

void Visitor(double visitorTime, int rank) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  visitorTime = rank + gen() % 10;
  MPI_Ssend(&visitorTime, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
  std::cout << "Visitor is  " << rank << std::endl;
}

void SleepingBarber() {
  int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (size < 2) {
      throw "Error";
    } else {
      double visitorTime = 0;
      if (rank == 0) {
        Barber(visitorTime, size);
      } else {
        Visitor(visitorTime, rank);
        }
    }
    std::cout << " The end!";
}

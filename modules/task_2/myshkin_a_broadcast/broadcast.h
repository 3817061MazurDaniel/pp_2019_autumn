// Copyright 2019 Myshkin Andrew
#ifndef MODULES_TASK_2_MYSHKIN_A_BROADCAST_BROADCAST_H_
#define MODULES_TASK_2_MYSHKIN_A_BROADCAST_BROADCAST_H_

#include <mpi.h>

int* getRandomArrayInt(int sizeA);  // for int
double* getRandomArrayDouble(int sizeA);  // for double
float* getRandomArrayFloat(int sizeA);  // for float

int getMaxArrayInt(int* buffer, int sizeA);
double getMaxArrayDouble(double* buffer, int sizeA);
float getMaxArrayFloat(float* buffer, int sizeA);

void myBroadcast(void *buf, int count, MPI_Datatype type, int root, MPI_Comm comm);  // broadcast function


#endif  // MODULES_TASK_2_MYSHKIN_A_BROADCAST_BROADCAST_H_

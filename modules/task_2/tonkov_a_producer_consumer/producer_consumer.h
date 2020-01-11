// Copyright 2019 Tonkov Alexei
#ifndef MODULES_TASK_2_TONKOV_A_PRODUCER_CONSUMER_PRODUCER_CONSUMER_H_
#define MODULES_TASK_2_TONKOV_A_PRODUCER_CONSUMER_PRODUCER_CONSUMER_H_

#include <mpi.h>

int* generateNumbers(int size);
void rootsFromNumbers(int* numbers, double* roots, int size);
void produceConsume(int* numbers, double* results, int size);

#endif  // MODULES_TASK_2_TONKOV_A_PRODUCER_CONSUMER_PRODUCER_CONSUMER_H_

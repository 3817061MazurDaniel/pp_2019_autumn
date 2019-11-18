// Copyright 2019 Sokolov Andrey
#ifndef MODULES_TASK_2_SOKOLOV_A_DINING_PHILOSOPHERS_DINING_PHILOSOPHERS_H_
#define MODULES_TASK_2_SOKOLOV_A_DINING_PHILOSOPHERS_DINING_PHILOSOPHERS_H_

void startTable(int comm_size, int size);

void startPilosopher(int* result, const int &_number, int* _mas, int comm_size, int mode, int size);

#endif  // MODULES_TASK_2_SOKOLOV_A_DINING_PHILOSOPHERS_DINING_PHILOSOPHERS_H_

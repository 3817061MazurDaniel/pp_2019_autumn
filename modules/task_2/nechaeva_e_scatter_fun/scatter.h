// Copyright 2019 Nechaeva Ekaterina
#ifndef MODULES_TASK_2_NECHAEVA_E_SCATTER_FUN_SCATTER_H_
#define MODULES_TASK_2_NECHAEVA_E_SCATTER_FUN_SCATTER_H_

#include <string.h>
#include <iostream>

int Scatter(void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data,
                  int recv_count, MPI_Datatype recv_datatype, int root, MPI_Comm communicator);

#endif  // MODULES_TASK_2_NECHAEVA_E_SCATTER_FUN_SCATTER_H_

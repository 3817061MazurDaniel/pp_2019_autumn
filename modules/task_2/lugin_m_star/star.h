/*
 * Copyright (C) 2019 LOOGIN. All Rights Reserved.
 */
#ifndef MODULES_TASK_2_LUGIN_M_STAR_STAR_H_
#define MODULES_TASK_2_LUGIN_M_STAR_STAR_H_

#include<mpi.h>

MPI_Comm createStarComm(const MPI_Comm oldComm);
bool isStarTopology(const MPI_Comm curComm);

#endif  // MODULES_TASK_2_LUGIN_M_STAR_STAR_H_

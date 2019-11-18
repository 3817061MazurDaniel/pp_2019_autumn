// Copyright 2019 Okmyanskiy Andrey
#include <math.h>
#include <iostream>
#include "../../../modules/task_2/okmyanskiy_a_topology_line/topology_line.h"
int getMessage(int ProcRank, int ProcNum, MPI_Comm MPI_COMM) {
    if (ProcNum == 1) {
        std::cout << "The program must run on more than one process\n";
        return -1;
    } else {
        int message = -1;
        int rank1 = ProcNum / 3;
        int rank2 = ProcNum / 2;
        if (rank1 == 0)
            rank1++;
        if (ProcRank == rank1) {
            MPI_Send(&ProcRank, 1, MPI_INT, 0, 0, MPI_COMM);
        }
        if (ProcRank == rank2) {
            MPI_Send(&ProcRank, 1, MPI_INT, 0, 0, MPI_COMM);
        }
        if (ProcRank == 0) {
            int temp;
            MPI_Status Status;
            MPI_Recv(&temp, 1, MPI_INT, rank1, 0, MPI_COMM, &Status);
            MPI_Recv(&message, 1, MPI_INT, rank2, 0, MPI_COMM, &Status);
            message += temp;
        }
        return message;
    }
}
MPI_Comm getLineComm(MPI_Comm oldComm, int ProcNum) {
    if (ProcNum == 1) {
        return oldComm;
    }
    MPI_Comm LineComm;
    int* index = new int[ProcNum];
    index[0] = 1;
    int count = 1;
    for (int i = 1; i < ProcNum - 1; i++) {
        count += 2;
        index[i] = count;
    }
    index[ProcNum - 1] = ++count;
    int* edges = new int[count];
    edges[0] = 1;
    for (int i = 1, j = 1; i < count - 1; i += 2, j++) {
        edges[i] = j - 1;
        edges[i + 1] = j + 1;
    }
    edges[count - 1] = ProcNum - 2;
    MPI_Graph_create(oldComm, ProcNum, index, edges, 1, &LineComm);
    return LineComm;
}
bool isLineTopology(MPI_Comm LineComm, int ProcRank) {
    if (LineComm == MPI_COMM_WORLD) {
        return false;
    }
    int num;
    MPI_Graph_neighbors_count(LineComm, ProcRank, &num);
    int *neighbors = new int[num];
    MPI_Graph_neighbors(LineComm, ProcRank, num, neighbors);
    for (int i = 0; i < num; i++) {
        int rank1 = 0;
        MPI_Status Status;
        MPI_Sendrecv(&ProcRank, 1, MPI_INT, neighbors[i], 1, &rank1, 1, MPI_INT, neighbors[i], 1, LineComm, &Status);
        if (abs(rank1 - ProcRank) != 1)
            return false;
    }
    return true;
}

// Copyright 2019 Okmyanskiy Andrey
#include <math.h>
#include <vector>
#include <iostream>
#include "../../../modules/task_2/okmyanskiy_a_topology_line/topology_line.h"

std::vector<int> getMessage(int ProcRank, int ProcNum, MPI_Comm MPI_COMM, int sizeVector) {
    if (ProcNum == 1) {
        std::cout << "The program must run on more than one process\n";
        std::vector<int> result(1);
        result[0] = 1;
        return result;
    } else {
        std::vector<int> result(ProcNum*sizeVector);
        MPI_Status Status;
        if (ProcRank == 0) {
            for (int i = 1; i < ProcNum; i++) {
                MPI_Recv(&result[i*sizeVector], sizeVector, MPI_INT, i, 12, MPI_COMM, &Status);
            }
            for (int i = 0; i < sizeVector; i++) {
                result[i] = ProcRank;
            }
        } else {
            std::vector<int> Temp(sizeVector);
            for (unsigned int i = 0; i < Temp.size(); i++) {
                Temp[i] = ProcRank;
            }
            MPI_Send(&Temp[0], sizeVector, MPI_INT, 0, 12, MPI_COMM);
        }
        return result;
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

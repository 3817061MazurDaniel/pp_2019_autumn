// Copyright 2019 Shemetov Philipp
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include "./sum_vector.h"


//Получить рандомный вектор
std::vector<int> getRandomVector(int sizeVector) {
    if (sizeVector < 1) {
        throw "ErrorLength";
    }
    std::mt19937 generation;
    generation.seed(static_cast<unsigned int>(time(NULL)));
    std::vector<int> vec(sizeVector);
    for (int i = 0; i < sizeVector; i++) {
        vec[i] = generation() % 100;
    }
    return vec;
}


//Получить не рандомный вектор для дебага
std::vector<int> getVector(int sizeVector) {
    if (sizeVector < 1) {
        throw "ErrorLength";
    }
    std::vector<int> vec(sizeVector);
    for (int i = 0; i < sizeVector; i++) {
        vec[i] = i + 1;
    }
    return vec;
}

//Получить сумму вектора
int getLocalSum(std::vector<int> _vec) {
    const int sizeTempVector = _vec.size();
    int sum = 0;
    for (int i = 0; i < sizeTempVector; i++) {
        sum += _vec[i];
    }

    return sum;
}

//Высчитываем сумму вектора с параллельными вычислениями
int getParallelSumVector(std::vector<int> _vec, int sizeVector) {
    if (sizeVector < 1) {
        throw "ErrorLength";
    }
    int rankProc, sizeProc;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    const int blockData = sizeVector / sizeProc;
    const int blockDataRemainder = sizeVector % sizeProc;


    if (rankProc == 0) {
        for (int i = 1; i < sizeProc; i++) {
            MPI_Send(&_vec[0] + i * blockData + blockDataRemainder, blockData,
                     MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }


    std::vector<int> localVector(blockData);
    if (rankProc == 0) {
        localVector = std::vector<int>(_vec.begin(),
                                       _vec.begin() + blockData +
                                       blockDataRemainder);
    } else {
        MPI_Status status;
        MPI_Recv(&localVector[0], blockData, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 &status);
    }

    const int sizeLocalVector = localVector.size();
    int globalSum = 0;
    int localSum = 0;
    for (int i = 0; i < sizeLocalVector; i++) {
        localSum += localVector[i];
    }
    MPI_Reduce(&localSum, &globalSum, 1,
               MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return globalSum;
}

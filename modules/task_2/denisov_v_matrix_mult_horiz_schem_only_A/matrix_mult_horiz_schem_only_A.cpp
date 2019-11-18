// Copyright 2019 Denisov Vladislav
#include <mpi.h>
#include <random>
#include <ctime>
#include <vector>
#include <iostream>
#include "../../../modules/task_2/denisov_v_matrix_mult_horiz_schem_only_A/matrix_mult_horiz_schem_only_A.h"

std::vector <int> createRandomMatrix(int sizeSide) {
    if (sizeSide <= 0)
        throw "Error size of matrix";

    std::default_random_engine generator;
    generator.seed(static_cast<unsigned int>(time(0)));

    int sizeVector = sizeSide * sizeSide;
    std::vector <int> matrix(sizeVector);
    for (int i = 0; i < sizeVector; i++)
        matrix[i] = generator() % 10;

    return matrix;
}

std::vector<int> getMatrixMultSeq(std::vector<int> matrixA, std::vector<int> matrixB, int sizeSide) {
    if (sizeSide <= 0)
        throw "Error size of matrix";

    int sizeVector = sizeSide * sizeSide;
    if (matrixA.size() != static_cast<unsigned int>(sizeVector) ||
        matrixB.size() != static_cast<unsigned int>(sizeVector))
        throw "The dimensions of the matrices do not correspond to the parameter passed";

    std::vector<int> matrixResult(sizeVector);

    for (int i = 0; i < sizeSide; i++) {
        for (int j = 0; j < sizeSide; j++) {
            matrixResult[i * sizeSide + j] = 0;
            for (int k = 0; k < sizeSide; k++) {
                matrixResult[i * sizeSide + j] += matrixA[i * sizeSide + k] * matrixB[k * sizeSide + j];
            }
        }
    }

    return matrixResult;
}

std::vector<int> getMatrixMultPar(std::vector<int> matrixA, std::vector<int> matrixB, int sizeSide) {
    if (sizeSide <= 0)
        throw "Error size of matrix";

    int sizeVector = sizeSide * sizeSide;
    if (matrixA.size() != static_cast<unsigned int>(sizeVector) ||
        matrixB.size() != static_cast<unsigned int>(sizeVector))
        throw "The dimensions of the matrices do not correspond to the parameter passed.";

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int delta = sizeSide / size;
    int remainder = sizeSide % size;

    std::vector<int> vectorLocal(delta * sizeSide);
    std::vector<int> resultGlobal;
    if (rank == 0) {
        resultGlobal.resize(sizeVector);
    }

    if (rank == 0) {
        if (remainder != 0) {
            vectorLocal.resize(sizeSide * delta + remainder * sizeSide);
        }
        if (delta != 0) {
            for (int proc = 1; proc < size; proc++) {
                MPI_Send(&matrixA[0] + proc * delta * sizeSide + remainder * sizeSide,
                    delta * sizeSide, MPI_INT, proc, 1, MPI_COMM_WORLD);
            }
        }
    }

    if (rank == 0) {
        if (remainder != 0) {
            for (int i = 0; i < sizeSide * delta + sizeSide * remainder; i++) {
                vectorLocal[i] = matrixA[i];
            }
        } else {
            for (int i = 0; i < sizeSide * delta; i++) {
                vectorLocal[i] = matrixA[i];
            }
        }
    } else {
        MPI_Status status;
        if (delta != 0) {
            MPI_Recv(&vectorLocal[0], delta * sizeSide, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        }
    }

    std::vector<int> resultLocal(sizeSide * delta);

    if (rank == 0) {
        if (remainder != 0) {
            resultLocal.resize(sizeSide * delta + sizeSide * remainder);
        }

        for (int i = 0; i < delta + remainder; i++) {
            for (int j = 0; j < sizeSide; j++) {
                resultLocal[i * sizeSide + j] = 0;
                for (int k = 0; k < sizeSide; k++) {
                    resultLocal[i * sizeSide + j] += vectorLocal[i * sizeSide + k] * matrixB[k * sizeSide + j];
                }
            }
        }
    } else {
        if (delta != 0) {
            for (int i = 0; i < delta; i++) {
                for (int j = 0; j < sizeSide; j++) {
                    resultLocal[i * sizeSide + j] = 0;
                    for (int k = 0; k < sizeSide; k++) {
                        resultLocal[i * sizeSide + j] += vectorLocal[i * sizeSide + k] * matrixB[k * sizeSide + j];
                    }
                }
            }
        }
    }

    if (rank == 0) {
        if (delta != 0) {
            MPI_Status status;
            for (int proc = 1; proc < size; proc++) {
                MPI_Recv(&resultGlobal[0] + proc * delta * sizeSide + remainder * sizeSide,
                    delta * sizeSide, MPI_INT, proc, 2, MPI_COMM_WORLD, &status);
            }
        }
        if (remainder != 0) {
            for (int i = 0; i < sizeSide * (delta + remainder); i++) {
                resultGlobal[i] = resultLocal[i];
            }
        } else {
            for (int i = 0; i < sizeSide * delta; i++) {
                resultGlobal[i] = resultLocal[i];
            }
        }
    } else {
        if (delta != 0) {
            MPI_Send(&resultLocal[0], delta * sizeSide, MPI_INT, 0, 2, MPI_COMM_WORLD);
        }
    }

    return resultGlobal;
}

void printMatrix(std::vector<int> matrix, int sizeSide) {
    std::cout << "\nPrint Matrix:" << std::endl;

    if (sizeSide > 10) {
        std::cout << "The matrix is too large to display on the console." << std::endl << std::endl;
        return;
    }

    for (int i = 0; i < sizeSide; ++i) {
        for (int j = 0; j < sizeSide; ++j) {
            std::cout << matrix[i * sizeSide + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

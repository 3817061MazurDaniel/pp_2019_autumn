// Copyright 2019 Denisov Vladislav
#include <mpi.h>
#include <random>
#include <ctime>
#include <vector>
#include <iostream>
#include "../../../modules/task_3/denisov_v_gradient_method/gradient_method.h"

std::vector<double> createRandomMatrix(int sizeSide) {
    if (sizeSide <= 0)
        throw "Error size of matrix";

    std::default_random_engine generator;
    generator.seed(static_cast<unsigned int>(time(0)));

    int sizeVector = sizeSide * sizeSide;
    std::vector <double> matrix(sizeVector);
    for (int i = 0; i < sizeSide; ++i) {
        for (int j = i; j < sizeSide; ++j) {
            matrix[i * sizeSide + j] = matrix[j * sizeSide + i] = generator() % 10;
        }
    }

    return matrix;
}

std::vector<double> createRandomVector(int size) {
    if (size <= 0)
        throw "Error size of vector";

    std::default_random_engine generator;
    generator.seed(static_cast<unsigned int>(time(0)));

    std::vector <double> vector(size);
    for (int i = 0; i < size; ++i) {
        vector[i] = generator() % 10 + 1;
    }

    return vector;
}

void printMatrix(std::vector<double> matrix, int size) {
    std::cout << "\nPrint Matrix:" << std::endl;

    if (size > 10) {
        std::cout << "The matrix is too large to display on the console." << std::endl << std::endl;
        return;
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            std::cout << matrix[i * size + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void printVector(std::vector<double> vector, int size) {
    std::cout << "\nPrint Vector:" << std::endl;

    if (size > 10) {
        std::cout << "The vector is too large to display on the console." << std::endl << std::endl;
        return;
    }

    for (int i = 0; i < size; ++i) {
        std::cout << vector[i] << std::endl;
    }
}

void printSystem(std::vector<double> matrix, std::vector<double> vector, int size) {
    std::cout << "\nPrint System:" << std::endl;

    if (size > 10) {
        std::cout << "The system is too large to display on the console." << std::endl << std::endl;
        return;
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            std::cout << matrix[i * size + j] << " ";
        }
        std::cout << "| "<< vector[i] << std::endl;
    }
    std::cout << std::endl;
}

double vectorMult(const std::vector<double>& vectorA, const std::vector<double>& vectorB) {
    if (vectorA.size() != vectorB.size())
        throw "The dimensions of vectors are not consistent";

    double res = 0.0;
    for (size_t i = 0; i < vectorA.size(); ++i)
        res += vectorA[i] * vectorB[i];

    return res;
}

std::vector<double> matrixVectorMult(const std::vector<double>& matrix, const std::vector<double>& vector) {
    if (0 != (matrix.size() % vector.size()))
        throw "The dimensions of the matrix and vector are not consistent";

    std::vector<double> res(matrix.size() / vector.size());
    for (size_t i = 0; i < (matrix.size() / vector.size()); ++i) {
        res[i] = 0.0;
        for (size_t j = 0; j < vector.size(); ++j) {
            res[i] += matrix[i * vector.size() + j] * vector[j];
        }
    }

    return res;
}

std::vector<double> getSolveSeq(const std::vector<double>& matrix, const std::vector<double>& vector, int size) {
    if (size <= 0)
        throw "Error size";

    int iters = 0;
    double eps = 0.1, beta = 0.0, alpha = 0.0, check = 0.0;

    std::vector<double> result(size);
    for (int i = 0; i < size; i++) {
        result[i] = 1;
    }

    std::vector<double> Ah = matrixVectorMult(matrix, result);
    std::vector<double> rprev(size), rnext(size);
    for (int i = 0; i < size; i++)
        rprev[i] = vector[i] - Ah[i];  // r^(k) = b - A * result

    std::vector<double> h(rprev);  // h = r^(k)

    do {
        iters++;
        Ah = matrixVectorMult(matrix, h);  // Ah = A * h
        alpha = vectorMult(rprev, rprev) / vectorMult(h, Ah);
        for (int i = 0; i < size; i++) {
            result[i] += alpha * h[i];
            rnext[i] = rprev[i] - alpha * Ah[i];  // r^(k+1) = r^(k)-alpha*Ah
        }
        beta = vectorMult(rnext, rnext) / vectorMult(rprev, rprev);

        // Norma neviazki
        check = sqrt(vectorMult(rnext, rnext));

        for (int j = 0; j < size; j++)
            h[j] = rnext[j] + beta * h[j];  // h^(k+1) = r^(k+1) + beta*h^(k)

        rprev = rnext;
    } while ((check > eps) && (iters <= size));

    return result;
}

std::vector<double> getSolvePar(const std::vector<double>& matrixInput, const std::vector<double>& vectorInput,
    int sizeSide) {
    if (sizeSide <= 0)
        throw "Error size";

    std::vector<double> matrix = matrixInput;
    std::vector<double> vector = vectorInput;

    MPI_Bcast(matrix.data(), sizeSide * sizeSide, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(vector.data(), sizeSide, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int comm_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int delta = sizeSide / comm_size;
    int remainder = sizeSide % comm_size;

    std::vector<double> matrixLocal(delta * sizeSide);
    if (rank == 0) {
        if (remainder != 0) {
            matrixLocal.resize(sizeSide * delta + remainder * sizeSide);
        }
        if (delta != 0) {
            for (int proc = 1; proc < comm_size; proc++) {
                MPI_Send(&matrix[0] + proc * delta * sizeSide + remainder * sizeSide,
                    delta * sizeSide, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD);
            }
        }
    }

    if (rank == 0) {
        if (remainder != 0) {
            for (int i = 0; i < sizeSide * delta + sizeSide * remainder; i++) {
                matrixLocal[i] = matrix[i];
            }
        } else {
            for (int i = 0; i < sizeSide * delta; i++) {
                matrixLocal[i] = matrix[i];
            }
        }
    } else {
        MPI_Status status;
        if (delta != 0) {
            MPI_Recv(&matrixLocal[0], delta * sizeSide, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
        }
    }

    int iters = 0;
    double eps = 0.1, beta = 0.0, alpha = 0.0, check = 0.0;

    std::vector<double> result(sizeSide);
    for (int i = 0; i < sizeSide; i++) {
        result[i] = 1;
    }
    std::vector<double> Ah = matrixVectorMult(matrixLocal, result);


    std::vector<double> rprev(delta), rnext(delta);
    if (rank == 0) {
        if (remainder != 0) {
            rprev.resize(delta + remainder);
            rnext.resize(delta + remainder);
        }
        for (int i = 0; i < delta + remainder; i++)
            rprev[i] = vector[i] - Ah[i];  // r^(k) = b - A * result
    } else {
        for (int i = 0; i < delta; i++)
            rprev[i] = vector[rank * delta + remainder + i] - Ah[i];  // r^(k) = b - A * result
    }

    std::vector<double> h(sizeSide);  // h = r^(k)
    if (rank == 0) {
        if (remainder != 0) {
            for (int i = 0; i < delta + remainder; i++) {
                h[i] = rprev[i];
            }
        } else {
            for (int i = 0; i < delta; i++) {
                h[i] = rprev[i];
            }
        }
        if (delta != 0) {
            MPI_Status status;
            for (int proc = 1; proc < comm_size; proc++) {
                MPI_Recv(&h[0] + proc * delta + remainder,
                    delta, MPI_DOUBLE, proc, 2, MPI_COMM_WORLD, &status);
            }
        }
    } else {
        if (delta != 0) {
            MPI_Send(&rprev[0], delta, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
        }
    }
    MPI_Bcast(h.data(), sizeSide, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    std::vector<double> hblock(delta);
    std::vector<double> hlocal(delta);
    if (rank == 0) {
        if (remainder != 0) {
            hblock.resize(delta + remainder);
        }
    }

    do {
        iters++;
        Ah = matrixVectorMult(matrixLocal, h);  // Ah = A * h

        if (rank == 0) {
            for (int i = 0; i < delta + remainder; i++) {
                hblock[i] = h[i];
            }
        } else {
            for (int i = 0; i < delta; i++) {
                hblock[i] = h[rank * delta + remainder + i];
            }
        }

        double tmp1 = vectorMult(rprev, rprev), rprevScal;
        double tmp2 = vectorMult(hblock, Ah), znam;
        MPI_Allreduce(&tmp1, &rprevScal, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(&tmp2, &znam, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        alpha = rprevScal / znam;
        for (int i = 0; i < sizeSide; i++) {
            result[i] += alpha * h[i];
        }
        if (rank == 0) {
            for (int i = 0; i < delta + remainder; i++) {
                rnext[i] = rprev[i] - alpha * Ah[i];  // r^(k+1) = r^(k)-alpha*Ah
            }
        } else {
            for (int i = 0; i < delta; i++) {
                rnext[i] = rprev[i] - alpha * Ah[i];  // r^(k+1) = r^(k)-alpha*Ah
            }
        }

        double rnextScal;
        tmp1 = vectorMult(rnext, rnext);
        MPI_Allreduce(&tmp1, &rnextScal, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        beta = rnextScal / rprevScal;

        // Norma neviazki
        check = sqrt(rnextScal);

        if (rank == 0) {
            for (int i = 0; i < delta + remainder; i++) {
                h[i] = rnext[i] + beta * h[i];  // r^(k+1) = r^(k)-alpha*Ah
            }

            if (delta != 0) {
                MPI_Status status;
                for (int proc = 1; proc < comm_size; proc++) {
                    MPI_Recv(&h[0] + proc * delta + remainder,
                        delta, MPI_DOUBLE, proc, 3, MPI_COMM_WORLD, &status);
                }
            }
        } else {
            for (int i = 0; i < delta; i++) {
                hlocal[i] = rnext[i] + beta * h[rank * delta + remainder + i];  // r^(k+1) = r^(k)-alpha*Ah
            }

            if (delta != 0) {
                MPI_Send(&hlocal[0], delta, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
            }
        }
        MPI_Bcast(h.data(), sizeSide, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        rprev = rnext;
    } while ((check > eps) && (iters <= sizeSide));

    return result;
}

// Copyright 2019 Savosina
#include <mpi.h>
#include <random>
#include <ctime>
#include <vector>
#include <iostream>
#include "../../../modules/task_2/savosina_a_seidel_method/seidel_method.h"

std::vector<double> getRandomVector(int n) {
    if (n < 0)
        throw 0;
    if (n > (INT32_MAX/100)/n)
        throw 1;
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<double> matrix(n);
    for (int i = 0; i < n; i++)
        matrix[i] = gen() % 9 + 1;
    return matrix;
}

void PrintVector(std::vector<double> a, int n) {
    for (size_t i = 1; i < a.size() + 1; i++) {
        std::cout << "[" << i << "]: " << a[i - 1] << "  ";
        if (i % n == 0) {
            std::cout << std::endl;
        }
    }
}

std::vector<double> Transparent(std::vector<double> a) {
    std::vector<double> tmp(a.size());
    int n = sqrt(a.size());  // one side of matrix 'a'

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            tmp[n * i + j] = a[i + n * j];

    return tmp;
}

double Norma(const std::vector<double> x) {
    double norm = 0;
    for (auto i = x.cbegin(); i != x.cend(); ++i)
        norm += *i * *i;

    return sqrt(norm);
}

std::vector<double> getSequentialOperations(std::vector<double> a,
                                                std::vector<double> b, int n) {
  std::vector<double> y(n);
  std::vector<double> tmp(n);
  std::vector<double> x(n);
    for (int i = 0; i < n; i++)
        x[i] = 0;
  double sum1;
  double sum2;
  int count = 100;

  for (int c = 0; c < count; c++) {
    for (int i = 0; i < n; ++i)
        y[i] = x[i];

    for (int i = 0; i < n; ++i) {
        sum1 = 0;
        sum2 = 0;
        for (int j = 0; j < i; ++j)
            sum1 += a[i * n + j] * x[j];
        for (int j = i + 1; j < n; ++j)
            sum2 += a[i * n + j] * y[j];

        x[i] = (b[i] - sum1 - sum2) / a[i * n + i];
    }

    for (int i = 0; i < n; ++i)
        tmp[i] = x[i] - y[i];
    // std::cout << "---iteration " << c << " ---" << std::endl;
    // PrintVector(x, n);

    if (Norma(x), Norma(y), 0.001)
        break;
  }
  return x;
}

std::vector<double> getParallelOperations(std::vector<double> a,
                                            std::vector<double> b, int n) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> tmp(n);
    std::vector<double> y(n);
    for (int i = 0; i < n; i++)
        y[i] = 0;
    std::vector<double> x(n);
    for (int i = 0; i < n; i++)
        x[i] = 0;
    std::vector<double> mainPart(1);
    int mainLen = n / size;
    std::vector<double> localPart(1);
    int lastLen = mainLen + n % size;
    MPI_Status status;

    int modulo = n % size;
    int blocksSize = n * mainLen;

    if (rank == size - 1) {
        for (int i = 1; i < lastLen; i++)
            localPart.push_back(0);
        for (int i = 1; i < lastLen * n; i++)
            mainPart.push_back(0);
    } else {
        for (int i = 1; i < mainLen; i++)
            localPart.push_back(0);
        for (int i = 1; i < blocksSize; i++)
            mainPart.push_back(0);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(&b[0], mainLen, MPI_DOUBLE, &localPart[0], mainLen,
                            MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        // send info:
        if (modulo != 0) {  // with modulo:
            for (int i = 1; i < size; i++)
                for (int j = 0; j < mainLen; j++)
                    MPI_Send(&a[((size_t)i * mainLen + j) * n + 0], n,
                                    MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            // + in the last process:
            MPI_Send(&b[(size_t)size * mainLen], modulo,
                            MPI_DOUBLE, size - 1, 0, MPI_COMM_WORLD);
            for (int i = 0; i < modulo; ++i)
                MPI_Send(&a[((size_t)size * mainLen + i) * n + 0], n,
                                MPI_DOUBLE, size - 1, 0, MPI_COMM_WORLD);
        } else {  // without modulo:
            for (int i = 1; i < size; ++i)
                for (int j = 0; j < mainLen; ++j)
                    MPI_Send(&a[((size_t)i * mainLen + j) * n + 0], n,
                                    MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
        // for last process:
    } else if (rank == size - 1) {
        if (modulo != 0) {  // with modulo:
            for (int i = 0; i < mainLen; i++)
                MPI_Recv(&mainPart[(size_t)n * i], n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

            MPI_Recv(&localPart[(size_t)mainLen], modulo,
                                MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
            for (int i = 0; i < modulo; i++)
                MPI_Recv(&mainPart[(size_t)((size_t)mainLen + i) * n], n,
                                    MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

        } else {  // without modulo:
            for (int i = 0; i < mainLen; i++)
                MPI_Recv(&mainPart[(size_t)n * i], n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        }
    } else {  // for all other process:
        for (int i = 0; i < mainLen; i++)
            MPI_Recv(&mainPart[(size_t)n * i], n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }

    int dElem;
    double sum1 = 0;
    double sum2 = 0;
    // Seidel method realisation:
    bool flag = true;
    while (flag) {
        if (rank == 0) {
            for (int i = 0; i < mainLen; i++) {
                for (int j = 0; j < i; j++)
                    sum1 += x[j] * a[i * n + j];
                for (int j = i + 1; j < n; j++)
                    sum2 += y[j] * a[i * n + j];

                x[i] = (localPart[i] - sum1 - sum2) / a[i * n + i];

                for (int j = 1; j < size; j++)
                    MPI_Send(&x[i], 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD);
                sum1 = 0;
                sum2 = 0;
            }
        } else if (rank == size - 1) {
            for (int i = 0; i < rank; i++)
                for (int j = 0; j < mainLen; j++)
                    MPI_Recv(&x[(size_t)i * mainLen + j], 1,
                                    MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);

            for (int i = 0; i < lastLen; i++) {
                dElem = mainLen * rank + i;
                for (int j = dElem + 1; j < n; j++)
                    sum2 += mainPart[(size_t)i * n + j] * y[j];

                for (int j = 0; j < dElem; j++)
                    sum1 += mainPart[(size_t)i * n + j] * x[j];

                x[dElem] = (localPart[i] - sum2 - sum1) / mainPart[(size_t)i * n + dElem];
                sum1 = 0;
                sum2 = 0;
            }
        } else {
            for (int i = 0; i < rank; i++) {
                for (int j = 0; j < mainLen; j++)
                    MPI_Recv(&x[(size_t)i * mainLen + j], 1,
                                    MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
            }
            for (int i = 0; i < mainLen; i++) {
                dElem = mainLen * rank + i;
                for (int j = dElem + 1; j < n; j++)
                    sum2 += mainPart[(size_t)i * n + j] * y[j];

                for (int j = 0; j < dElem; j++)
                    sum1 += mainPart[(size_t)i * n + j] * x[j];

                x[dElem] = (localPart[i] - sum2 - sum1) / mainPart[(size_t)i * n + dElem];
                sum1 = 0;
                sum2 = 0;
                for (int j = rank + 1; j < size; j++)
                    MPI_Send(&x[dElem], 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD);
            }
        }

        if (rank == size - 1)
            MPI_Send(&x[0], n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            MPI_Recv(&x[0], n, MPI_DOUBLE, size - 1, 0, MPI_COMM_WORLD, &status);
            for (int i = 0; i < n; i++)
                tmp[i] = x[i] - y[i];
            // check for result:
           if (Norma(x), Norma(y), 0.001)
                flag = false;
        }
        for (int i = 0; i < n; i++)
            y[i] = x[i];

        MPI_Bcast(&y[0], n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&flag, 1, MPI_C_BOOL, 0, MPI_COMM_WORLD);
    }
    return x;
}

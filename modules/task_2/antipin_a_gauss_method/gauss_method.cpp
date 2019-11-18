// Copyright 2019 Antipin Alexander

#include "../../../modules/task_2/antipin_a_gauss_method/gauss_method.h"
#include <cmath>
#include <random>
#include <utility>
#include <vector>

Matrix::Matrix() {
    mem = std::vector<double>(0);
    n = 0;
}

Matrix::Matrix(const std::vector<double>& vec) {
    mem = vec;
    n = static_cast<int>(sqrt(vec.size()));
}

Matrix::Matrix(const std::vector<std::vector<double>>& mat) {
    int k = 0;
    mem.resize(pow(mat.size(), 2));
    for (size_t i = 0; i < mat.size(); ++i) {
        for (size_t j = 0; j < mat[i].size(); ++j) {
            mem[k] = mat[i][j];
            k++;
        }
    }
    n = mat.size();
}

Matrix::Matrix(const int n) {
    this->n = n;
    mem.resize(n * n);
}

Matrix::Matrix(const Matrix& mat) {
    mem = mat.mem;
    n = mat.n;
}

Matrix::~Matrix() {
}

int Matrix::getMatrixSize() {
    return n;
}

std::vector<double>* Matrix::getMemOfMatrix() {
    return &mem;
}

double & Matrix::getElem(const int row, const int col) {
    return mem[row * n + col];
}

std::vector<double> Matrix::getSequentialSolution(const std::vector<double>& coefVec) {
    std::vector<double> result(n);
    std::vector<double> additionalMat(mem);
    std::vector<double> copyCoefVec(coefVec);
    for (int k = 0; k < n - 1; ++k) {
        double leaderElem = additionalMat[k*n + k];
        if (leaderElem == 0.0) {
            for (int j = k + 1; j < n; ++j) {
                if (additionalMat[j*n + k] != 0.0) {
                    std::swap(copyCoefVec[k], copyCoefVec[j]);
                    for (int i = 0; i < n; ++i) {
                        std::swap(additionalMat[j*n + i], additionalMat[k*n + i]);
                    }
                    leaderElem = additionalMat[k*n + k];
                    break;
                }
                if (j == n - 1) {
                    throw(1);
                }
            }
        }
        bool moreElem = leaderElem >= 0.0 ? true : false;
        for (int rows = k + 1; rows < n; ++rows) {
            double leaderRow = additionalMat[rows*n + k];
            bool moreRow = leaderRow >= 0.0 ? true : false;
            if ((moreElem && moreRow) || (!moreElem && !moreRow)) {
                for (int cols = k; cols < n; ++cols) {
                    additionalMat[rows*n + cols] = isZero(additionalMat[rows*n + cols] -
                        additionalMat[k*n + cols] * myAbs(leaderRow / leaderElem));
                }
                copyCoefVec[rows] -= copyCoefVec[k] * myAbs(leaderRow / leaderElem);
            } else {
                for (int cols = k; cols < n; ++cols) {
                    additionalMat[rows*n + cols] = isZero(additionalMat[rows*n + cols] +
                        additionalMat[k*n + cols] * myAbs(leaderRow / leaderElem));
                }
                copyCoefVec[rows] = isZero(copyCoefVec[rows] +
                    copyCoefVec[k] * myAbs(leaderRow / leaderElem));
            }
        }
    }
    for (int i = n - 1; i >= 0; --i) {
        double res = 0.0;
        for (int j = n - 1; j >= i + 1; --j) {
            res = isZero(res + additionalMat[i*n + j] * result[j]);
        }
        res = isZero((copyCoefVec[i] - res) / additionalMat[i*n + i]);
        result[i] = res;
    }
    return result;
}

std::vector<double> Matrix::getParallelSolution(const std::vector<double>& coefVec) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (coefVec.size() != static_cast<unsigned int>(n)) {
        throw(1);
    }
    if (size > n) {
        std::vector<double> res;
        if (rank == 0) {
            res = getSequentialSolution(coefVec);
        }
        return res;
    }
    std::vector<double> copyCoefVec(coefVec);
    double leaderElem = 0;
    int colsCount = n / size;
    (n % size) / (rank + 1) < 1 ? colsCount: ++colsCount;
    int additionalMatSize = colsCount * n;
    std::vector<double> additionalMat(additionalMatSize);
    size_t k = 0;
    for (int i = rank; i < n; i += size) {
        for (int j = 0; j < n; ++j) {
            additionalMat[k] = mem[i + j * n];
            k++;
        }
    }
    std::vector<double> recvVec;
    for (int i = 0; i < n; ++i) {
        recvVec.resize(n);
        if (i % size == rank) {
            recvVec = std::vector<double>(additionalMat.begin()+ n * (i / size),
                additionalMat.begin() + n * (i / size) + n);
        }
        MPI_Bcast(&recvVec[0], n, MPI_DOUBLE, i % size, MPI_COMM_WORLD);
        leaderElem = recvVec[i];
        if (leaderElem == 0.0) {
            for (int j = i + 1; j < n; ++j) {
                if (recvVec[j] != 0.0) {
                    std::swap(recvVec[i], recvVec[j]);
                    if (rank == 0) {
                        std::swap(copyCoefVec[i], copyCoefVec[j]);
                    }
                    for (int k = 0; k < colsCount; ++k) {
                        std::swap(additionalMat[k*n + i], additionalMat[k*n + j]);
                    }
                    leaderElem = recvVec[i];
                    break;
                }
                if (j == n - 1) {
                    throw(1);
                }
            }
        }
        for (int j = i / size; j < colsCount; ++j) {
            double leaderCol = additionalMat[j*n + i];
            for (int k = 0; k < n; ++k) {
                if (k == i) {
                    additionalMat[j*n + k] = isZero(additionalMat[j*n + k] / leaderElem);
                } else {
                    additionalMat[j*n + k] = isZero(additionalMat[j*n + k]
                        - leaderCol * recvVec[k] / leaderElem);
                }
            }
        }
        if (rank == 0) {
            double leaderCol = copyCoefVec[i];
            for (int k = 0; k < n; ++k) {
                if (k == i) {
                    copyCoefVec[k] = isZero(copyCoefVec[k] / leaderElem);
                } else {
                    copyCoefVec[k] = isZero(copyCoefVec[k] - leaderCol
                        * recvVec[k] / leaderElem);
                }
            }
        }
    }
    return copyCoefVec;
}

double myAbs(const double & number) {
    if (number >= 0.0) {
        return number;
    } else {
        return -number;
    }
}

void getRandomVector(std::vector<double>* vec) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < static_cast<int>((*vec).size()); ++i) {
        (*vec)[i] = static_cast<double>(gen() % 10);
    }
}

double isZero(const double number) {
    if (std::abs(number) < 0.0000000001) {
        return 0.0;
    } else {
        return number;
    }
}

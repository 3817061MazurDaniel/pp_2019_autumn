// Copyright 2019 Kriukov Dmitry
#include <mpi.h>
#include <random>
#include <ctime>
#include <vector>
#include "../../../modules/task_2/kriukov_d_fixed_point_iteration/fixed_point_iteration.h"

std::vector<double> getRandomVector(int sz) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<double> vec(sz);
    for (int i = 0; i < sz; i++) {
        vec[i] = gen()%99 + 1;
    }
    return vec;
}

std::vector<double> getNormalMatrix(int sz) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<double> vec(sz*sz);
    for (int i = 0; i < sz*sz; i++) {
        vec[i] = gen() % 99 + 1;
    }
    for (int i = 0; i < sz; i++) {
        vec[i*sz + i] += 100*sz;
    }
    return vec;
}

double getScalarProduct(const std::vector<double>& a, const std::vector<double>& b) {
    if (a.size() != b.size()) {
        throw(1);
    }
    double res = 0;
    for (unsigned int i = 0; i < a.size(); i++) {
        res += a[i] * b[i];
    }
    return res;
}

double getError(std::vector<double> vec1, std::vector<double> vec2, int var_count) {
    double res = 0.0;
    for (int i = 0; i < var_count; i++) {
        res += std::abs(vec2[i] - vec1[i]);
    }
    return res;
}

double getAbsError(std::vector<double> matrix,
                   std::vector<double> free_term, std::vector<double> resvec, int var_count) {
    double res = 0;
    for (int i = 0; i < var_count; i++) {
        std::vector<double> lvec = std::vector<double>(matrix.begin() + i * var_count,
        matrix.begin() + (i + 1) * var_count);
        res += getScalarProduct(resvec, lvec) - free_term[i];
    }
    return res;
}

double fixedPointIterationStep(std::vector<double> src_vect,
                               std::vector<double> base, double free_term, int var_count, int ind) {
    std::vector<double> vec(var_count);
    double dev = src_vect[ind];
    for (int j = 0; j < var_count; j++) {
        vec[j] = -src_vect[j] / dev;
    }
    vec[ind] = 0;
    double res = getScalarProduct(base, vec) + free_term / dev;
    return res;
}

std::vector<double> fixedPointIterationSequential(std::vector<double> matrix,
                                                  std::vector<double> free_term, int var_count, double err) {
    std::vector<double> res(var_count);
    std::vector<double> buff(var_count);
    double currerr = 0;
    do {
        for (int i = 0; i < var_count; i++) {
            std::vector<double> local_vec = std::vector<double>(matrix.begin() + i * var_count,
                                                                matrix.begin() + (i + 1) * var_count);
            res[i] = fixedPointIterationStep(local_vec, buff, free_term[i], var_count, i);
        }
         currerr = getError(buff, res, var_count);
        buff = res;
    } while (std::abs(currerr) >= err);
    return res;
}

std::vector<double> fixedPointIterationParralel(std::vector<double> matrix,
                                                std::vector<double> free_term, int var_count, double err) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (var_count < size) {
        std::vector<double> res(var_count);
        if (rank == 0) {
            res = fixedPointIterationSequential(matrix, free_term, var_count, err);
        }
        MPI_Bcast(&res[0], var_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        return res;
    }
    std::vector<double> fterm(var_count);
    const int delta = (var_count / size);
    const int rem = var_count %size;

    if (rank == 0) {
        fterm = free_term;
        for (int proc = 1; proc < size; proc++) {
            MPI_Request reqv;
            MPI_Isend(&matrix[0] + rem* var_count + proc * delta * var_count,
                      delta * var_count, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD, &reqv);
        }
    }

    MPI_Bcast(&fterm[0], var_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    std::vector<double> local_vec(delta * var_count);
    if (rank == 0) {
        local_vec.resize((delta + rem) * var_count);
        local_vec = std::vector<double>(matrix.begin(), matrix.begin() + (delta + rem) * var_count);
    } else {
        MPI_Status status;
        MPI_Recv(&local_vec[0], delta * var_count, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }
    std::vector<double> res(var_count);
    double currerr = 0;
    do {
        std::vector<double> buff(var_count);
        if (rank == 0) {
            for (int i = 0; i < delta + rem; i++) {
                std::vector<double> lvec = std::vector<double>(local_vec.begin() + i * var_count,
                                                               local_vec.begin() + (i + 1) * var_count);
                buff[i] = fixedPointIterationStep(lvec, res, fterm[i], var_count, i);
            }
            for (int j = delta + rem; j < var_count; j++) {
                MPI_Status status;
                MPI_Recv(&buff[j], 1, MPI_DOUBLE, (j - rem) / delta, j, MPI_COMM_WORLD, &status);
            }
            currerr = getError(buff, res, var_count);
            res = buff;
        } else {
            for (int i = 0; i < delta; i++) {
                int ind = rem + i + rank * delta;
                std::vector<double> lvec = std::vector<double>(local_vec.begin() + i * var_count,
                                                               local_vec.begin() + (i + 1) * var_count);
                double stepRes = fixedPointIterationStep(lvec, res, fterm[ind], var_count, ind);
                MPI_Send(&stepRes, 1, MPI_DOUBLE, 0, ind, MPI_COMM_WORLD);
            }
        }
        MPI_Bcast(&res[0], var_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&currerr, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } while (std::abs(currerr) >= err);
    return res;
}

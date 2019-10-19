// Copyright 2018 Koroleva Daria
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/koroleva_d_matrix_min_in_line/matrix_min_in_line.h"

std::vector<int> getRandomMatrix(int m, int n) {
    if (m <= 0 || n <= 0)
        throw - 1;

    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    int elem_num = m * n;
    std::vector<int> matr(elem_num);
    for (int i = 0; i < elem_num; ++i) {
        matr[i] = gen() % 100;
    }
    return matr;
}

std::vector<int> getMinsInMatrixLines(const std::vector<int>& matr, int m, int n) {
    if (m <= 0 || n <= 0)
        throw - 1;

    std::vector<int> out;
    for (int i = 0; i < m; ++i) {
        int firstelem_num = i * n;
        int next_line_elem = (i + 1) * n;

        out.push_back(*std::min_element(matr.begin() + firstelem_num,
                                        matr.begin() + next_line_elem));
    }
    return out;
}

std::vector<int> getParallelMinsInMatrixLines
                 (const std::vector<int>& matr, int m, int n) {
    if (m <= 0 || n <= 0)
        throw -1;

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size > m) size = m;
    if (rank >= size) return std::vector<int>();

    const int delta_lines = m / size;
    const int delta_ints = delta_lines * n;
    const int rem_lines = m % size;
    const int rem_ints = rem_lines * n;

    int lines_per_process = delta_lines;
    int ints_per_process = delta_ints;
    if (rank == 0) {
        lines_per_process = rem_lines + delta_lines;
        ints_per_process = rem_ints + delta_ints;
    }

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(&matr[rem_ints] + proc * delta_ints, delta_ints,
                MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> local_vec(ints_per_process);
    if (rank == 0) {
        local_vec = std::vector<int>(matr.begin(),
                                     matr.begin() + ints_per_process);
    } else {
        MPI_Status status;
        MPI_Recv(&local_vec[0], ints_per_process,
            MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    std::vector<int> local_mins = getMinsInMatrixLines(local_vec, lines_per_process, n);

    if (rank != 0) {
        MPI_Send(&local_mins[0], lines_per_process,
            MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    std::vector<int> out_vec(m);
    if (rank == 0) {
        out_vec = local_mins;
        MPI_Status status;
        for (int proc = 1; proc < size; proc++) {
            std::vector<int> tmp(delta_lines);
            MPI_Recv(&tmp[0], delta_lines,
                MPI_INT, proc, 0, MPI_COMM_WORLD, &status);
            out_vec.insert(out_vec.end(), tmp.begin(), tmp.end());
        }
    }

    return out_vec;
}

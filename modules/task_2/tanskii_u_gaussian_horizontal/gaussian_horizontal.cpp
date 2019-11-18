// Copyright 2019 Tanskii Yuriy
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <stdexcept>
#include <algorithm>
#include "../../../modules/task_2/tanskii_u_gaussian_horizontal/gaussian_horizontal.h"

std::vector<int> getRandomMatrix(int m, int n) {
    if (m <= 0 || n <= 0)
        throw std::runtime_error("Matrix size <= 0");
    if (m < (n - 1))
        throw std::runtime_error("Infinitity solution or doesn't sovmestna");
    if (m > (n - 1))
        throw std::runtime_error("System have no solution");
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    int size_m = m * n;
    std::vector<int> matrix(size_m);
    for (int i = 0; i < size_m; ++i) {
        matrix[i] = gen() % 10;
    }
    return matrix;
}
std::vector<double> gaussianSequential(const std::vector<double>& matrix, int m, int n) {
    if (m <= 0 || n <= 0)
        throw std::runtime_error("Matrix size <= 0");
    if (m < (n - 1))
        throw std::runtime_error("Infinitity solution or doesn't sovmestna");
    if (m > (n - 1))
        throw std::runtime_error("System have no solution");
    std::vector<double> local_matrix(matrix);
    std::vector<double> result_vec(m);
    for (int k = 0; k < m; ++k) {
        double tmp = local_matrix[k*n + k];
        for (int j = m; j >= k; j--) {
            local_matrix[k*n + j] /= tmp;
        }
        for (int j = k + 1; j < m; j++) {
            tmp = local_matrix[j*n + k];
            for (int i = m; i >= k; i--) {
                local_matrix[j*n + i] -= tmp * local_matrix[k*n + i];
            }
        }
    }
    result_vec[m - 1] = local_matrix[(m - 1)*n + m];
    for (int i = m - 2; i >= 0; i--) {
        result_vec[i] = local_matrix[i*n + m];
        for (int j = i + 1; j < m; j++)
            result_vec[i] -= local_matrix[i*n + j] * result_vec[j];
    }
    return result_vec;
}

std::vector<double> gaussianParallel(const std::vector <double> &matrix, int m, int n) {
    if (m <= 0 || n <= 0)
        throw std::runtime_error("Matrix size <= 0");
    if (m < (n-1))
        throw std::runtime_error("Infinitity solution or doesn't sovmestna");
    if (m > (n-1))
        throw std::runtime_error("System have no solution");
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta_lines = m / size;
    const int rem = m % size;
    int tmp = delta_lines;
    if (rank < rem) {
        tmp = delta_lines + 1;
    }
    std::vector<double> local_vec(tmp * n);
    if (local_vec.size() == 0)
        local_vec.resize(1);
    std::vector<int> proc_elems(size);
    std::vector<int> proc_offset(size);
    proc_offset[0] = 0;
    for (int i = 0; i < size; ++i) {
        if (i < rem) {
            proc_elems[i] = (delta_lines + 1)*n;
        } else {
            proc_elems[i] = delta_lines * n;
        }
        if (i)
            proc_offset[i] = (proc_offset[i - 1] + proc_elems[i - 1]);
    }
    MPI_Scatterv(matrix.data(), proc_elems.data(), proc_offset.data(),
        MPI_DOUBLE, &local_vec[0], tmp * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    std::vector<double> leader_row(n);
    for (int i = 0; i < proc_offset[rank] / n; i++) {
        int root = 0;
        int sum = 0;
        for (int j = 0; j < size; ++j, ++root) {
            sum += proc_elems[j] / n;
            if (i < sum) {
                root = j;
                break;
            }
        }
        MPI_Bcast(&leader_row[0], n, MPI_DOUBLE, root, MPI_COMM_WORLD);
        for (int j = 0; j < proc_elems[rank] / n; j++) {
            double d = leader_row[i] / local_vec[j * n + i];
            for (int k = i; k < n; ++k)
                local_vec[j * n + k] = d * local_vec[j * n + k] - leader_row[k];
        }
    }
    for (int i = 0; i < proc_elems[rank] / n; ++i) {
        for (int j = 0; j < n; ++j)
            leader_row[j] = local_vec[i * n + j];
        MPI_Bcast(&leader_row[0], n, MPI_DOUBLE, rank, MPI_COMM_WORLD);
        for (int j = i + 1; j < proc_elems[rank] / n; j++) {
            double s = leader_row[proc_offset[rank] / n + i] / local_vec[j * n + i + proc_offset[rank] / n];
            for (int k = i + proc_offset[rank] / n; k < n; ++k)
                local_vec[j * n + k] = s * local_vec[j * n + k] - leader_row[k];
        }
    }
    int root = 0;
    for (int i = proc_offset[rank] / n + proc_elems[rank] / n; i < m; ++i) {
        int sum = 0;
        for (int j = 0; j < size; ++j, ++root) {
            sum += proc_elems[j] / n;
            if (i < sum) {
                root = j; break;
            }
        }
        MPI_Bcast(&leader_row[0], n, MPI_DOUBLE, root, MPI_COMM_WORLD);
    }
    std::vector<double> tmp_vec(0);
    if (rank == 0)
        tmp_vec.resize(m * n);
    MPI_Gatherv(local_vec.data(), tmp * n, MPI_DOUBLE, tmp_vec.data(),
        proc_elems.data(), proc_offset.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    std::vector<double> result_vec(0);
    if (rank == 0) {
        result_vec.resize(m);
        for (int i = m - 1; i >= 0; --i) {
            double B = tmp_vec[i * n + n - 1];
            for (int j = m - 1; j >= i + 1; --j)
                B -= tmp_vec[i * n + j] * result_vec[j];
            result_vec[i] = B / tmp_vec[i * n + i];
        }
    }
    return result_vec;
}

  // Copyright 2019 Boganov Sergey
#include <mpi.h>
#include <vector>
#include <random>
#include <string>
#include <ctime>
#include <utility>
#include <iostream>
#include "../../../modules/task_2/boganov_s_gauss_method_horizontal_scheme/gauss_method_horizontal_scheme.h"

bool Ax_b_check(const std::vector <double> &a, unsigned int n, const std::vector<double> &x) {
    std::vector <double> res(n, 0);

    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < n; j++) {
            res[i] += a[j + i * (n + 1)] * x[j];
        }
    }

    for (unsigned int i = 0; i < n; i++) {
        if (res[i] - a[n + i * (n + 1)] > 1e-6) {
            return false;
        }
    }

    return true;
}

std::vector<double> getRandomMatrix(unsigned int n, double min_rand, double max_rand) {
    std::uniform_real_distribution<double> unif(min_rand, max_rand);
    std::mt19937 rand_engine(static_cast<unsigned int>(time(0)));

    std::vector<double> matrix(n * (n + 1));

    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < n + 1; j++) {
            matrix[j + i * (n + 1)] = unif(rand_engine);
        }
    }

    return matrix;
}

std::vector<double> getSequentialGauss(std::vector<double> matrix, unsigned int n) {
    if (n < 2) {
        throw static_cast<std::string>("Размер матрицы должен быть хотя бы 2 х 2");
    }
    if ((n * (n + 1)) != matrix.size()) {
        throw static_cast<std::string>("Размер матрицы должен быть равен m * (n + 1)");
    }

    for (unsigned int i = 0; i < n; i++) {
        double max_elem = std::abs(matrix[i + i * (n + 1)]);
        unsigned int index_max_elem = i;

        for (unsigned int j = i + 1; j < n; j++) {
            if (std::abs(matrix[i + j * (n + 1)]) > max_elem) {
                max_elem = std::abs(matrix[i + j * (n + 1)]);
                index_max_elem = j;
            }
        }

        if (max_elem < 1e-6) {
            throw static_cast<std::string>("Система несовместна");
        }

        for (unsigned int j = 0; j < n + 1; j++) {
            double temp = matrix[j + i * (n + 1)];
            matrix[j + i * (n + 1)] = matrix[j + index_max_elem * (n + 1)];
            matrix[j + index_max_elem * (n + 1)] = temp;
        }

        for (unsigned int j = i; j < n; j++) {
            double temp = matrix[i + j * (n + 1)];

            if (std::abs(temp) < 1e-6) {
                continue;
            }

            for (unsigned int k = 0; k < n + 1; k++) {
                matrix[k + j * (n + 1)] /= temp;
            }

            if (j == i) {
                continue;
            }

            for (unsigned int k = 0; k < n + 1; k++) {
                matrix[k + j * (n + 1)] -= matrix[k + i * (n + 1)];
            }
        }
    }

    for (unsigned int i = 0; i < n; i++) {
        bool key = true;
        for (unsigned int j = 0; j < n; j++) {
            if (std::abs(matrix[j + i * (n + 1)]) >= 1e-6) {
                key = false;
            }
        }
        if (key && (std::abs(matrix[n + i * (n + 1)]) >= 1e-6)) {
            throw static_cast<std::string>("Система несовместна");
        } else if (key && (std::abs(matrix[n + i * (n + 1)]) < 1e-6)) {
            throw static_cast<std::string>("Система имеет бесконечное множество решений");
        }
    }

    std::vector<double> res(n);

    for (int i = static_cast<int>(n - 1); i >= 0; i--) {
        res[i] = matrix[n + i * (n + 1)];
        for (int j = 0; j < i; j++) {
            matrix[n + j * (n + 1)] -= matrix[i + j *(n + 1)] * res[i];
        }
    }

    return res;
}

double getSequentialMaxElem(const std::vector<double> &matrix, unsigned int n, unsigned int row0, unsigned int rows,
unsigned int i, unsigned int *index) {
    double max_elem = 0;
    for (unsigned int j = row0; j < rows; j++) {
        if (std::abs(matrix[i + j * (n + 1)]) > max_elem) {
            max_elem = std::abs(matrix[i + j * (n + 1)]);
            *index = j;
        }
    }

    return max_elem;
}

int getParallelIndexMaxElem(const std::vector<double> &matrix, unsigned int row_count, int rank, int size,
unsigned int i, unsigned int n) {
    if (i == n) {
        int ans = i - 1;
        return ans;
    }

    unsigned int delta = n / size, rem = n % size;

    double global_max_elem = 0;
    double local_max_elem = 0;

    unsigned int global_index_max_elem = i;
    unsigned int local_index_max_elem = i;

    int local_i = i - rank * delta;
    int rank_i = findRank(n, size, delta, rem, i);

    if (rank == rank_i) {
        local_max_elem = getSequentialMaxElem(matrix, n, local_i, row_count, i, &local_index_max_elem);
        local_index_max_elem += rank * delta;
    } else if (rank > rank_i) {
        local_max_elem = getSequentialMaxElem(matrix, n, 0, row_count, i, &local_index_max_elem);
        local_index_max_elem += rank * delta;
    }

    if (rank != 0) {
        MPI_Send(&local_max_elem, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&local_index_max_elem, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD);
    } else {
        std::vector<std::pair<double, int>> local_max_vec(size);
        local_max_vec[0] = std::make_pair(local_max_elem, local_index_max_elem);

        for (int j = 1; j < size; j++) {
            MPI_Status status;
            MPI_Recv(&local_max_vec[j].first, 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&local_max_vec[j].second, 1, MPI_UNSIGNED, j, 0, MPI_COMM_WORLD, &status);
        }

        for (int j = 0; j < size; j++) {
            if (std::abs(local_max_vec[j].first) > global_max_elem) {
                global_max_elem = std::abs(local_max_vec[j].first);
                global_index_max_elem = local_max_vec[j].second;
            }
        }
    }

    int err_code = -1;

    if (rank == 0) {
        if (global_max_elem < 1e-6) {
            err_code = 0;
        }
    }

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(&err_code, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Status status;
        MPI_Recv(&err_code, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    if (err_code == 0) {
        throw static_cast<std::string>("Система несовместна");
    }

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(&global_index_max_elem, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Status status;
        MPI_Recv(&global_index_max_elem, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    return global_index_max_elem;
}

int findRank(unsigned int n, unsigned int size, unsigned int delta, unsigned int rem, unsigned int index) {
    std::vector<int> find_rank;
    unsigned int temp_ind = 0;

    for (unsigned int i = 0; i < size - 1; i++) {
        for (unsigned int j = 0; j < delta; j++) {
            find_rank.push_back(temp_ind);
        }
        temp_ind++;
    }
    for (unsigned int j = 0; j < delta + rem; j++) {
        find_rank.push_back(temp_ind);
    }

    return find_rank[index];
}

void getSwapedRows(unsigned int n, unsigned int ind, int rank, int size,
unsigned int index_max_elem, std::vector<double>* matrix) {
    unsigned int delta = n / size, rem = n % size;

    MPI_Status status;
    std::vector<double> temp_row(n + 1, 0);
    int rank_i, rank_max;

    if (rank == 0) {
        rank_i = findRank(n, size, delta, rem, ind);
        rank_max = findRank(n, size, delta, rem, index_max_elem);
    }

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(&rank_i, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Status status;
        MPI_Recv(&rank_i, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(&rank_max, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Status status;
        MPI_Recv(&rank_max, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    int local_ind_i = ind - rank * delta, local_ind_max = index_max_elem - rank * delta;

    if (rank_i == rank_max) {
        if (rank == rank_i) {
            for (unsigned int j = 0; j < n + 1; j++) {
                double temp = (*matrix)[j + local_ind_i * (n + 1)];
                (*matrix)[j + local_ind_i * (n + 1)] = (*matrix)[j + local_ind_max * (n + 1)];
                (*matrix)[j + local_ind_max * (n + 1)] = temp;
            }
        }
        return;
    }

    if (rank == rank_i) {
        MPI_Send(&(*matrix)[0] + local_ind_i * (n + 1), n + 1, MPI_DOUBLE, rank_max, 0, MPI_COMM_WORLD);
    }
    if (rank == rank_max) {
        MPI_Recv(&temp_row[0], n + 1, MPI_DOUBLE, rank_i, 0, MPI_COMM_WORLD, &status);
    }

    if (rank == rank_max) {
        MPI_Send(&(*matrix)[0] + local_ind_max * (n + 1), n + 1, MPI_DOUBLE, rank_i, 0, MPI_COMM_WORLD);
    }
    if (rank == rank_i) {
        MPI_Recv(&temp_row[0], n + 1, MPI_DOUBLE, rank_max, 0, MPI_COMM_WORLD, &status);
    }

    if (rank == rank_i) {
        for (unsigned int j = 0; j < n + 1; j++) {
            (*matrix)[j + local_ind_i * (n + 1)] = temp_row[j];
        }
    }

    if (rank == rank_max) {
        for (unsigned int j = 0; j < n + 1; j++) {
            (*matrix)[j + local_ind_max * (n + 1)] = temp_row[j];
        }
    }
}

void getParallelNormalizedRows(unsigned int n, unsigned int i, int rank, int size,
std::vector<double>* matrix, unsigned int row_count) {
    unsigned int delta = n / size, rem = n % size;

    int local_ind = i - rank * delta;
    int rank_i = findRank(n, size, delta, rem, i);

    if (rank == rank_i) {
        for (unsigned int j = local_ind; j < row_count; j++) {
            double temp = (*matrix)[i + j * (n + 1)];

            if (std::abs(temp) < 1e-6) {
                continue;
            }

            for (unsigned int k = 0; k < n + 1; k++) {
                (*matrix)[k + j * (n + 1)] /= temp;
            }
        }
    } else if (rank > rank_i) {
        for (unsigned int j = 0; j < row_count; j++) {
            double temp = (*matrix)[i + j * (n + 1)];

            if (std::abs(temp) < 1e-6) {
                continue;
            }

            for (unsigned int k = 0; k < n + 1; k++) {
                (*matrix)[k + j * (n + 1)] /= temp;
            }
        }
    }
}

void getParallelSubtractRows(unsigned int n, unsigned int ind, int rank, int size,
std::vector<double>* matrix, unsigned int row_count) {
    unsigned int delta = n / size, rem = n % size;

    int local_ind = ind - rank * delta;
    int rank_i = findRank(n, size, delta, rem, ind);

    if (rank == rank_i) {
        if (rank != size - 1) {
            for (int r = rank_i + 1; r < size; r++) {
                MPI_Send(&(*matrix)[0] + local_ind * (n + 1), n + 1, MPI_DOUBLE, r, 0, MPI_COMM_WORLD);
            }
        }

        for (unsigned int j = local_ind + 1; j < row_count; j++) {
            if (std::abs((*matrix)[ind + j * (n + 1)]) < 1e-6) {
                continue;
            }

            for (unsigned int k = 0; k < n + 1; k++) {
               (*matrix)[k + j * (n + 1)] -= (*matrix)[k + local_ind * (n + 1)];
            }
        }

    } else if (rank > rank_i) {
        std::vector<double> temp(n + 1);
        MPI_Status status;
        MPI_Recv(&temp[0], n + 1, MPI_DOUBLE, rank_i, 0, MPI_COMM_WORLD, &status);

        for (unsigned int j = 0; j < row_count; j++) {
            if (std::abs((*matrix)[ind + j * (n + 1)]) < 1e-6) {
                continue;
            }

            for (unsigned int k = 0; k < n + 1; k++) {
                (*matrix)[k + j * (n + 1)] -= temp[k];
            }
        }
    }
}

std::vector<double> getParallelGauss(std::vector<double> matrix, unsigned int n) {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int err_code = -1;
    int proc_size = size;

    if (size > static_cast<int>(n)) {
        size = static_cast<int>(n);
        if (rank >= size) {
            MPI_Status status;

            MPI_Recv(&err_code, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, &status);
            if (err_code == 1) {
                throw static_cast<std::string>("Размер матрицы должен быть хотя бы 2 х 2");
            } else if (err_code == 2) {
                throw static_cast<std::string>("Размер матрицы должен быть равен m * (n + 1)");
            }

            MPI_Recv(&err_code, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, &status);
            if (err_code == 3) {
                throw static_cast<std::string>("Система несовместна");
            }

            MPI_Recv(&err_code, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, &status);
            if (err_code == 4) {
                throw static_cast<std::string>("Система несовместна");
            } else if (err_code == 5) {
                throw static_cast<std::string>("Система имеет бесконечное множество решений");
            }

            return matrix;
        }
    }

    unsigned int delta = n / size, rem = n % size;
    unsigned int row_count;

    if (rank == size - 1) {
        if (n < 2) {
            err_code = 1;
        }
        if ((n * (n + 1)) != matrix.size()) {
            err_code = 2;
        }
    }

    if (rank == size - 1) {
        for (int i = 0; i < size - 1; i++) {
            MPI_Send(&err_code, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        for (int i = size; i < proc_size; i++) {
            MPI_Send(&err_code, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Status status;
        MPI_Recv(&err_code, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, &status);
    }

    if (rank == size - 1) {
        for (int i = 0; i < size - 1; i++) {
            MPI_Send(&matrix[0] + i * delta * (n + 1), delta * (n + 1), MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
    }

    if (err_code == 1) {
        throw static_cast<std::string>("Размер матрицы должен быть хотя бы 2 х 2");
    } else if (err_code == 2) {
        throw static_cast<std::string>("Размер матрицы должен быть равен m * (n + 1)");
    }

    std::vector<double> local_matrix(delta * (n + 1));

    if (rank == size - 1) {
        local_matrix = std::vector<double>(matrix.begin() + (size - 1) * delta * (n + 1),
            matrix.begin() + (size - 1) * delta * (n + 1)  + (delta + rem) * (n + 1));
        row_count = delta + rem;
    } else {
        MPI_Status status;
        MPI_Recv(&local_matrix[0], delta * (n + 1), MPI_DOUBLE, size - 1, 0, MPI_COMM_WORLD, &status);
        row_count = delta;
    }

    for (unsigned int i = 0; i < n; i++) {
        unsigned index_max_elem = i;
        if (i + 1 != n) {
            try {
                index_max_elem = getParallelIndexMaxElem(local_matrix, row_count, rank, size, i, n);
            } catch (std::string err) {
                err_code = 3;
                break;
            }
        }

        getSwapedRows(n, i, rank, size, index_max_elem, &local_matrix);
        getParallelNormalizedRows(n, i, rank, size, &local_matrix, row_count);
        getParallelSubtractRows(n, i, rank, size, &local_matrix, row_count);
    }

    if (rank == size - 1) {
        for (int l = size; l < proc_size; l++) {
            MPI_Send(&err_code, 1, MPI_INT, l, 0, MPI_COMM_WORLD);
        }
    }

    if (err_code == 3) {
        throw static_cast<std::string>("Система несовместна");
    }

    std::vector<double> res(n, 0);

    if (rank != size - 1) {
        MPI_Send(&local_matrix[0], delta * (n + 1), MPI_DOUBLE, size - 1, 0, MPI_COMM_WORLD);
    } else {
        MPI_Status status;
        for (int i = 0; i < size - 1; i++) {
            MPI_Recv(&matrix[0] + i * delta * (n + 1), delta * (n + 1), MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
        }
        for (unsigned int i = 0; i < delta + rem; i++) {
            for (unsigned int j = 0; j < n + 1; j++) {
                matrix[j + (i + delta * (size - 1)) * (n + 1)] = local_matrix[j + i * (n + 1)];
            }
        }

        for (unsigned int i = 0; i < n; i++) {
            bool key = true;
            for (unsigned int j = 0; j < n; j++) {
                if (std::abs(matrix[j + i * (n + 1)]) >= 1e-6) {
                    key = false;
                }
            }
            if (key && (std::abs(matrix[n + i * (n + 1)]) >= 1e-6)) {
                err_code = 4;
            } else if (key && (std::abs(matrix[n + i * (n + 1)]) < 1e-6)) {
                err_code = 5;
            }
        }

        for (int i = static_cast<int>(n - 1); i >= 0; i--) {
            res[i] = matrix[n + i * (n + 1)];
            for (int j = 0; j < i; j++) {
                matrix[n + j * (n + 1)] -= matrix[i + j * (n + 1)] * res[i];
            }
        }
    }

    if (rank == size - 1) {
        for (int i = 0; i < size - 1; i++) {
            MPI_Send(&err_code, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        for (int i = size; i < proc_size; i++) {
            MPI_Send(&err_code, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Status status;
        MPI_Recv(&err_code, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, &status);
    }

    if (err_code == 4) {
        throw static_cast<std::string>("Система несовместна");
    } else if (err_code == 5) {
        throw static_cast<std::string>("Система имеет бесконечное множество решений");
    }

    return res;
}

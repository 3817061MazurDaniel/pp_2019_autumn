// Copyright 2019 Savkin Yuriy
#include <mpi.h>
#include <vector>
#include <functional>
#include <random>
#include "../../../modules/task_3/savkin_y_conjugate_gradient_method/conjugate_gradient_method.h"

double** getRandomMatrix(int size, double miv, double mav, unsigned seed) {
    double** a = new double*[size];
    a[0] = new double[size * size];
    for (int i = 1; i < size; ++i)
        a[i] = a[i - 1] + size;
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> urd(miv, mav);
    for (int i = 0; i < size; ++i)
        for (int j = i; j < size; ++j)
            a[i][j] = a[j][i] = urd(mt);
    return a;
}

double* getRandomVector(int size, double miv, double mav, unsigned seed) {
    double* a = new double[size];
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> urd(miv, mav);
    for (int i = 0; i < size; ++i)
        a[i] = urd(mt);
    return a;
}

std::vector<double> conjugateGradientMethod(double** a, const double* b, int size) {
    int comm_size, comm_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    int rows = size / comm_size + ((comm_size - comm_rank <= size % comm_size) ? 1 : 0);

    std::function<double(const std::vector<double>&, const std::vector<double>&)> vector_mul =
    [](const std::vector<double>& a, const std::vector<double>& b)->double {
        double result = 0.0;
        for (size_t i = 0; i < a.size(); ++i)
            result += a[i] * b[i];
        return result;
    };

    std::function<void(double**, const std::vector<double>&, std::vector<double>*)> mat_vec_mul =
    [rows](double** a, const std::vector<double>& b, std::vector<double>* r)->void {
        for (int i = 0; i < rows; ++i) {
            (*r)[i] = 0.0;
            for (size_t j = 0; j < b.size(); ++j) {
                (*r)[i] += a[i][j] * b[j];
            }
        }
    };

    double** m;
    if (rows > 0) {
        m = new double*[rows];
    } else {
        m = new double*[1];
    }
    m[0] = new double[rows * size];
    for (int i = 1; i < rows; ++i)
        m[i] = m[i - 1] + size;

    std::vector<double> x(rows), z(rows), r(rows), w(rows), z_all(size);

    std::vector<int> counts(comm_size);
    std::vector<int> displs(comm_size);
    if (comm_rank == 0) {
        counts[0] = rows * size;
        displs[0] = 0;
        for (int i = 1; i < comm_size; ++i) {
            int step = size / comm_size + ((comm_size - i <= size % comm_size) ? 1 : 0);
            counts[i] = step * size;
            displs[i] = displs[i - 1] + counts[i - 1];
        }
        MPI_Scatterv(a[0], counts.data(), displs.data(), MPI_DOUBLE, m[0], counts[0], MPI_DOUBLE, 0, MPI_COMM_WORLD);
        for (int i = 0; i < size; ++i)
            z_all[i] = b[i];
    } else {
        MPI_Scatterv(nullptr, nullptr, nullptr, MPI_DOUBLE, m[0], rows * size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    MPI_Bcast(z_all.data(), size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    counts[0] = size / comm_size;
    displs[0] = 0;
    for (int i = 1; i < comm_size; ++i) {
        counts[i] = size / comm_size + ((comm_size - i <= size % comm_size) ? 1 : 0);
        displs[i] = displs[i - 1] + counts[i - 1];
    }

    for (int i = 0; i < rows; ++i)
        z[i] = r[i] = z_all[i + displs[comm_rank]];

    double tmp = vector_mul(r, r), u;
    MPI_Allreduce(&tmp, &u, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    for (int i = 0; i < size; ++i) {
        mat_vec_mul(m, z_all, &w);

        double h;
        tmp = vector_mul(w, z);
        MPI_Allreduce(&tmp, &h, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        double t1 = u / h;
        for (int j = 0; j < rows; ++j) {
            x[j] += t1 * z[j];
            r[j] -= t1 * w[j];
        }

        double g;
        tmp = vector_mul(r, r);
        MPI_Allreduce(&tmp, &g, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        double t2 = g / u;
        u = g;
        for (int j = 0; j < rows; ++j) {
            z[j] = t2 * z[j] + r[j];
        }
        MPI_Allgatherv(z.data(), rows, MPI_DOUBLE,
            z_all.data(), counts.data(), displs.data(), MPI_DOUBLE, MPI_COMM_WORLD);
    }

    delete[] m[0];
    delete[] m;

    std::vector<double> ans;

    if (comm_rank == 0) {
        ans.resize(size);
        MPI_Gatherv(x.data(), rows, MPI_DOUBLE,
            ans.data(), counts.data(), displs.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } else {
        MPI_Gatherv(x.data(), rows, MPI_DOUBLE, nullptr, nullptr, nullptr, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    return ans;
}

std::vector<double> conjugateGradientMethodOneProc(double** a, const double* b, int size) {
    std::function<double(const std::vector<double>&, const std::vector<double>&)> vector_mul =
    [](const std::vector<double>& a, const std::vector<double>& b)->double {
        double result = 0.0;
        for (size_t i = 0; i < a.size(); ++i)
            result += a[i] * b[i];
        return result;
    };

    std::function<void(double**, const std::vector<double>&, std::vector<double>*)> mat_vec_mul =
    [size](double** a, const std::vector<double>& b, std::vector<double>* r)->void {
        for (int i = 0; i < size; ++i) {
            (*r)[i] = 0.0;
            for (size_t j = 0; j < b.size(); ++j) {
                (*r)[i] += a[i][j] * b[j];
            }
        }
    };

    std::vector<double> x(size), z(size), r(size), w(size);
    for (int i = 0; i < size; ++i)
        z[i] = r[i] = b[i];

    double u = vector_mul(r, r);

    for (int i = 0; i < size; ++i) {
        mat_vec_mul(a, z, &w);

        double h = vector_mul(w, z);
        double t1 = u / h;
        for (int j = 0; j < size; ++j) {
            x[j] += t1 * z[j];
            r[j] -= t1 * w[j];
        }

        double w = vector_mul(r, r);
        double t2 = w / u;
        u = w;
        for (int j = 0; j < size; ++j) {
            z[j] = t2 * z[j] + r[j];
        }
    }

    return x;
}

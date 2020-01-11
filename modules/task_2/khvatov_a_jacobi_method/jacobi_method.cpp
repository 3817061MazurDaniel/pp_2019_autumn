// Copyright 2019 Khvatov Alexander
#include <mpi.h>

#include <math.h>

#include "../../../modules/task_2/khvatov_a_jacobi_method/jacobi_method.h"

double* seq_jacobi_method(double* A, double *b, int n, double epsilon) {
    double* x_curr = new double[n];
    double* x_next = new double[n];
    double norm = 0.0;

    for (int i = 0; i < n; i++) {
        x_curr[i] = b[i];
    }

    do {
        for (int i = 0; i < n; i++) {
            x_next[i] = b[i];
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    x_next[i] -= A[i * n + j] * x_curr[j];
                }
            }
            x_next[i] /= A[(n + 1) * i];
        }
        norm = fabs(x_next[0] - x_curr[0]);
        for (int i = 0; i < n; i++) {
            double temp = fabs(x_next[i] - x_curr[i]);
            if (norm < temp) {
                norm = temp;
            }
            x_curr[i] = x_next[i];
        }
    } while (epsilon < norm);
    delete[] x_next;
    return x_curr;
}

double* par_jacobi_method(double* A, double *b, int n, double epsilon) {
    int proc_size = 0, proc_rank = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    int count = n / proc_size, addition = n % proc_size;

    double* A_buff = NULL;
    double* b_buff = NULL;
    double* x_curr = new double[n];
    double* x_next = NULL;

    if (0 == proc_rank) {
        A_buff = new double[(count + addition) * n];
        b_buff = new double[count + addition];
        x_next = new double[count + addition];

        for (int i = 0; i < n; i++) {
            x_curr[i] = b[i];
        }
    } else {
        A_buff = new double[count * n];
        b_buff = new double[count];
        x_next = new double[count];
    }

    int* displs_a = new int[proc_size];
    int* scounts_a = new int[proc_size];
    int* displs_b = new int[proc_size];
    int* scounts_b = new int[proc_size];

    displs_a[0] = 0;
    scounts_a[0] = (count + addition) * n;
    displs_b[0] = 0;
    scounts_b[0] = count + addition;
    for (int i = 1; i < proc_size; i++) {
        displs_a[i] = addition * n + count * n * i;
        scounts_a[i] = count * n;
        displs_b[i] = addition + count * i;
        scounts_b[i] = count;
    }

    MPI_Scatterv(A, scounts_a, displs_a, MPI_DOUBLE, A_buff, scounts_a[proc_rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(b, scounts_b, displs_b, MPI_DOUBLE, b_buff, scounts_b[proc_rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(x_curr, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (0 == proc_rank) {
        addition = 0;
    }

    double local_norm = 0.0, global_norm = 0.0;
    while (1) {
        for (int i = 0; i < scounts_b[proc_rank]; i++) {
            x_next[i] = b_buff[i];
            for (int j = 0; j < n; j++) {
                if ((i + addition + count * proc_rank) != j) {
                    x_next[i] -= A_buff[i * n + j] * x_curr[j];
                }
            }
            x_next[i] /= A_buff[(n + 1) * i + addition + count * proc_rank];
        }
        local_norm = fabs(x_next[0] - x_curr[addition + count * proc_rank]);
        for (int i = 0; i < count; i++) {
            double temp = fabs(x_next[i] - x_curr[addition + count * proc_rank + i]);
            if (local_norm < temp) {
                local_norm = temp;
            }
        }
        MPI_Gatherv(x_next, scounts_b[proc_rank], MPI_DOUBLE, x_curr, scounts_b,
            displs_b, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Allreduce(&local_norm, &global_norm, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        if (global_norm < epsilon) {
            break;
        } else {
            MPI_Bcast(x_curr, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        }
    }
    delete[] A_buff;
    delete[] b_buff;
    delete[] x_next;
    delete[] displs_a;
    delete[] displs_b;
    delete[] scounts_a;
    delete[] scounts_b;
    if (0 != proc_rank) {
        delete[] x_curr;
        x_curr = NULL;
    }
    return x_curr;
}

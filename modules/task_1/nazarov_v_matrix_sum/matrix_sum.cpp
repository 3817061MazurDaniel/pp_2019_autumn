// Copyright 2019 Nazarov Vladislav
#include <mpi.h>
#include <random>
#include <vector>
#include <stdexcept>
#include <ctime>
#include "../../../modules/task_1/nazarov_v_matrix_sum/matrix_sum.h"

int getParallelSum(std::vector<int> matrix, int matrix_size_n, int matrix_size_m) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int number_of_elems, index;
    int local_sum, global_sum = 0;
    if (rank == 0) {
        if (matrix_size_m * matrix_size_n != static_cast<int>(matrix.size())) {
            int err = -1;
            for (int proc_num = 1; proc_num < size; proc_num++)
                MPI_Send(&err, 1, MPI_INT, proc_num, 0, MPI_COMM_WORLD);
            throw std::runtime_error("Vector and matrix size different");
        }
        int send_size = 0;
        number_of_elems = matrix_size_n * matrix_size_m / size;
        int number_of_elems_remainder = (matrix_size_n * matrix_size_m) % size;
        index = number_of_elems;
        for (int proc_num = 1; proc_num < size; proc_num++) {
            if (number_of_elems_remainder > 0) {
                number_of_elems_remainder--;
                send_size = number_of_elems + 1;
            } else {
                send_size = number_of_elems;
            }
            MPI_Send(&send_size, 1, MPI_INT, proc_num, 0, MPI_COMM_WORLD);
            MPI_Send(&matrix[0]+index, send_size, MPI_INT, proc_num, 1, MPI_COMM_WORLD);
            index+=send_size;
        }
        local_sum = getSequentialSum(std::vector<int>(matrix.begin(), matrix.begin()+number_of_elems));
    } else {
        MPI_Status status;
        MPI_Recv(&number_of_elems, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        if (number_of_elems == -1)
            throw std::runtime_error("Vector and matrix size different");
        std::vector<int> local_vec(number_of_elems);
        MPI_Recv(&local_vec[0], number_of_elems, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        local_sum = getSequentialSum(local_vec);
    }
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_sum;
}

int getSequentialSum(std::vector<int> vec) {
    int sum = 0;
    int size = vec.size();
    for (int i = 0; i < size; i++)
        sum+=vec[i];
    return sum;
}

std::vector<int> getRandomMatrix(int n, int m) {
    if (n <= 0 || m <= 0)
        throw std::runtime_error("Matrix size <= 0");
    if (n > (INT32_MAX/100)/m || m > (INT32_MAX/100)/n)
        throw std::runtime_error("Matrix size too big (overflow opportuniy)");
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> matrix(n*m);
    for (int i = 0; i < n*m; i++)
        matrix[i] = gen() % 100;
    return matrix;
}

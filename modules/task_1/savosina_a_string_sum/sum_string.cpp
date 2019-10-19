// Copyright 2019 Savosina
#include <mpi.h>
#include <string>
#include <random>
#include <ctime>
#include <vector>
#include "../../../modules/task_1/savosina_a_string_sum/sum_string.h"

std::vector<char> getRandomVector(int sz) {
     if (sz < 0)
        throw "error";
    std::mt19937 gen;
    char tmp1[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz   \n\n\n";
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<char> vec(sz);
    char s;
    for (int  i = 0; i < sz; i++) {
        s = tmp1[gen() % (sizeof(tmp1) - 1)];
        vec[i] = s;
    }
    return vec;
}

int getSequentialOperationsVector(std::vector<char> str) {
    int count = 0;
    int t = str.size();
    for (int i = 0; i < t; i++) {
        if (isalnum(str[i]))
            count++;
    }
    return count;
}

int getParallelOperationsVector(std::vector<char> global_str, int sz) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = sz / size;
    const int modulo = sz % size;

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(&global_str[0] + modulo + i * delta, delta,
                    MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<char> local_str(delta);
    if (rank == 0) {
        local_str = std::vector<char>(global_str.begin(),
                global_str.begin() + delta + modulo);
    } else {
        MPI_Status status;
        MPI_Recv(&local_str[0], delta, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    }

    int global_count = 0;
    int local_count = getSequentialOperationsVector(local_str);
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_count;
}

int getParallelOperationsVectorTry(std::vector<char> global_str, int sz) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int delta = sz / size;
    if (sz%size != 0) {
        char tmp = ' ';
        delta++;
        for (int i = 0; i < ((delta + 1) * size - sz); i++) {
            global_str.push_back(tmp);
        }
    }
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(&global_str[0] + i * delta, delta,
                    MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<char> local_str(delta);
    if (rank == 0) {
        local_str = std::vector<char>(global_str.begin(),
                global_str.begin() + delta);
    } else {
        MPI_Status status;
        MPI_Recv(&local_str[0], delta, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    }

    int global_count = 0;
    int local_count = getSequentialOperationsVector(local_str);
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_count;
}

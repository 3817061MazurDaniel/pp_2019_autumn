// Copyright 2019 Suslov Egor
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_2/suslov_e_producers_consumers/producers_consumers.h"


std::vector<int> getRandomVector(int size_v) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vect(size_v);
    for (int i = 0; i < size_v; i++) { vect[i] = gen() % 100; }
    return vect;
}
int getPositive_elem(std::vector<int> vector, int count_size_vector) {
    if (count_size_vector < 2) {
        return 0;
    }
    int negative_elem = 0;
    for (int c = 0; c < count_size_vector; c++) {
        if (vector[c] == -1) {
            negative_elem++;
        }
    }
    return vector.size() - negative_elem;
}
int* Create_dinamic_massiv_from_vector(std::vector<int> vec) {
    int vec_size = vec.size();
    int* mas = new int[vec_size];
    for (int i = 0; i < vec_size; i++) {
        mas[i] = vec[i];
    }
    return mas;
}

int Consumer(int *buffer, int buffer_size, int rank_proc, int* resurce) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (size == 1 || rank_proc == 0) {
        for (int i = 0; i < buffer_size; i++) {
            if (buffer[i] != -1) {
                *resurce = buffer[i];
                buffer[i] = -1;
                break;
            }
        }
    } else {
        if (rank == 0) {
            int temp_resurs;
            for (int i = 0; i < buffer_size; i++) {
                if (buffer[i] != -1) {
                    temp_resurs = buffer[i];
                    buffer[i] = -1;
                    break;
                }
            }
            MPI_Send(&temp_resurs, 1, MPI_INT, rank_proc, 0, MPI_COMM_WORLD);
        } else {
            if (rank == rank_proc) {
                int temp_resurs;
                MPI_Recv(&temp_resurs, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                *resurce = temp_resurs;
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    return 0;
}
int Producer(int *buffer, int buffer_size, int rank_proc, int resurce) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (size == 1 || rank_proc == 0) {
        for (int i = 0; i < buffer_size; i++) {
            if (buffer[i] == -1) {
                buffer[i] = resurce;
                break;
            }
        }
    } else {
        if (rank == 0) {
            int resurce_for_bufer;
            MPI_Recv(&resurce_for_bufer, 1, MPI_INT, rank_proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int i = 0; i < buffer_size; i++) {
                if (buffer[i] == -1) {
                    buffer[i] = resurce;
                    break;
                }
            }
        } else {
            if (rank == rank_proc) {
                MPI_Send(&resurce, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    return 0;
}

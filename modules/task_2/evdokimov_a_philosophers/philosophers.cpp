// Copyright 2019 Evdokimov Artem
#include <mpi.h>
#include <stdio.h>
#include <numeric>
#include <list>
#include "../../../modules/task_2/evdokimov_a_philosophers/philosophers.h"

#define FORK_REQUEST 1
#define FORK_GET 2
#define FORK_SEND 3
#define ALL_FED_UP 4

void philosophersHasLunch(int rank, int size) {
    if (size != 1) {
        int in_buffer;
        int out_buffer;
        int philosopher;
        int check_hunger = 0;
        MPI_Status stat;

        std::list<int> queue;

        bool check = false;
        bool *dinner = new bool[size - 1];
        bool *fork = new bool[size - 1];
        if (rank == 0) {
            for (int i = 0; i < size - 1; i++) {
                dinner[i] = false;
                fork[i] = true;
            }
            dinner[0] = true;

            while (check != true) {
                MPI_Recv(&in_buffer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
                philosopher = stat.MPI_SOURCE;

                if (stat.MPI_TAG == FORK_REQUEST) {
                    if (fork[philosopher % (size - 1)] == true && fork[philosopher - 1] == true) {
                        fork[philosopher % (size - 1)] = false;
                        fork[philosopher - 1] = false;
                        MPI_Send(&out_buffer, 1, MPI_INT, philosopher, FORK_GET, MPI_COMM_WORLD);
                    } else {
                        queue.push_back(philosopher);
                    }
                }
                if (stat.MPI_TAG == FORK_SEND) {
                    fork[philosopher % (size - 1)] = true;
                    fork[philosopher - 1] = true;
                    dinner[philosopher] = true;

                    for (size_t i = 0U; i < queue.size(); ++i) {
                        philosopher = queue.front();
                        if (fork[philosopher % (size - 1)] == true && fork[philosopher - 1] == true) {
                            fork[philosopher % (size - 1)] = false;
                            fork[philosopher - 1] = false;
                            MPI_Send(&out_buffer, 1, MPI_INT, philosopher, FORK_GET, MPI_COMM_WORLD);

                            queue.remove(philosopher);
                        }
                    }
                }
                check_hunger = 0;
                for (int i = 0; i < size; i++) {
                    if (dinner[i] == true)
                        check_hunger++;
                }
                if (check_hunger == size) {
                    check = true;
                    for (int i = 1; i < size; i++) {
                        MPI_Send(&out_buffer, 1, MPI_INT, i, ALL_FED_UP, MPI_COMM_WORLD);
                    }
                }
            }
        } else {
            while (check != true) {
                MPI_Send(&out_buffer, 1, MPI_INT, 0, FORK_REQUEST, MPI_COMM_WORLD);
                MPI_Recv(&in_buffer, 1, MPI_INT, 0, FORK_GET, MPI_COMM_WORLD, &stat);
                MPI_Send(&out_buffer, 1, MPI_INT, 0, FORK_SEND, MPI_COMM_WORLD);
                MPI_Recv(&in_buffer, 1, MPI_INT, 0, ALL_FED_UP, MPI_COMM_WORLD, &stat);
                if (stat.MPI_TAG == ALL_FED_UP) {
                    check = true;
                }
            }
        }
    }
}

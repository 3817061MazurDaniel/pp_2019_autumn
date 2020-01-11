// Copyright 2019 Fedotov Vlad
#include <mpi.h>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <list>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_2/fedotov_v_dining_philosophers/dining_philosophers.h"

#define WANT_EAT 1  // need forks
#define ALLOW_EAT 2  // allow to take two nearest forks
#define END_EAT 3  // ended eating and released forks
#define EATING_CYCLE 1  // each philosopher have to eat EATING_CYCLE times

void philosopher(int rank) {
    // printf("Hello from philosopher %d! \n", rank);
    int in_buffer[1];
    int out_buffer[1];
    MPI_Status stat;

    for (int i = 0; i < EATING_CYCLE; i++) {
        MPI_Send(out_buffer, 1, MPI_INT, 0, WANT_EAT, MPI_COMM_WORLD);
        MPI_Recv(in_buffer, 1, MPI_INT, 0, ALLOW_EAT, MPI_COMM_WORLD, &stat);
        MPI_Send(out_buffer, 1, MPI_INT, 0, END_EAT, MPI_COMM_WORLD);
    }
}

std::vector<bool> waiter(int philosophers_count) {
    // printf("Hello from waiter! \n");
    int in_buffer[1];
    int out_buffer[1];
    int philosopher;
    MPI_Status stat;

    std::list<int> hungry_queue;
    std::vector<int> number_of_eating_cycles(philosophers_count, 0);
    std::vector<bool> all_ate_eating_cycle_times(philosophers_count, false);
    std::vector<bool> fork(philosophers_count, true);  // true if fork is free

    // while exist any philosophers which didn't ate EATING_CYCLE times
    while (true) {
        // if all finished eating, waiter finished his job too
        if (std::find(all_ate_eating_cycle_times.begin(),
            all_ate_eating_cycle_times.end(),
            false) == all_ate_eating_cycle_times.end()) {
            break;
        }

        MPI_Recv(in_buffer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
            MPI_COMM_WORLD, &stat);
        philosopher = stat.MPI_SOURCE;  // detect rank of philosopher,
        // which sent message

        if (stat.MPI_TAG == WANT_EAT) {
            // If left and right forks are free:
            if (fork[philosopher % (philosophers_count)] == true &&
            fork[philosopher - 1] == true) {
                // allow philosopher take these forks and mark them as taken
                fork[philosopher % (philosophers_count)] = false;
                fork[philosopher - 1] = false;
                MPI_Send(out_buffer, 1, MPI_INT, philosopher,
                    ALLOW_EAT, MPI_COMM_WORLD);
            } else  {
                hungry_queue.push_back(philosopher);
                // If forks are not free,
                // forbid philosopher eating and put him to hungry queue
            }
        }

        if (stat.MPI_TAG == END_EAT) {
            // Mark forks as free
            fork[philosopher % (philosophers_count)] = true;
            fork[philosopher - 1] = true;

            // increase number of eating times
            number_of_eating_cycles[philosopher - 1] =
                number_of_eating_cycles[philosopher - 1] + 1;
            // check whether philosopher ate EATING_CYCLE times
            if (number_of_eating_cycles[philosopher - 1] == EATING_CYCLE)
            all_ate_eating_cycle_times[philosopher - 1] = true;

            // looking in hungry queue for philosopher,
            // which has both left and right forks free,
            // and allowing him eating
            if (!hungry_queue.empty()) {
                for (std::list<int>::iterator it = hungry_queue.begin();
                it != hungry_queue.end(); ) {
                    philosopher = *it;
                    if (fork[philosopher % (philosophers_count)] == true &&
                    fork[philosopher-1] == true) {
                        fork[philosopher % (philosophers_count)] = false;
                        fork[philosopher - 1] = false;
                        MPI_Send(out_buffer, 1, MPI_INT, philosopher,
                            ALLOW_EAT, MPI_COMM_WORLD);
                        it = hungry_queue.erase(it);
                        // delete from hungry queue
                    } else {
                        it++;
                    }
                }
            }
        }
        // printVector(number_of_eating_cycles);
        // printVector(all_ate_eating_cycle_times);
    }
    // printf("Waiter finished his work! ");
    return all_ate_eating_cycle_times;
}

void printVector(const std::vector<bool>& vec) {  // for bool vectors
    for (std::vector<bool>::const_iterator it = vec.begin();
        it < vec.end(); it++)
        std::cout << *it << " ";
    std::cout << "Bool vector printed (philosopher ate EATING_CYCLES times)"
        << std::endl;
}

void printVector(const std::vector<int>& vec) {  // for int vectors
    for (std::vector<int>::const_iterator it = vec.begin();
        it < vec.end(); it++)
        std::cout << *it << " ";
    std::cout << "Int vector printed (number of each philosopher's eating)"
        << std::endl;
}

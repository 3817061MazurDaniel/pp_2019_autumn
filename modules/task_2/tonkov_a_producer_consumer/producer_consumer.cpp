// Copyright 2019 Tonkov Alexei
#include <mpi.h>
#include <time.h>
#include <random>
#include <iostream>
#include "../../../modules/task_2/tonkov_a_producer_consumer/producer_consumer.h"

int* generateNumbers(int size) {
    int* numbers = new int[size];

    for (int i = 0; i < size; i++) {
        numbers[i] = i;
    }
    return numbers;
}

void rootsFromNumbers(int* numbers, double* roots, int size) {
    for (int i = 0; i < size; i++) {
        roots[i] = sqrt(numbers[i]);
    }
}

void produceConsume(int* numbers, double* results, int MAX_NUMBERS) {
    int rank, size;
    int number_amount = MAX_NUMBERS;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Check if there's only on processor
    if (rank == 0 && size <= 1) {
        rootsFromNumbers(numbers, results, MAX_NUMBERS);
        return;
    }

    if (rank == 0 && size >= 2) {  // Producer
        // printf("[RANK:0] Sending %i numbers to %i ranks.\n", number_amount, size);

        for (int nextnum = 1 ; nextnum <= number_amount; ++nextnum) {
            // Wait for a worker to become available
            MPI_Status status;
            double result = 0;  // Temp result answer
            MPI_Recv(&result, 1, MPI_DOUBLE, MPI_ANY_SOURCE,
                MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            // If a root was computed
            if (status.MPI_TAG > 0) {
                results[status.MPI_TAG - 1] = result;
            }

            MPI_Send(&(numbers[nextnum - 1]), 1, MPI_INT, status.MPI_SOURCE,
                nextnum, MPI_COMM_WORLD);  // Sends nextnum as a tag (MPI_TAG = Number of job)
        }

        // Send termination signal after getting last job
        int num_terminated = 0;
        for (num_terminated = 0; num_terminated < size - 1; num_terminated++) {
            // Wait for a worker to become available
            MPI_Status status;
            double result = 0;
            MPI_Recv(&result, 1, MPI_DOUBLE, MPI_ANY_SOURCE,
                MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            // If a root was computed
            if (status.MPI_TAG > 0) {
                results[status.MPI_TAG - 1] = result;
            }
            // Send termination signal (tag = 0)
            MPI_Send(&num_terminated, 1, MPI_INT, status.MPI_SOURCE,
                0, MPI_COMM_WORLD);
        }
    } else if (rank > 0 && size >= 2) {  // Consumer
     // Announce myself to producer
        double result = 0;
        MPI_Send(&result, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        bool terminated = false;

        do {
            // Wait for a job
            int num = 0;
            MPI_Status status;
            MPI_Recv(&num, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (status.MPI_TAG == 0) {
                terminated = true;
            } else {
                result = sqrt(num);
                MPI_Send(&result, 1, MPI_DOUBLE, 0, status.MPI_TAG, MPI_COMM_WORLD);
            }
        } while (!terminated);
    }

    MPI_Barrier(MPI_COMM_WORLD);
}

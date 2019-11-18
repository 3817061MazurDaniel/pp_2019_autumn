  // Copyright 2019 Arisova Anastasiia
#include <mpi.h>
#include <queue>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_2/arisova_a_sleeping_barber/sleeping_barber.h"

enum message {start_haircut, end_haircut, want_haircut, empty_seat, no_seat};

void sleep(double t) {
    double time = MPI_Wtime();
    while (MPI_Wtime() - time < t) {
    }
}

void haircut() {
    sleep(0.01);
}

int Barber(int size_queue, int size) {
    MPI_Status status;
    std:: queue <int> reception;

    int counter = 0;  // the number of customers who cut the Barber

    int cust_rank;
    int flag;
    int buff;  // buffer for send letter

      // work or sleep while there are a custimers
    while (counter != (size-1)) {
        // check probably customers
        MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &flag, &status);
        // add customer in reception
        while (flag) {
            MPI_Recv(&cust_rank, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            if ((static_cast<int>(reception.size()) != size_queue)) {
                reception.push(cust_rank);
                buff = empty_seat;
                MPI_Send(&buff, 1, MPI_INT, cust_rank, 0, MPI_COMM_WORLD);
            } else {
                buff = no_seat;
                MPI_Send(&buff, 1, MPI_INT, cust_rank, 0, MPI_COMM_WORLD);
            }
            // recheck probably customers
            MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &flag, &status);
        }

        if (!reception.empty()) {
            cust_rank = reception.front();
            reception.pop();
            buff = start_haircut;
            MPI_Send(&buff, 1, MPI_INT, cust_rank, 0, MPI_COMM_WORLD);
            haircut();
            buff = end_haircut;
            MPI_Send(&buff, 1, MPI_INT, cust_rank, 0, MPI_COMM_WORLD);
            counter++;
        }
    }
    return counter;
}

void Customer(int R) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    double temp = std::rand() / static_cast<double>(RAND_MAX);
    sleep(temp);

    int mess = -1;
    while (mess != end_haircut) {
        MPI_Send(&rank, 1, MPI_INT, R, 0, MPI_COMM_WORLD);
        int flag;
        MPI_Recv(&flag, 1, MPI_INT, R, 0, MPI_COMM_WORLD, &status);
        if (flag == empty_seat) {
            MPI_Recv(&flag, 1, MPI_INT, R, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&mess, 1, MPI_INT, R, 0, MPI_COMM_WORLD, &status);
        } else {
            sleep(temp);
        }
    }
}

int BarberShop(int R, int size_queue) {
    // R - rang process-barber
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int count_cust = 0;
    if (size_queue == 0)
        throw("Incorrect size of queue");
    if (R >= size)
        R = 0;

    if (rank == R)
        // This is process-barber
        count_cust = Barber(size_queue, size);
    else
        // This is process-customer
        Customer(R);

    MPI_Bcast(&count_cust, 1, MPI_INT, R, MPI_COMM_WORLD);
    return count_cust;
}

// Copyright 2019 Pinaev Danil
#include <mpi.h>
#include <random>
#include <ctime>
#include <string>
#include <stdexcept>
#include "../../../modules/task_1/pinaev_d_string_characters_num/string_characters_num.h"

static int offset = 0;

char* getRandomString(int stringSize) {
    std::mt19937 gen;
    gen.seed((unsigned)time(0) + ++offset);

    char* str = new char[stringSize];
    std::string charArr = "abcdefghijklmnaoqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ";

    for (int i = 0; i < stringSize - 1; ++i)
        str[i] = charArr[gen() % charArr.size()];
    str[stringSize - 1] = '\0';

    return str;
}

int getCarNum(const char* str, int stringSize) {
    int ans = 0;

    for (int i = 0; i < stringSize; ++i)
        if ((str[i] >= 'a' && str[i] <= 'z') ||
            (str[i] >= 'A' && str[i] <= 'Z'))
            ++ans;

    return ans;
}

int getParalCarNum(const char* str, int stringSize) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int delta;
    int rem;
    if (size > 1) {
        delta = stringSize / (size - 1);
        rem = stringSize % (size - 1);
    } else {
        delta = 0;
        rem = stringSize;
        }
    const char *global_cstr = str;
    char *local_cstr = new char[delta];

    if (rank == 0) {
        for (int proc = 1; proc < size; ++proc)
                MPI_Send(&global_cstr[rem + (proc - 1) * delta], delta, MPI_CHAR, proc, 0, MPI_COMM_WORLD);
    } else {
        MPI_Status status;
        MPI_Recv(&local_cstr[0], delta, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    }

    int ans = 0;
    int tmp = 0;
    if (rank == 0) {
        ans = getCarNum(global_cstr, rem);
        for (int proc = 1; proc < size; ++proc) {
            MPI_Status status;
            MPI_Recv(&tmp, 1, MPI_INT, proc, 1, MPI_COMM_WORLD, &status);
            ans += tmp;
        }
    } else {
        ans = getCarNum(local_cstr, delta);
        MPI_Send(&ans, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    delete[] local_cstr;

    return ans;
}

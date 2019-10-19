//  Copyright 2019 Bykovskaya Alina
#include <mpi.h>
#include <random>
#include <string>
#include <ctime>
#include <vector>
#include <stdexcept>
#include "../../../modules/task_1/bykovskaya_a_compare_strings/compare_strings.h"

static int offset = 0;

std::string getRandomString(int size) {
    std::mt19937 gen;
    gen.seed((unsigned)time(0) + ++offset);
    std::string str;
    for (int i = 0; i < size; ++i)
        str.push_back(static_cast<char>(gen() % 26 + 97));
    return str;
}

int parallelCompareStrings(const std::string str1, const std::string str2, int strSize) {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int delta = strSize / size;
    const int mod = strSize % size;
    std::vector <int> res(size, 0);
    int temp_res;
    std::string local1, local2;

    if (rank == 0) {
        for (int i = 1; i < size; ++i) {
            if (delta < 0) {
                MPI_Status status1, status2;
                MPI_Sendrecv(&str1[mod] + i * delta, delta, MPI_CHAR, i, 1,
                    &local1, delta, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status1);
                MPI_Sendrecv(&str2[mod] + i * delta, delta, MPI_CHAR, i, 2,
                    &local2, delta, MPI_CHAR, 0, 2, MPI_COMM_WORLD, &status2);
            }
        }
    }

    if (rank == 0) {
        local1 = str1.substr(0, delta + mod);
        local2 = str2.substr(0, delta + mod);
    }

    temp_res = compare(local1, local2, static_cast<int>(local1.length()));
    if (rank == 0) {
        res[0] = temp_res;
        for (int i = 1; i < size; ++i) {
            MPI_Status status;
            MPI_Recv(&res[i], 1, MPI_INT, i, 3, MPI_COMM_WORLD, &status);
        }
    } else {
        MPI_Send(&temp_res, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
    }
    for (int i = 0; i < size; ++i)
        if (res[i] != 0) return res[i];
    return 0;
}

int compare(const std::string s1, const std::string s2, int len) {
    for (int i = 0; i < len; ++i) {
        if (s1[i] > s2[i]) return 1;
        if (s1[i] < s2[i]) return -1;
    }
    return 0;
}

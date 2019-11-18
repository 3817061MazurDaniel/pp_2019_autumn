// Copyright 2019 Khvatov Alexander
#include <mpi.h>

#include <string>
#include <algorithm>
#include <stdexcept>

#include "../../../modules/task_1/khvatov_a_word_count/word_count.h"

bool string_is_correct(std::string str) {
    if (str.empty()) return false;
    if (' ' == str[0]) return false;
    if (' ' == str[str.size() - 1]) return false;
    auto p = [](const char a, const char b) {
        return (a == b) && (a == ' ');
    };
    if (str.end() != std::adjacent_find(str.begin(), str.end(), p)) return false;
    return true;
}

int space_count(std::string str) {
    return std::count(str.begin(), str.end(), ' ');
}

int seq_word_count(std::string str) {
    if (!string_is_correct(str)) throw std::invalid_argument("Invalid string");
    return space_count(str) + 1;
}

int parallel_word_count(std::string str) {
    if (!string_is_correct(str)) throw std::invalid_argument("Invalid string");

    int size = 0, rank = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int lenght = 0, addition = 0;
    if (0 == rank) {
        int string_size = str.size();
        lenght = string_size / size;
        addition = string_size % size;
    }

    MPI_Bcast(&lenght, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::string local_storage("", lenght);
    if (0 == rank) {
        for (int i = 1; i < size; i++) {
            MPI_Send(&str[addition + i * lenght], lenght, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
        local_storage = str.substr(0, lenght + addition);
    } else {
        MPI_Status status;
        MPI_Recv(&local_storage[0], lenght, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    }

    int local_space_count = space_count(local_storage), global_space_count = 0;
    MPI_Reduce(&local_space_count, &global_space_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    global_space_count++;
    return global_space_count;
}

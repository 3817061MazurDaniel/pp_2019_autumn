// Copyright 2019 Babushkin Alexey

#include <mpi.h>

#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "../../../modules/task_1/babushkin_a_word_count/word_count.h"

int wordCountParallel(const std::string ref) {
    //-------------------------------------------------- Initialization
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (ref.empty() && rank == 0) {
        throw ERR_EMPTY_STRING;
    }

    int delta, remainder;

    if (rank == 0) {
        delta = ref.size() / size;
        remainder = ref.size() % size;
    }
    MPI_Bcast(&delta, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&remainder, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    //--------------------------------------------------

    //-------------------------------------------------- Main logic
    if (rank == 0) {
        for (int process = 1; process < size; process++) {
            MPI_Send(&ref[0] + process * delta + remainder,
                     delta,
                     MPI_CHAR,
                     process,
                     0,
                     MPI_COMM_WORLD);
        }
    }

    std::string localStr("", delta);

    if (rank == 0) {
        localStr = ref.substr(0, delta + remainder);
    } else {
        MPI_Status status;
        MPI_Recv(&localStr[0],
                 delta,
                 MPI_CHAR,
                 0,
                 0,
                 MPI_COMM_WORLD,
                 &status);
    }
    //--------------------------------------------------

    //-------------------------------------------------- Collecting results
    int globalWordCount = 0;
    int localWordCount = wordCount(localStr) - 1;

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Reduce(&localWordCount,
               &globalWordCount,
               1,
               MPI_INT,
               MPI_SUM,
               0,
               MPI_COMM_WORLD);
    //--------------------------------------------------

    return globalWordCount + 1;
}

int wordCount(const std::string ref) {
    if (ref.empty()) {
        throw ERR_EMPTY_STRING;
    }

    return std::count(ref.begin(), ref.end(), ' ') + 1;
}

std::string randomString(const int wordCount) {
    std::srand((unsigned)std::time(0));

    auto randomWord = []() -> std::string {
        std::vector<std::string> words = {
            "there", "are", "some", "random",
            "words", "in", "here", "and",
            "a", "boy", "with", "girl",
            "she", "has", "cat", "he",
            "dog", "summer", "much", "fun",
            "please", "don't", "mind", "this" };

        return words[ std::rand() % (words.size() - 1)];
    };

    std::string str("");

    str.append("Beginning ");
    for (int word = 0; word < wordCount - 2; word++) {
        str.append(randomWord());
        str.append(" ");
    }
    str.append("ending.");

    return str;
}

// Copyright 2019 Isaev Ilya
#include "../../../modules/task_2/isaev_bubble_sort/odd_even.h"
#include <algorithm>
#include <utility>
#include <vector>
#include <random>
#include <ctime>
#include <iostream>


std::vector<int> getRandomVector(const int& size) {
    if (size <= 0)
        throw -1;
    std::vector<int> randvec(size);
    std::default_random_engine random;
    random.seed(static_cast<unsigned int>(time(0)));
    for (auto& i : randvec)
        i = random() % 1000;
    return randvec;
}

std::vector<int> sequentialBubbleSort(const std::vector<int>& vec) {
    if (static_cast<int>(vec.size()) == 0) {
        throw -1;
    }
    std::vector<int> sorted(vec);
    for (int i = 0; i < static_cast<int>(vec.size()-1); ++i) {
        for (int j = 0; j < static_cast<int>(vec.size()-1); ++j) {
            if (sorted[j] > sorted[j+1]) {
                std::swap(sorted.at(j), sorted.at(j+1));
            }
        }
    }
    return sorted;
}

std::vector<int> parallelOddEvenSort(std::vector<int> vec, const int& size) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int codeErr = 0;

    if ( ProcRank == 0 ) {
        if (static_cast<int>(vec.size()) == 0 || static_cast<int>(vec.size()) != size)
            codeErr = -1;
        for (int prcs = 1; prcs < ProcNum; prcs++) {
            MPI_Send(&codeErr, 1, MPI_INT, prcs, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Status status;
        MPI_Recv(&codeErr, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    if (codeErr == -1)
        throw -1;

    int ds = size / ProcNum;
    int ms = size % ProcNum;

    std::vector<int> localvec(ds);
    std::vector<int> externalvec(ds);
    vec.resize(size);
    if (ds != 0)
        MPI_Scatter(&vec[0], ds, MPI_INT, &localvec[0], ds, MPI_INT, 0, MPI_COMM_WORLD);
    if (ProcRank == 0 && ms != 0) {
        localvec.insert(localvec.end(), vec.begin()+(size-ms), vec.end());
    }
    std::vector<int> res;
    localvec = sequentialBubbleSort(localvec);
    for (int i = 0; i < ProcNum; ++i) {
        int partner = computePartner(i, ProcRank);
        if (partner < 0 || partner >= ProcNum) {
            continue;
        }
        MPI_Status stats;
        if (ProcRank == 0)
            MPI_Sendrecv(&localvec[0]+ms, ds, MPI_INT, partner, 1, &externalvec[0],
            ds, MPI_INT, partner, 1, MPI_COMM_WORLD, &stats);
        else
            MPI_Sendrecv(&localvec[0], ds, MPI_INT, partner, 1, &externalvec[0],
             ds, MPI_INT, partner, 1, MPI_COMM_WORLD, &stats);
        res.resize(localvec.size()+externalvec.size());
        std::merge(localvec.begin(), localvec.end(), externalvec.begin(), externalvec.end(), res.begin(),
        [](const int& a, const int& b) {return a < b;});
        if (ProcRank < partner) {
            int iter;
            if (ProcRank == 0)
                iter = ms + ds;
            else
                iter = ds;
            localvec = std::vector<int>(res.begin(), res.begin() + iter);
         } else {
            localvec = std::vector<int>(res.end() - ds, res.end());
        }
    }
    if (ProcRank == 0) {
        MPI_Status stat;
        res = localvec;
        for (int i = 1; i < ProcNum; ++i) {
            MPI_Recv(&externalvec[0], ds, MPI_INT, i, 2, MPI_COMM_WORLD, &stat);
            res.insert(res.end(), externalvec.begin(), externalvec.end());
        }
    } else {
        MPI_Send(&localvec[0], ds, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }

    return res;
}

int computePartner(const int& iter, const int& rank) {
    int res;
    if (iter % 2 == 0) {
        if (rank % 2 == 0) {
            res = rank + 1;
        } else {
            res = rank - 1;
        }
    } else {
        if (rank % 2 == 0) {
            res = rank - 1;
        } else {
            res = rank + 1;
        }
    }
    return res;
}

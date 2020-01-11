// Copyright 2019 Kurakin Mikhail
#include "../../../modules/task_3/kurakin_m_batcher_sort/batcher_sort.h"
#include <mpi.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

std::vector<std::pair<int, int>> comparators;

void GatComparators(std::vector<int> procs_up, std::vector<int> procs_down) {
    int procCount = procs_up.size() + procs_down.size();
    if (procCount == 1) return;
    if (procCount == 2) {
        std::pair<int, int> tmpPair{procs_up[0], procs_down[0]};
        comparators.push_back(tmpPair);
        return;
    }
    std::vector<int> procs_up_odd;
    std::vector<int> procs_down_odd;

    std::vector<int> procs_up_even;
    std::vector<int> procs_down_even;
    std::vector<int> procsAll(procCount);
    for (uint32_t i = 0; i < procs_up.size(); i++) {
        if (i % 2) {
            procs_up_even.push_back(procs_up[i]);
        } else {
            procs_up_odd.push_back(procs_up[i]);
        }
    }
    for (uint32_t i = 0; i < procs_down.size(); i++) {
        if (i % 2) {
            procs_down_even.push_back(procs_down[i]);
        } else {
            procs_down_odd.push_back(procs_down[i]);
        }
    }
    GatComparators(procs_up_odd, procs_down_odd);
    GatComparators(procs_up_even, procs_down_even);

    std::copy(procs_up.begin(), procs_up.end(), procsAll.begin());
    std::copy(procs_down.begin(), procs_down.end(),
              procsAll.begin() + procs_up.size());

    for (uint32_t i = 1; i < procsAll.size() - 1; i += 2) {
        std::pair<int, int> tmpPair{procsAll[i], procsAll[i + 1]};
        comparators.push_back(tmpPair);
    }
}

void getOddEvenSortNet(std::vector<int> procs) {
    if (procs.size() < 2) return;
    std::vector<int> procs_up(procs.size() / 2);
    std::vector<int> procs_down(procs.size() / 2 + procs.size() % 2);

    std::copy(procs.begin(), procs.begin() + procs_up.size(), procs_up.begin());
    std::copy(procs.begin() + procs_up.size(), procs.end(), procs_down.begin());

    getOddEvenSortNet(procs_up);
    getOddEvenSortNet(procs_down);

    GatComparators(procs_up, procs_down);
}

void CreateSortNet(int numProcs) {
    std::vector<int> procs(numProcs);
    for (uint32_t i = 0; i < procs.size(); i++) {
        procs[i] = i;
    }
    getOddEvenSortNet(procs);
}

void CreateArray(std::vector<int> *array) {
    if (array->size() < 1) return;

    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));

    for (uint32_t i = 0; i < array->size(); i++) {
        (*array)[i] = gen() % 64001 - 32000;
    }
    return;
}

void BatcherSort(std::vector<int> *arrIn) {
    MPI_Status status;

    int rank, proc_count;
    int size = arrIn->size();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

    if (size <= 0) {
        throw "wrong size";
    }

    CreateSortNet(proc_count);

    int sizeNew =
        size + ((size % proc_count) ? (proc_count - (size % proc_count)) : 0);
    int elems_per_proc_size = sizeNew / proc_count;

    for (int i = size; i < sizeNew; i++) {
        arrIn->push_back(INT16_MIN);
    }

    std::vector<int> elems_res(elems_per_proc_size);
    std::vector<int> elems_cur(elems_per_proc_size);
    std::vector<int> elems_tmp(elems_per_proc_size);

    MPI_Scatter(&(*arrIn)[0], elems_per_proc_size, MPI_INT, &elems_res[0],
                elems_per_proc_size, MPI_INT, 0, MPI_COMM_WORLD);

    std::sort(elems_res.begin(), elems_res.end());

    for (uint32_t i = 0; i < comparators.size(); i++) {
        std::pair<int, int> comparator = comparators[i];
        if (rank == comparator.first) {
            MPI_Send(&elems_res[0], elems_per_proc_size, MPI_INT,
                     comparator.second, 0, MPI_COMM_WORLD);
            MPI_Recv(&elems_cur[0], elems_per_proc_size, MPI_INT,
                     comparator.second, 0, MPI_COMM_WORLD, &status);

            for (int resInd = 0, curInd = 0, tmpInd = 0;
                 tmpInd < elems_per_proc_size; tmpInd++) {
                int res = elems_res[resInd];
                int cur = elems_cur[curInd];
                if (res < cur) {
                    elems_tmp[tmpInd] = res;
                    resInd++;
                } else {
                    elems_tmp[tmpInd] = cur;
                    curInd++;
                }
            }

            elems_res.swap(elems_tmp);
        } else if (rank == comparator.second) {
            MPI_Recv(&elems_cur[0], elems_per_proc_size, MPI_INT,
                     comparator.first, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&elems_res[0], elems_per_proc_size, MPI_INT,
                     comparator.first, 0, MPI_COMM_WORLD);
            int start = elems_per_proc_size - 1;
            for (int resInd = start, curInd = start, tmpInd = start;
                 tmpInd >= 0; tmpInd--) {
                int res = elems_res[resInd];
                int cur = elems_cur[curInd];
                if (res > cur) {
                    elems_tmp[tmpInd] = res;
                    resInd--;
                } else {
                    elems_tmp[tmpInd] = cur;
                    curInd--;
                }
            }
            elems_res.swap(elems_tmp);
        }
    }

    MPI_Gather(&elems_res[0], elems_per_proc_size, MPI_INT, &(*arrIn)[0],
               elems_per_proc_size, MPI_INT, 0, MPI_COMM_WORLD);

    int elDiff = sizeNew - size;
    if (rank == 0 && elDiff) {
        arrIn->erase(arrIn->begin(), arrIn->begin() + elDiff);
    }
    return;
}

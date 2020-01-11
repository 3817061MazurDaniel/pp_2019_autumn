// Copyright 2019 Trenina Elizaveta
#include <mpi.h>

#include <algorithm>
#include <utility>
#include <random>
#include <vector>
#include <ctime>
#include "../../../modules/task_3/trenina_e_shell_batcher_sort/shell_batcher_sort.h"


std::vector<pair> comparators;

std::vector<int> GetRandomVector(int n) {
    if (n < 1) throw;

    std::vector<int> v(n);

    std::mt19937 gen(time(0));
    std::uniform_int_distribution<> uid(0, 100);

    for (int i = 0; i < n; i++) {
        v[i] = uid(gen);
    }

    return v;
}

std::vector<int> Shell(std::vector<int> arr) {
    int n = arr.size();

    for (int step = n / 2; step > 0; step /= 2)
        for (int i = step; i < n; i++)
            for (int j = i - step; j >= 0 && arr[j] > arr[j + step]; j -= step)
                std::swap(arr[j], arr[j + step]);

    return arr;
}


void batcher(int proc_num) {
    std::vector<int> procsVec(proc_num);

    for (int i = 0; i < proc_num; i++) {
        procsVec[i] = i;
    }
    build_Network(procsVec);
}

void build_Network(std::vector<int> procsVec) {
    int size = procsVec.size();

    if (size == 1) return;

    int sizeUp = size / 2;
    int sizeDown = size - sizeUp;

    std::vector<int> UpProcVec(sizeUp);
    std::vector<int> DownProcVec(sizeDown);

    std::copy(procsVec.begin(), procsVec.begin() + sizeUp, UpProcVec.begin());
    std::copy(procsVec.begin() + sizeUp, procsVec.end(), DownProcVec.begin());

    build_Network(UpProcVec);
    build_Network(DownProcVec);

    build_Connection(UpProcVec, DownProcVec);
}

void build_Connection(std::vector<int>UpProcVec, std::vector<int>DownProcVec) {
    int sizeUp = UpProcVec.size();
    int sizeDown = DownProcVec.size();
    int countProc = sizeUp + sizeDown;

    if (countProc == 1)
        return;

    if (countProc == 2) {
        comparators.push_back(pair{ UpProcVec[0], DownProcVec[0] });
        return;
    }

    std::vector<int> upProcVecOdd;
    std::vector<int> upProcVecEven;
    std::vector<int> downProcVecOdd;
    std::vector<int> downProcVecEven;

    std::vector<int> procRes(countProc);

    for (int i = 0; i < sizeUp; i++) {
        if (i % 2) {
            upProcVecEven.push_back(UpProcVec[i]);
        } else {
            upProcVecOdd.push_back(UpProcVec[i]);
        }
    }

    for (int i = 0; i < sizeDown; i++) {
        if (i % 2) {
            downProcVecEven.push_back(DownProcVec[i]);
        } else {
            downProcVecOdd.push_back(DownProcVec[i]);
        }
    }

    build_Connection(upProcVecOdd, downProcVecOdd);
    build_Connection(upProcVecEven, downProcVecEven);

    std::merge(UpProcVec.begin(), UpProcVec.end(),
        DownProcVec.begin(), DownProcVec.end(),
        procRes.begin());

    for (int l = 1; l + 1 < countProc; l += 2)
        comparators.push_back(pair{ procRes[l], procRes[l + 1] });
}


std::vector<int> Shell_Batcher(std::vector<int> globalVec) {
    int globalSize = globalVec.size();

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size <= 0) {
        throw - 1;
    }

    if (size == 1 || size >= globalSize) {
        return Shell(globalVec);
    }

    int globalNewSize = globalSize;
    if (globalSize % size != 0) {
        globalNewSize += size - (globalSize % size);
        for (int i = 0; i < size - (globalSize % size); ++i) {
            globalVec.push_back(999);
        }
    }
    int localSize = globalNewSize / size;

    batcher(size);

    std::vector<int> localVec(localSize);
    std::vector<int> recVec(localSize);
    std::vector<int> tmpVec(localSize);

    MPI_Scatter(&globalVec[0], localSize, MPI_INT,
        &localVec[0], localSize, MPI_INT, 0,
        MPI_COMM_WORLD);

    localVec = Shell(localVec);

    int compsSize = comparators.size();


    for (int i = 0; i < compsSize; i++) {
        pair comp = comparators[i];
        MPI_Status st;

        if (rank == comp.rank1) {
            MPI_Send(&localVec[0], localSize, MPI_INT,
                comp.rank2, 0, MPI_COMM_WORLD);
            MPI_Recv(&recVec[0], localSize, MPI_INT,
                comp.rank2, 0, MPI_COMM_WORLD, &st);

            for (int locIndex = 0, recIndex = 0, tmpIndex = 0;
                tmpIndex < localSize; ++tmpIndex) {
                int local = localVec[locIndex];
                int rec = recVec[recIndex];
                if (local < rec) {
                    tmpVec[tmpIndex] = local;
                    ++locIndex;
                } else {
                    tmpVec[tmpIndex] = rec;
                    ++recIndex;
                }
            }
            std::swap(localVec, tmpVec);
        } else if (rank == comp.rank2) {
            MPI_Recv(&recVec[0], localSize, MPI_INT,
                comp.rank1, 0, MPI_COMM_WORLD, &st);
            MPI_Send(&localVec[0], localSize, MPI_INT,
                comp.rank1, 0, MPI_COMM_WORLD);

            int startIndex = localSize - 1;
            for (int locIndex = startIndex, recIndex = startIndex,
                tmpIndex = startIndex; tmpIndex >= 0; --tmpIndex) {
                int local = localVec[locIndex];
                int rec = recVec[recIndex];
                if (local > rec) {
                    tmpVec[tmpIndex] = local;
                    --locIndex;
                } else {
                    tmpVec[tmpIndex] = rec;
                    --recIndex;
                }
            }
            std::swap(localVec, tmpVec);
        }
    }

    MPI_Gather(&localVec[0], localSize, MPI_INT,
        &globalVec[0], localSize, MPI_INT, 0,
        MPI_COMM_WORLD);

    if (rank == 0 && globalNewSize != globalSize) {
        for (int i = 0; i < size - (globalSize % size); i++) {
            globalVec.pop_back();
        }
    }

    return globalVec;
}



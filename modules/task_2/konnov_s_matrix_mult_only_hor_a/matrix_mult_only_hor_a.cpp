// Copyright 2019 Konnov Sergey
#include "../../../modules/task_2/konnov_s_matrix_mult_only_hor_a/matrix_mult_only_hor_a.h"
#include <mpi.h>
#include <iostream>
#include <random>
#include <vector>
#include <utility>

std::vector<int> matrix_mult_parallel(const std::vector<int>& aa, const std::vector<int>&bb, int msize) {
    std::vector<int> a, b = bb;
    std::vector<int> ans;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    b.resize(msize * msize);
    if (rank == 0) {
        for (int i = 0; i < msize; i++)
            for (int j = 0; j < i; j++)
                std::swap(b[i*msize+j], b[j*msize+i]);
    }
    MPI_Bcast(b.data(), msize * msize, MPI_INT, 0, MPI_COMM_WORLD);

    int portion = msize * ((rank < msize % size)?msize / size + 1:msize / size);
    a.resize(portion + 1);

    if (rank == 0) {
        int teq = portion;
        for (int i = 1; i < size; i++) {
            if (teq == msize*msize)
                break;
            int teqportion = msize*((i < msize % size)?msize / size + 1:msize / size);
            MPI_Send(aa.data()+teq, teqportion, MPI_INT, i, 0, MPI_COMM_WORLD);
            teq+=teqportion;
        }
        for (int i = 0; i < portion; i++) {
            a[i] = aa[i];
        }
    } else if (portion > 0) {
        MPI_Status status;
        MPI_Recv(a.data(), portion, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }

    ans.resize(portion);
    for (int i = 0; i < portion / msize; i++)
        for (int j = 0; j < msize; j++) {
             ans[i * msize + j] = 0;
             for (int k = 0; k < msize; k++)
                 ans[i * msize + j] += a[i * msize + k] * b[j * msize + k];
        }

    std::vector<int> ans2;
    if (rank == 0)
        ans2.resize(msize*msize);
    std::vector<int> recvcounts(size), displs(size);
    for (int i = 0 ; i < size; i++) {
        recvcounts[i] =  msize * ((i < msize % size)?msize / size + 1:msize / size);
        if (i != 0) {
            displs[i] = displs[i-1] + recvcounts[i-1];
        } else {
            displs[i] = 0;
        }
    }
    MPI_Gatherv(ans.data(), portion, MPI_INT,
                ans2.data(), recvcounts.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);
    return ans2;
}

std::vector<int> matrix_mult_sequential(const std::vector<int>& a, const std::vector<int>&b, int msize) {
    std::vector<int> ans(msize * msize);
    for (int i = 0; i < msize; i++)
        for (int j = 0; j < msize; j++) {
            ans[i*msize+j] = 0;
            for (int k = 0; k < msize; k++)
                ans[i * msize + j]+=a[i * msize + k] * b[k * msize + j];
        }
    return ans;
}

std::vector<int> generate_matrix(int size, int time) {
    std::mt19937 mt(time);
    std::uniform_int_distribution<int> rand(-10, 10);
    std::vector<int> v;
    v.resize(size*size);
    for (int i = 0; i < size*size; i++)
        v[i] = rand(mt);
    return v;
}

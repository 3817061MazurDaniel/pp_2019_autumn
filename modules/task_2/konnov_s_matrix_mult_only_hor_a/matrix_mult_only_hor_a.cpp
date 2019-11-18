// Copyright 2019 Konnov Sergey
#include "../../../modules/task_2/konnov_s_matrix_mult_only_hor_a/matrix_mult_only_hor_a.h"
#include <mpi.h>
#include <iostream>
#include <random>
#include <vector>

std::vector<int> matrix_mult_parallel(const std::vector<int>& aa, const std::vector<int>&bb, int msize) {
    std::vector<int> a = aa, b = bb;
    std::vector<int> ans;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    b.resize(msize * msize);
    MPI_Bcast(b.data(), msize * msize, MPI_INT, 0, MPI_COMM_WORLD);
    int portion = msize * ((rank < msize % size)?msize / size + 1:msize / size);
    if (rank != 0)
        a.resize(portion*size);
    if (rank == 0) {
        int teq = portion;
        for (int i = 1; i < size; i++) {
            if (teq == msize*msize)
                break;
            int teqportion = msize*((i < msize % size)?msize / size + 1:msize / size);
            MPI_Send(a.data()+teq, teqportion, MPI_INT, i, 0, MPI_COMM_WORLD);
            teq+=teqportion;
        }
    } else if (portion > 0) {
        MPI_Status status;
        MPI_Recv(a.data(), portion, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }
    if (rank == 0) {
        ans.resize(msize * msize);
    } else {
        ans.resize(msize * portion);
    }
    for (int i = 0; i < portion / msize; i++)
        for (int j = 0; j < msize; j++) {
             ans[i * msize + j] = 0;
             for (int k = 0; k < msize; k++)
                 ans[i * msize + j] += a[i * msize + k] * b[k * msize + j];
          }
    if (rank == 0) {
        int teq = portion;
        for (int i = 1; i < size; i++) {
            if (teq == msize * msize)
                break;
            int teqportion = msize * ((i < msize % size)?msize / size + 1:msize / size);
            MPI_Status status;
            MPI_Recv(ans.data() + teq, teqportion, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            teq+=teqportion;
        }
    } else if (portion > 0) {
        MPI_Send(&ans[0], portion, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    return ans;
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

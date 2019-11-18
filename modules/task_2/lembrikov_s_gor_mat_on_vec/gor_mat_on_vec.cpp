// Copyright 2019 Lembrikov Stepan
#include <../../../modules/task_2/lembrikov_s_gor_mat_on_vec/gor_mat_on_vec.h>
#include <algorithm>
#include <vector>

std::vector<int> getIdentityVector(int n) {
    std::vector <int> a(n);
    for (int i = 0; i < n; i++) {
        a[i] = 1;
    }
    return a;
}

std::vector<int> getRandomVector(int n) {
    std::mt19937 engine;
    std::vector<int> a(n);
    engine.seed(n);
    int k = 1;
    for (int i = 0; i < n; i++) {
        k = -k;
        a[i] = engine() % 100;
        a[i] = a[i] * k;
    }
    return a;
}

std::vector<int> getConstVector(int n, int c) {
    std::vector <int> a(n);
    for (int i = 0; i < n; i++) {
        a[i] = c;
    }
    return a;
}

std::vector <int> MatrixOnVector(const std::vector <int> &matr, int cols,
    int rows, const std::vector <int> &vect) {
    int size;
    int rank;
    int ost;
    int k;
    int flag = 0;
    int per = vect.size();
    if ((per != cols) || (cols < 0) || (rows < 0))
        throw "wrong size of matrix a or vector b";
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ost = rows % size;
    if (rows < size) {
        size = 1;
        ost = 0;
        flag = -1;
    }
    k = rows / size;

    std::vector <int> res_part_vec(k);
    std::vector <int> res_vec(rows);

    if (rank == 0) {
        res_part_vec.resize(k + ost);
        for (int i = 0; i < cols * (k + ost); i += cols) {
            for (int j = 0; j < cols; j++)
                res_part_vec[i / cols] += matr[i + j] * vect[j];
        }
    }

    int index_rank = 1;
    int index_vec = 0;
    if (flag == 0) {
        while (index_rank < size) {
            if (rank == index_rank) {
                index_vec = 0;
                for (int i = cols * (ost + index_rank * k);
                    i < cols * (ost + (index_rank + 1) * k); i += cols) {
                    for (int j = 0; j < cols; j++) {
                        res_part_vec[index_vec] += matr[i + j] * vect[j];
                    }
                    index_vec++;
                }
            }
            index_rank++;
        }
    }

    if (flag == 0) {
        MPI_Gather(&res_part_vec[0], k, MPI_INT,
            &res_vec[ost], k, MPI_INT, 0, MPI_COMM_WORLD);
        if (rank == 0) {
            for (int i = 0; i < k + ost; i++)
                res_vec[i] = res_part_vec[i];
        }
        return res_vec;
    } else {
        if (rank == 0) {
            return res_part_vec;
        } else {
            std::vector <int> excep_vec = { -1 };
            return excep_vec;
        }
    }
}

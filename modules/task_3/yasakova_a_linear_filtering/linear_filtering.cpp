// Copyright 2019 Yasakova Anastasia
#include <mpi.h>
#include <vector>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>
#include "../../../modules/task_3/yasakova_a_linear_filtering/linear_filtering.h"



std::vector <int> getImage(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        throw - 1;
    }
    std::vector <int> Matrix(rows * cols);
    std::default_random_engine random;
    random.seed(static_cast<unsigned int>(std::time(0)));
    for (int i = 0; i < rows * cols; i++) {
        Matrix[i] = random() % 256;
    }
    return Matrix;
}


int CountPixel(const std::vector<int> mtx, const std::vector<int> mask, int i, int j, int rows, int cols) {
    int p = 0;
    int mask_size = sqrt(mask.size());
    for (int l = - mask_size / 2; l <= mask_size / 2; ++l) {
        for (int k = - mask_size / 2; k <= mask_size / 2; ++k) {
            int x = i + l;
            int y = j + k;
            if (x * cols + y >= cols * rows) {
                x = i;
                y = j;
            }
            if (x < 0 || x > rows - 1) {
                x = i;
            }
            if (y < 0 || y > cols - 1) {
                y = j;
            }
            p += mtx[x * cols + y] * mask[(l + 1) * 3 + k + 1];
        }
    }
    int norm = std::accumulate(mask.begin(), mask.end(), 0);
    if (norm != 0) {
        p = std::min(p / norm, 255);
    }
    return p;
}

std::vector <int> ParallelLinearFilter(const std::vector <int> mask, std::vector <int> mtx, int rows, int cols) {
    if (static_cast<int>(mtx.size()) != rows * cols) {
        throw - 1;
    }
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int mask_size = sqrt(mask.size());
    int i_ = 0, j_ = 0;
    int rows_ = 0, cols_ = 0;
    int sqrt_size = floor(sqrt(size));
    float sqrt_size_rest = sqrt(size) - sqrt_size;
    int amount_cols = cols / sqrt_size, rest_cols = cols % sqrt_size;
    int amount_rows = 0, rest_rows = 0;
    if (sqrt_size_rest == 0) {
        amount_rows = rows / sqrt_size;
        rest_rows = rows % sqrt_size;
    } else if (fabs(sqrt_size - sqrt_size_rest) < 1) {
        amount_rows = rows - static_cast<int>((rows / (sqrt_size + 0.5))) * sqrt_size;
        rest_rows = static_cast<int>(rows / (sqrt_size + 0.5)) - amount_rows;
    } else {
        amount_rows = rows / (sqrt_size + 1);
        rest_rows = rows % (sqrt_size + 1);
    }
    int rule = 0;
    if ((size - (rows / amount_rows - 1) * sqrt_size) == 0) {
        rule = 1;
    } else {
        rule = cols / (size - (rows / amount_rows - 1) * sqrt_size) >= mask_size ? 1 : 0;
    }
    if (amount_rows >= mask_size && amount_cols >= mask_size && size > 1 && rule == 1) {
        MPI_Bcast(&mtx[0], rows * cols, MPI_INT, 0, MPI_COMM_WORLD);
        int count = size - (rows / amount_rows - 1) * sqrt_size;
        if (rank >= (rows / amount_rows - 1) * sqrt_size && sqrt_size_rest != 0 && size > 2) {
            sqrt_size = size - (rows / amount_rows - 1) * sqrt_size;
            amount_cols = cols / count;
            rest_cols = cols % count;
            i_ = (rows / amount_rows - 1) * amount_rows;
            j_ = (sqrt_size - (size - rank)) * amount_cols;
            if ((size - rank) % sqrt_size < rest_cols) {
                cols_ = 1;
            }
            if (j_ > 0 && rest_cols > 0) {
                j_ += (sqrt_size - (size - rank) - 1);
                if ((sqrt_size - (size - rank) - 1) == 0) {
                    j_ += rest_cols;
                }
            }
            rows_ = amount_rows + rest_rows;
            cols_ += amount_cols;
        } else {
            i_ = (rank / (sqrt_size)) * amount_rows;
            if (rank % sqrt_size < rest_cols) {
                j_ = rank % sqrt_size;
                cols_ = 1;
            } else {
                j_ = rest_cols;
            }
            j_ += (rank % sqrt_size) * amount_cols;
            if (rank >= size - count) {
                rows_ = rest_rows;
            }
            rows_ += amount_rows;
            cols_ += amount_cols;
        }
    } else {
        i_ = 0;
        j_ = 0;
        rows_ = rows;
        cols_ = cols;
    }

    std::vector<int> res(rows * cols);

    if (rank == 0) {
        for (int i = i_; i < i_ + rows_; i++) {
            for (int j = j_; j < j_ + cols_; j++) {
                 res[i * cols + j] = CountPixel(mtx, mask, i, j, rows, cols);
            }
        }
        if (amount_rows >= mask_size && amount_cols >= mask_size && size > 1 && rule == 1) {
            int proc = 0;
            int amount_cols_ = amount_cols, rest_cols_ = rest_cols;
            MPI_Status status;
            int sqrt_size_ = sqrt_size;
            int i1 = 0, l_ = 0;
            int flag_ = 0, flag = 1;
            if (sqrt_size == 1) {
                i1++;
                flag = 0;
                flag_ = 1;
                l_++;
            }
            for (int i = i1; i < rows / amount_rows; i++) {
                for (int j = 0; j < amount_rows; j++) {
                    if (proc * sqrt_size + flag_ >= (rows / amount_rows - 1) * sqrt_size
                        && sqrt_size_rest != 0 && size > 2) {
                        amount_cols_ = cols / (size - (rows / amount_rows - 1) * sqrt_size);
                        rest_cols_ = cols % (size - (rows / amount_rows - 1) * sqrt_size);
                        sqrt_size_ =  size - (rows / amount_rows - 1) * sqrt_size;
                    }
                    int l1 = 0;
                    if (flag == 1) {
                        l1++;
                    }
                    for (int l = l1; l < sqrt_size_; l++) {
                        int rest = 0, c = 0;
                        if (l >= rest_cols_) {
                            rest = rest_cols_;
                        } else {
                            rest = l;
                            c = 1;
                        }
                        MPI_Recv(&res[(i  * amount_rows + j) * cols + l * amount_cols_ + rest],
                           amount_cols_ + c, MPI_INT, proc * sqrt_size+ l + l_, 1, MPI_COMM_WORLD, &status);
                    }
                }
                flag = 0;
                amount_cols_ = amount_cols;
                rest_cols_ = rest_cols;
                sqrt_size_ = sqrt_size;
                proc++;
                if (proc == size - 1) {
                    break;
                }
            }
            amount_cols_ = amount_cols;
            rest_cols_ = rest_cols;
            for (int i = 0; i < rest_rows; i++) {
                for (int l = 0; l < size - (rows / amount_rows - 1) * sqrt_size; l++) {
                    if (proc * sqrt_size + l >=(rows / amount_rows - 1)* sqrt_size && sqrt_size_rest != 0 && size > 2) {
                        amount_cols_ = cols / (size - (rows / amount_rows - 1) * sqrt_size);
                        rest_cols_ = cols % (size - (rows / amount_rows - 1) * sqrt_size);
                    }
                    int rest = 0, c = 0;;
                    if (l >= rest_cols_) {
                        rest = rest_cols_;
                    } else {
                        rest = l;
                        c = 1;
                    }
                    MPI_Recv(&res[(rows - rest_rows + i) * cols + l * amount_cols_ + rest],
                       amount_cols_ + c, MPI_INT, (rows / amount_rows - 1) * sqrt_size + l, 1, MPI_COMM_WORLD, &status);
                }
            }
        }
    } else {
        std::vector<int> res_;
        if (amount_rows >= mask_size && amount_cols >= mask_size && rule == 1) {
            for (int i = i_; i < i_ + rows_; i++) {
                for (int j = j_; j <j_ + cols_; j++) {
                     res_.push_back(CountPixel(mtx, mask, i, j, rows, cols));
                }
                MPI_Send(&res_[0], cols_, MPI_INT, 0, 1, MPI_COMM_WORLD);
                res_.clear();
            }
        }
    }
    return res;
}


std::vector <int> SequentialLinearFilter(const std::vector <int> mask, std::vector <int> mtx, int rows, int cols) {
    if (static_cast<int>(mtx.size()) != rows * cols) {
        throw - 1;
    }
    std::vector <int> res;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            res.push_back(CountPixel(mtx, mask, i, j, rows, cols));
        }
    }
    return res;
}

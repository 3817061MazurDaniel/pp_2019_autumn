// Copyright 2019 Kudryashov Nikita

#include "../../../modules/task_3/kudryashov_n_fox_algorithm/fox_algorithm.h"
#include <cmath>
#include <vector>

bool is_square(int size) {
    int i = 1;
    while (i <= size) {
        if (i * i == size) {
            return true;
        } else {
            i++;
        }
    }
    return false;
}

std::vector<double> subtask_matr_mult(std::vector<double> a_block, unsigned int a_block_size,
        std::vector<double> b_block, unsigned int b_block_size) {
    std::vector<double> c(a_block_size * a_block_size);
    for (unsigned int i = 0; i < a_block_size; i++) {
        for (unsigned int j = 0; j < a_block_size; j++) {
            c.at(i * a_block_size + j) = 0.0;
            for (unsigned int k = 0; k < a_block_size; k++) {
                c.at(i * a_block_size + j) += a_block.at(i * a_block_size + k) * b_block.at(k * a_block_size + j);
            }
        }
    }
    return c;
}

std::vector<double> fox_mult(std::vector<double> a, unsigned int a_size, std::vector<double> b, unsigned int b_size) {
    if (a.size() != b.size()) {
        throw "Matrix must have equal size.";
    }

    bool matr_size_changed = false;
    int save_size;
    int size, rank, size_inside;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (!is_square(size) && static_cast<unsigned int>(size) < a_size * a_size) {
        int i = 1;
        while (i*i < size) {
            i++;
        }
        i--;
        size_inside = i * i;
    } else {
        if (static_cast<unsigned int>(size) < a_size * a_size) {
            size_inside = size;
        } else {
            size_inside = a_size * a_size;
        }
    }

    if (size_inside == 1) {
        std::vector<double> c;
        return c = subtask_matr_mult(a, a_size, b, b_size);
    }

    save_size = static_cast<int>(a_size);
    std::vector<double> temp_a(save_size * save_size, 0.0);
    std::vector<double> temp_b(save_size * save_size, 0.0);
    if (a_size % ((unsigned int)sqrt(size_inside)) != 0) {
    // Creating matrix of new size and surrounding by zeros'.
        matr_size_changed = true;
        a_size = a_size + (unsigned int)sqrt(size_inside) - a_size % ((unsigned int)sqrt(size_inside));
        // Saving A.
        for (int i = 0; i < save_size; i++) {
            for (int j = 0; j < save_size; j++) {
                temp_a.at(i*save_size+j) = a.at(i*save_size+j);
            }
        }
        a.resize(a_size * a_size);
        for (int i = 0; i < save_size; i++) {
            for (int j = 0; j < save_size; j++) {
                a.at(i*a_size+j) = temp_a.at(i*save_size+j);
            }
        }
        // Left: not full.
        for (int i = save_size; i < static_cast<int>(a_size); i++) {
            for (int j = 0; j < save_size; j++) {
                a.at(i*a_size+j) = 0.0;
            }
        }
        // Right: full.
        for (int i = 0; i < static_cast<int>(a_size); i++) {
            for (int j = save_size; j < static_cast<int>(a_size); j++) {
                a.at(i*a_size+j) = 0.0;
            }
        }
        // Finished creating new A.

        // Saving B.
        for (int i = 0; i < save_size; i++) {
            for (int j = 0; j < save_size; j++) {
                temp_b.at(i*save_size+j) = b.at(i*save_size+j);
            }
        }
        b.resize(a_size * a_size);

        for (int i = 0; i < save_size; i++) {
            for (int j = 0; j < save_size; j++) {
                b.at(i*a_size+j) = temp_b.at(i*save_size+j);
            }
        }
        // Left: not full.
        for (int i = save_size; i < static_cast<int>(a_size); i++) {
            for (int j = 0; j < save_size; j++) {
                b.at(i*a_size+j) = 0.0;
            }
        }
        // Right: full.
        for (int i = 0; i < static_cast<int>(a_size); i++) {
            for (int j = save_size; j < static_cast<int>(a_size); j++) {
                b.at(i*a_size+j) = 0.0;
            }
        }
        // Finished creating new B.
    }

    int block_size = a_size / static_cast<int>(sqrt(size_inside));

    int color, key = rank;
    MPI_Comm inside;
    if (rank < size_inside) {
        color = 1;
    } else {
        color = 0;
    }
    MPI_Comm_split(MPI_COMM_WORLD, color, key, &inside);
    MPI_Comm_rank(inside, &rank);

    if (color == 1) {
        std::vector<double> a_block(block_size * block_size);
        std::vector<double> b_block(block_size * block_size);
        std::vector<double> c;

        // Initializing process
        int blocks_per_row = a_size / block_size;
        int blocks_per_column = a_size / block_size;
        for (int i = 0; i < block_size; i++) {
            for (int j = 0; j < block_size; j++) {
                a_block.at(block_size * i + j) = a.at(rank / blocks_per_row * block_size * a_size +
                    ((rank + rank / blocks_per_row) % blocks_per_row) * block_size + i * a_size + j);
                b_block.at(block_size * i + j) = b.at(((rank + (rank % blocks_per_column) * blocks_per_column) %
                    (blocks_per_column * blocks_per_column))
                        / blocks_per_row * block_size * a_size + rank % blocks_per_row * block_size + i * a_size + j);
                // b_block form operation is the same as a_block form operation, but matrix b is transposed by blocks
            }
        }
        std::vector<double> res(a_size * a_size);
        for (int i = 0; i < static_cast<int>(a_size); i++) {
            for (int j = 0; j < static_cast<int>(a_size); j++) {
                res.at(i * a_size + j) = 0.0;
            }
        }
        // Initialization over

        std::vector<double> temp_1(block_size * block_size), temp_2(block_size * block_size);
        // Subtask mult and move
        for (int k = 0; k < blocks_per_row; k++) {
            // Subtask mult and collecting the result
            c = subtask_matr_mult(a_block, block_size, b_block, block_size);
            if (rank == 0) {
                std::vector<double> recv_arr(block_size * block_size);
                for (int i = 0; i < block_size; i++) {
                    for (int j = 0; j < block_size; j++) {
                        res.at(i * a_size + j) += c.at(i * block_size +j);
                    }
                }

                int coord_i;
                int coord_j;
                for (int i = 1; i < size_inside; i++) {
                    coord_i = i / blocks_per_row;
                    coord_j = i % blocks_per_column;
                    MPI_Recv(&recv_arr[0], block_size * block_size, MPI_DOUBLE, i, 0, inside, &status);
                    for (int d1 = 0; d1 < block_size; d1++) {
                        for (int d2 = 0; d2 < block_size; d2++) {
                            res.at(block_size * a_size * coord_i + block_size*coord_j + d1*a_size + d2) +=
                                recv_arr.at(d1 * block_size + d2);
                        }
                    }
                }
            } else {
                MPI_Send(&c[0], block_size * block_size, MPI_DOUBLE, 0, 0, inside);
            }

            // Move
            // A blocks move
            int additive_a_1, additive_a_2;

            // Getting lesser than first in a row
            if (rank - 1  < blocks_per_row * (rank / blocks_per_row)) {
                additive_a_1 = blocks_per_row;
            } else {
                additive_a_1 = 0;
            }

            // Getting bigger than last in a row
            if (rank + 1  > blocks_per_row * (rank / blocks_per_row) + blocks_per_row - 1) {
                additive_a_2 = blocks_per_row;
            } else {
                additive_a_2 = 0;
            }

            MPI_Sendrecv(&a_block[0], block_size * block_size, MPI_DOUBLE, rank - 1 + additive_a_1, 0,
                &temp_1[0], block_size * block_size, MPI_DOUBLE, rank + 1 - additive_a_2, 0, inside, &status);
            for (int h = 0; h < block_size; h++) {
                for (int t = 0; t < block_size; t++) {
                    a_block[h * block_size + t] = temp_1[h * block_size + t];
                }
            }

            // B blocks move
            int dest;

            // Mod for negative number
            if (rank - blocks_per_column >= 0) {
                dest = (rank - blocks_per_column) % (blocks_per_row * blocks_per_column);
            } else {
                dest = blocks_per_column * blocks_per_column + (rank - blocks_per_column);
            }

            MPI_Sendrecv(&b_block[0], block_size * block_size, MPI_DOUBLE, dest, 1,
                &temp_2[0], block_size * block_size, MPI_DOUBLE,
                (rank + blocks_per_column) % (blocks_per_row * blocks_per_column), 1, inside, &status);
            for (int h = 0; h < block_size; h++) {
                for (int t = 0; t < block_size; t++) {
                    b_block[h * block_size + t] = temp_2[h * block_size + t];
                }
            }
        }

        MPI_Barrier(inside);
        MPI_Comm_free(&inside);

        if (rank == 0) {
            if (matr_size_changed) {
                std::vector<double> res_new(save_size * save_size);
                // Restoring result.
                for (int i = 0; i < save_size; i++) {
                    for (int j = 0; j < save_size; j++) {
                        res_new.at(i*save_size+j) = res.at(i*a_size+j);
                    }
                }

                // Restoring A.
                a.resize(save_size * save_size);
                for (int i = 0; i < save_size; i++) {
                    for (int j = 0; j < save_size; j++) {
                        a.at(i*save_size+j) = temp_a.at(i*save_size+j);
                    }
                }

                // Restoring B.
                b.resize(save_size * save_size);
                for (int i = 0; i < save_size; i++) {
                    for (int j = 0; j < save_size; j++) {
                        b.at(i*save_size+j) = temp_b.at(i*save_size+j);
                    }
                }

                return res_new;
            } else {
                return res;
            }
        } else {
            return a;
        }
    }
    MPI_Barrier(inside);
    MPI_Comm_free(&inside);
    return a;
}

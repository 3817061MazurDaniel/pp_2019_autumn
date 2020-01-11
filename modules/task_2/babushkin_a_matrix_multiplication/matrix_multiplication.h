// Copyright 2019 Babushkin Aleksey

#ifndef MODULES_TASK_2_BABUSHKIN_A_MATRIX_MULTIPLICATION_MATRIX_MULTIPLICATION_H_
#define MODULES_TASK_2_BABUSHKIN_A_MATRIX_MULTIPLICATION_MATRIX_MULTIPLICATION_H_

#include <vector>

#define ERR_INCORRECT_SIZE -1

typedef std::vector<int> matrix;

/**
 * @brief Make matrix __rows * __columns size with random numbers.
 * @param __rows Row count in matrix.
 * @param __columns Column count in matrix.
 */
matrix getRandomMatrix(const int __rows, const int __columns);

/**
 * @brief Multiply two matricies using parallelizm.
 * @param __left Left matrix.
 * @param __lRowCount Left matrix row count
 * @param __lColumnCount Left matrix column count. Right matrix must have equal
 * number of rows.
 * @param __rColumnCount Right matrix column count.
 * @param __right Right matrix.
 */
matrix multiply(const matrix __left, const matrix __right,
                const int __lRowCount, const int __lColumnCount,
                const int __rColumnCount);

/**
 * @brief Multiply two matricies.
 * @param __left Left matrix.
 * @param __lRowCount Left matrix row count
 * @param __lColumnCount Left matrix column count. Right matrix must have equal
 * number of rows.
 * @param __rColumnCount Right matrix column count.
 * @param __right Right matrix.
 */
matrix multiply_seq(const matrix __left, const matrix __right,
                    const int __lRowCount, const int __lColumnCount,
                    const int __rColumnCount);

int currentTimeNanoseconds();

#endif  // MODULES_TASK_2_BABUSHKIN_A_MATRIX_MULTIPLICATION_MATRIX_MULTIPLICATION_H_

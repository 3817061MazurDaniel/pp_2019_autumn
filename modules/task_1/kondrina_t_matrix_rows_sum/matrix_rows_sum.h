// Copyright 2019 Kondrina Tatyana

#ifndef MODULES_TASK_1_KONDRINA_T_MATRIX_ROWS_SUM_MATRIX_ROWS_SUM_H_
#define MODULES_TASK_1_KONDRINA_T_MATRIX_ROWS_SUM_MATRIX_ROWS_SUM_H_

#include <valarray>

std::valarray<int> rowsSumSeq(std::valarray<int> const matrix,
                              int const columns,
                              int const rows);

std::valarray<int> rowsSum(std::valarray<int> const matrix,
                           int const columns,
                           int const rows);

std::valarray<int> randomMatrix(int const columns, int const rows);

/**
 * @brief Check if two valarrays are equal.
 * @param _leftArr First vallarray.
 * @param _rightArr Second valarray.
 * @return Returns true, when each element of arrays is equal. Arrays must be equal in size.
*/
bool valarraysEquality(std::valarray<int> _leftArr, std::valarray<int> _rightArr);


#endif  // MODULES_TASK_1_KONDRINA_T_MATRIX_ROWS_SUM_MATRIX_ROWS_SUM_H_

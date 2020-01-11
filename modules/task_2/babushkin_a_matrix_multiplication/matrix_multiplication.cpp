// Copyright 2019 Babushkin Aleksey

#include "../../../modules/task_2/babushkin_a_matrix_multiplication/matrix_multiplication.h"

#include <mpi.h>

#include <algorithm>
#include <ctime>
#include <random>
#include <vector>

#define loop(__iterator, __iterStart, __border) \
  for (auto __iterator = __iterStart; __iterator < __border; ++__iterator)

matrix multiply_row(const matrix, const matrix, const int, const int);

matrix getRandomMatrix(const int __rows, const int __columns) {
  if (__rows <= 0 || __columns <= 0) {
    throw ERR_INCORRECT_SIZE;
  }

  matrix __result(__rows * __columns);

  std::mt19937 __randomGen(currentTimeNanoseconds() * __rows * __columns);
  std::uniform_int_distribution<> __distr(1, 50);

  std::generate(__result.begin(), __result.end(),
                [&__distr, &__randomGen]() { return __distr(__randomGen); });

  return __result;
}

matrix multiply(const matrix __left, const matrix __right,
                const int __lRowCount, const int __lColumnCount,
                const int __rColumnCount) {
  int __rank, __CommSize;
  MPI_Comm_size(MPI_COMM_WORLD, &__CommSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &__rank);

  /*------------------------- Error check -------------------------*/

  if (__lColumnCount != (signed)__right.size() / __rColumnCount) {
    throw ERR_INCORRECT_SIZE;
  }
  if (__lRowCount < __CommSize || __rColumnCount < __CommSize) {
    throw ERR_INCORRECT_SIZE;
  }

  /*----------------------------------------------------------------*/

  const int __colsPerProc = __rColumnCount / __CommSize;
  matrix __rowBuffer(__lColumnCount);
  matrix __columnBuffer(__lColumnCount * __colsPerProc);
  matrix __resultMatrix(__lRowCount * __rColumnCount);
  matrix __multResult;

  /*------------------------- Send columns -------------------------*/

  if (__rank == 0) {
    std::vector<int> __rColumns;

    loop(__column, 0, __rColumnCount) {
      loop(__row, 0, __lColumnCount) {
        __rColumns.push_back(__right[__row * __rColumnCount + __column]);
      }
    }

    MPI_Scatter(&__rColumns[0], __lColumnCount * __colsPerProc, MPI_INT,
                &__columnBuffer[0], __lColumnCount * __colsPerProc, MPI_INT, 0,
                MPI_COMM_WORLD);
  } else {
    MPI_Scatter(NULL, 0, MPI_INT, &__columnBuffer[0],
                __lColumnCount * __colsPerProc, MPI_INT, 0, MPI_COMM_WORLD);
  }

  /*----------------------------------------------------------------*/

  /*----------------- Send rows and calculate ----------------------*/

  for (auto __iter = 0; __iter < __lRowCount; ++__iter) {
    /*------------------------- Clearence ----------------------------*/

    __multResult.clear();

    /*----------------------------------------------------------------*/
    if (__rank == 0) {
      loop(__proc, 1, __CommSize) {
        auto __index = (__iter + __proc) % __lRowCount * __lColumnCount;
        MPI_Send(&__left[__index], __lColumnCount, MPI_INT, __proc, __proc,
                 MPI_COMM_WORLD);
      }

      __rowBuffer =
          matrix(__left.begin() + __iter % __lRowCount * __lColumnCount,
                 __left.begin() + __iter % __lRowCount * __lColumnCount +
                     __lColumnCount);
    } else {
      MPI_Recv(&__rowBuffer[0], __lColumnCount, MPI_INT, 0, __rank,
               MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    /*------------------------- Calcule and collect -------------------*/

    __multResult = multiply_row(__rowBuffer, __columnBuffer, __lColumnCount,
                                __colsPerProc);
    MPI_Barrier(MPI_COMM_WORLD);

    if (__rank == 0) {
      loop(__proc, 1, __CommSize) {
        auto __index = ((__iter + __proc) % __lRowCount) * __rColumnCount +
                       __proc * __colsPerProc;
        MPI_Recv(&__resultMatrix[__index], __colsPerProc, MPI_INT, __proc,
                 __proc, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }

      auto index = (__iter % __lRowCount) * __rColumnCount;
      __resultMatrix.erase(__resultMatrix.begin() + index, __resultMatrix.begin() + index + __colsPerProc);
      __resultMatrix.insert(__resultMatrix.begin() + index, __multResult.begin(), __multResult.end());
    } else {
      MPI_Send(&__multResult[0], __colsPerProc, MPI_INT, 0, __rank,
               MPI_COMM_WORLD);
    }

    /*----------------------------------------------------------------*/
  }

  /*----------------------------------------------------------------*/

  return __resultMatrix;
}

matrix multiply_seq(const matrix __left, const matrix __right,
                    const int __lRowCount, const int __lColumnCount,
                    const int __rColumnCount) {
  if (__lColumnCount != (signed)__right.size() / __rColumnCount) {
    throw ERR_INCORRECT_SIZE;
  }

  matrix __result(__lRowCount * __rColumnCount);
  loop(__row, 0, __lRowCount) {
    loop(__column, 0, __rColumnCount) {
      loop(__rRow, 0, __lColumnCount) {
        __result[__row * __rColumnCount + __column] +=
            __left[__row * __lColumnCount + __rRow] *
            __right[__rRow * __rColumnCount + __column];
      }
    }
  }

  return __result;
}

matrix multiply_row(const matrix __left, const matrix __right,
                    const int __lCols, const int __rCols) {
  matrix __result;
  loop(__rIndex, 0, __rCols) {
    int __mResult = 0;
    loop(__lIndex, 0, __lCols) {
      __mResult += __left[__lIndex] * __right[__rIndex * __lCols + __lIndex];
    }

    __result.push_back(__mResult);
  }

  return __result;
}

int currentTimeNanoseconds() {
  std::time_t __time = std::time(0);
  std::tm* __timeSpec = std::localtime(&__time);

  return __timeSpec->tm_sec * 1e+9;
}

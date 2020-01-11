// Copyright 2019 Babushkin Aleksey

#include "../../../modules/task_3/babushkin_a_sparse_matrix/sparse_matrix.h"

#include <mpi.h>

#include <algorithm>
#include <ctime>
#include <random>
#include <vector>

/*------------------------------ Class members ------------------------------*/

SparseMatrix::SparseMatrix(const std::vector<double>& A, const int& t_columns,
                           const int& t_rows)
    : m_columns(t_columns), m_rows(t_rows) {
  m_non_zero = 0;
  m_row_ptr.push_back(0);
  for (auto row = 0; row < t_rows; row++) {
    int non_zero = std::count_if(A.begin() + row * t_columns,
                                 A.begin() + row * t_columns + t_columns,
                                 [](double val) { return val != 0; });
    m_row_ptr.push_back(m_row_ptr.back() + non_zero);

    for (auto column = 0; column < t_columns; column++) {
      if (A.at(row * t_columns + column) != 0) {
        m_col_ind.push_back(column);
      }
    }

    m_non_zero += non_zero;
  }

  m_val.resize(m_non_zero);
  std::copy_if(A.begin(), A.end(), m_val.begin(),
               [](double val) { return val != 0; });
}

const std::vector<double> SparseMatrix::makeVector() const {
  std::vector<double> result;
  for (auto row = 0; row < m_rows; row++) {
    for (auto col = 0; col < m_columns; col++) {
      if (m_row_ptr.at(row + 1) - m_row_ptr.at(row) == 0) {
        result.push_back(0.0);
      } else {
        bool pushed = false;
        for (auto i = m_row_ptr.at(row); i <= m_row_ptr.at(row + 1) - 1; i++) {
          if (m_col_ind.at(i) == col) {
            result.push_back(m_val.at(i));
            pushed = true;
          }
        }
        if (!pushed) {
          result.push_back(0.0);
        }
      }
    }
  }

  return result;
}

const std::vector<double> SparseMatrix::makeColumn(const int& t_col) {
  std::vector<double> result;
  for (auto row = 0; row < m_rows; row++) {
    if (m_row_ptr.at(row + 1) - m_row_ptr.at(row) == 0) {
      result.push_back(0.0);
    } else {
      bool pushed = false;
      for (auto i = m_row_ptr.at(row); i <= m_row_ptr.at(row + 1) - 1; i++) {
        if (m_col_ind.at(i) == t_col) {
          result.push_back(m_val.at(i));
          pushed = true;
        }
      }
      if (!pushed) {
        result.push_back(0.0);
      }
    }
  }

  return result;
}

const std::vector<double> operator*(const SparseMatrix& A,
                                    const SparseMatrix& B) {
  if (A.m_columns != B.m_rows) {
    throw ERR_INCORRECT_SIZE;
  }

  std::vector<double> result;
  for (auto row = 0; row < A.m_rows; row++) {
    for (auto col = 0; col < B.m_columns; col++) {
      result.push_back(0.0);
      for (auto i = A.m_row_ptr.at(row); i <= A.m_row_ptr.at(row + 1) - 1;
           i++) {
        if (B.m_row_ptr.at(A.m_col_ind.at(i) + 1) -
                B.m_row_ptr.at(A.m_col_ind.at(i)) ==
            0) {
          continue;
        }
        if (B.m_col_ind.at(B.m_row_ptr.at(A.m_col_ind.at(i))) == col) {
          result.back() +=
              A.m_val.at(i) * B.m_val.at(B.m_row_ptr.at(A.m_col_ind.at(i)));
        }
      }
    }
  }

  return result;
}

const std::vector<double> operator*(const SparseMatrix& A,
                                    const std::vector<double>& B) {
  if (A.m_columns != (signed)B.size()) {
    throw ERR_INCORRECT_SIZE;
  }

  std::vector<double> result;
  for (auto row = 0; row < A.m_rows; row++) {
    result.push_back(0.0);

    for (auto i = A.m_row_ptr.at(row); i <= A.m_row_ptr.at(row + 1) - 1; i++) {
      result.back() += A.m_val.at(i) * B.at(A.m_col_ind.at(i));
    }
  }

  return result;
}

/*---------------------------------------------------------------------------*/

/*---------------------------- non-Class members ----------------------------*/

std::vector<double> multiply(SparseMatrix* A, SparseMatrix* B) {
  int rank, comm_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  if (A->m_rows < comm_size) {
    if (rank == 0) {
      return *A * *B;
    } else {
      return std::vector<double>();
    }
  }

  if (comm_size == 1) {
    return *A * *B;
  }

  /*---------------------------- Matricies cast ----------------------------*/

  MPI_Bcast(&A->m_non_zero, 1, MPI_INT, 0, MPI_COMM_WORLD);
  if (rank != 0) {
    A->m_val.resize(A->m_non_zero);
    A->m_col_ind.resize(A->m_non_zero);
  }
  MPI_Bcast(&A->m_val[0], A->m_non_zero, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&A->m_col_ind[0], A->m_non_zero, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&A->m_row_ptr[0], A->m_rows + 1, MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Bcast(&B->m_non_zero, 1, MPI_INT, 0, MPI_COMM_WORLD);
  if (rank != 0) {
    B->m_val.resize(B->m_non_zero);
    B->m_col_ind.resize(B->m_non_zero);
  }
  MPI_Bcast(&B->m_val[0], B->m_non_zero, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&B->m_col_ind[0], B->m_non_zero, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&B->m_row_ptr[0], B->m_rows + 1, MPI_INT, 0, MPI_COMM_WORLD);

  /*------------------------------------------------------------------------*/

  int rows_per_proc = A->m_rows / comm_size;
  int rem = A->m_rows % comm_size;
  int left_bound = rank * rows_per_proc;
  int right_bound = (rank + 1) * rows_per_proc;
  if (rank == comm_size - 1) {
    right_bound = A->m_rows;
  }

  /*---------------------------- Multiplication ----------------------------*/

  std::vector<double> local_result;
  for (auto row = left_bound; row < right_bound; row++) {
    for (auto col = 0; col < B->m_columns; col++) {
      local_result.push_back(0.0);
      for (auto i = A->m_row_ptr.at(row); i <= A->m_row_ptr.at(row + 1) - 1;
           i++) {
        if (B->m_row_ptr.at(A->m_col_ind.at(i) + 1) -
                B->m_row_ptr.at(A->m_col_ind.at(i)) ==
            0) {
          continue;
        }
        if (B->m_col_ind.at(B->m_row_ptr.at(A->m_col_ind.at(i))) == col) {
          local_result.back() +=
              A->m_val.at(i) * B->m_val.at(B->m_row_ptr.at(A->m_col_ind.at(i)));
        }
      }
    }
  }

  /*------------------------------------------------------------------------*/

  /*------------------------------ Collection ------------------------------*/

  std::vector<double> global_result;
  if (rank == 0) {
    global_result.resize(A->m_rows * B->m_columns);
  }

  if (rank == 0) {
    MPI_Gather(&local_result[0], rows_per_proc * B->m_columns, MPI_DOUBLE,
               &global_result[0], rows_per_proc * B->m_columns, MPI_DOUBLE, 0,
               MPI_COMM_WORLD);
  } else {
    MPI_Gather(&local_result[0], rows_per_proc * B->m_columns, MPI_DOUBLE,
               MPI_IN_PLACE, 0, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  if (rank == comm_size - 1) {
    MPI_Send(&local_result[local_result.size() - (rem * B->m_columns) - 1],
             rem * B->m_columns + 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }
  if (rank == 0) {
    MPI_Recv(&global_result[(A->m_rows - rem) * B->m_columns - 1],
             rem * B->m_columns + 1, MPI_DOUBLE, comm_size - 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
  }

  /*------------------------------------------------------------------------*/

  return global_result;
}

std::vector<double> randomSparseMatrix(const int& t_columns, const int& t_rows,
                                       const int& t_left_bound,
                                       const int& t_right_bound) {
  if (t_rows <= 0 || t_columns <= 0) {
    throw ERR_INCORRECT_SIZE;
  }

  std::vector<double> m_result(t_rows * t_columns);

  std::random_device r;
  std::seed_seq seed{r()};
  std::mt19937 m_randomGen(seed);
  std::uniform_int_distribution<int> m_distr(t_left_bound, t_right_bound);

  std::generate(m_result.begin(), m_result.end(),
                [&m_distr, &m_randomGen, &t_left_bound]() {
                  auto m_elem = m_distr(m_randomGen);
                  return m_elem <= t_left_bound + 4 ? m_elem : 0;
                });

  return m_result;
}

/*---------------------------------------------------------------------------*/

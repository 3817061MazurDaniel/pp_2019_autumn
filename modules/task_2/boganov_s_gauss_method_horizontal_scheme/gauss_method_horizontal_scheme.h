  // Copyright 2019 Boganov Sergey
#ifndef MODULES_TASK_2_BOGANOV_S_GAUSS_METHOD_HORIZONTAL_SCHEME_GAUSS_METHOD_HORIZONTAL_SCHEME_H_
#define MODULES_TASK_2_BOGANOV_S_GAUSS_METHOD_HORIZONTAL_SCHEME_GAUSS_METHOD_HORIZONTAL_SCHEME_H_

#include <vector>

bool Ax_b_check(const std::vector <double> &a, unsigned int n, const std::vector<double> &x);
std::vector<double> getRandomMatrix(unsigned int n, double min_rand, double max_rand);
std::vector<double> getSequentialGauss(std::vector<double> matrix, unsigned int n);
double getSequentialMaxElem(const std::vector<double> &matrix, unsigned int n, unsigned int row0, unsigned int rows,
unsigned int i, unsigned int *index);
int getParallelIndexMaxElem(const std::vector<double> &matrix, unsigned int row_count, int rank, int size,
unsigned int i, unsigned int n);
int findRank(unsigned int n, unsigned int size, unsigned int delta, unsigned int rem, unsigned int index);
void getSwapedRows(unsigned int n, unsigned int ind, int rank, int size,
unsigned int index_max_elem, std::vector<double>* matrix);
void getParallelNormalizedRows(unsigned int n, unsigned int i, int rank, int size,
std::vector<double>* matrix, unsigned int row_count);
void getParallelSubtractRows(unsigned int n, unsigned int ind, int rank, int size,
std::vector<double>* matrix, unsigned int row_count);
std::vector<double> getParallelGauss(std::vector<double> matrix, unsigned int n);

#endif  // MODULES_TASK_2_BOGANOV_S_GAUSS_METHOD_HORIZONTAL_SCHEME_GAUSS_METHOD_HORIZONTAL_SCHEME_H_

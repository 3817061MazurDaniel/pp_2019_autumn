// Copyright 2019 Vlasov Andrey
#ifndef MODULES_TASK_1_VLASOV_A_MIN_VALUE_ROWS_MIN_VALUE_ROWS_H_
#define MODULES_TASK_1_VLASOV_A_MIN_VALUE_ROWS_MIN_VALUE_ROWS_H_

#include <vector>

std::vector<int> getRandomMatrix(int m, int n);
std::vector<int> MinValuesRowsParallel(const std::vector<int> &matrix, int m, int n);
std::vector<int> MinValuesRowsSequential(const std::vector<int> &matrix, int m, int n);

#endif  // MODULES_TASK_1_VLASOV_A_MIN_VALUE_ROWS_MIN_VALUE_ROWS_H_

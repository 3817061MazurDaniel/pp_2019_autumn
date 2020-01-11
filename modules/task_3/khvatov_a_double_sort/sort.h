// Copyright 2019 Khvatov Alexander
#ifndef MODULES_TASK_3_KHVATOV_A_DOUBLE_SORT_SORT_H_
#define MODULES_TASK_3_KHVATOV_A_DOUBLE_SORT_SORT_H_

bool is_sorted(double* source, int size);
void pass(double* source, double* dest, int size, int offset);
void last_pass(double* source, double* dest, int size, int offset);
void ordered_merge(double* source1, int size1, double* source2, int size2, double* dest);
void seq_sort(double* source, int size);
void par_sort(double** source, int size);

#endif  // MODULES_TASK_3_KHVATOV_A_DOUBLE_SORT_SORT_H_

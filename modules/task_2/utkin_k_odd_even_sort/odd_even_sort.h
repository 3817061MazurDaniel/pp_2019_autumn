// Copyright 2019 Utkin Konstantin
#ifndef MODULES_TASK_2_UTKIN_K_ODD_EVEN_SORT_ODD_EVEN_SORT_H_
#define MODULES_TASK_2_UTKIN_K_ODD_EVEN_SORT_ODD_EVEN_SORT_H_

#include <vector>

std::vector<int> getRandVec(int size);

std::vector<int> seqSort(const std::vector<int>& vec);
std::vector<int> parSort(const std::vector<int>& globalVec);

std::vector<int> compareSplitMin(int rank1, int rank2,
    const std::vector<int>& localVec, const int delta,
    const int remain);
std::vector<int> compareSplitMax(int rank1, int rank2,
    const std::vector<int>& localVec, const int delta,
    const int remain);

#endif  // MODULES_TASK_2_UTKIN_K_ODD_EVEN_SORT_ODD_EVEN_SORT_H_

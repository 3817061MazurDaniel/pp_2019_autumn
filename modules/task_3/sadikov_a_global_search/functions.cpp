// Copyright 2019 Sadikov Artem
#include <cmath>
#include <vector>
#include "../../../modules/task_3/sadikov_a_global_search/functions.h"

double func1(double x) {
    return std::sqrt(x) * std::sin(x);
}

double func2(double x) {
    return std::sqrt(1 + 3 * std::pow(std::cos(x), 2)) + std::cos(10 * x);
}

double func3(double x) {
    return 100 + std::cos(x * 10);
}

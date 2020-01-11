// Copyright 2019 Konnov Sergey
#ifndef MODULES_TASK_3_KONNOV_S_MONTE_CARLO_MULTIPLE_INTEGRATION_MONTE_CARLO_MULTIPLE_INTEGRATION_H_
#define MODULES_TASK_3_KONNOV_S_MONTE_CARLO_MULTIPLE_INTEGRATION_MONTE_CARLO_MULTIPLE_INTEGRATION_H_
#include <vector>
#include <functional>

double monteCarloMultipleIntegraion(const std::vector<double>& lower_limits,
                                    const std::vector<double>& upper_limits,
                                    int count_of_dots,
                                    const std::function<double(const std::vector<double>&)>& f,
                                    int seed = -1);
double monteCarloMultipleIntegraionSequentional(const std::vector<double>& lower_limits,
                                                const std::vector<double>& upper_limits,
                                                int count_of_dots,
                                                const std::function<double(const std::vector<double>&)>& f,
                                                int seed = -1);
#endif  // MODULES_TASK_3_KONNOV_S_MONTE_CARLO_MULTIPLE_INTEGRATION_MONTE_CARLO_MULTIPLE_INTEGRATION_H_

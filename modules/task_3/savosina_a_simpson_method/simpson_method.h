// Copyright 2019 Savosina
#ifndef MODULES_TASK_3_SAVOSINA_A_SIMPSON_METHOD_SIMPSON_METHOD_H_
#define MODULES_TASK_3_SAVOSINA_A_SIMPSON_METHOD_SIMPSON_METHOD_H_

#include <vector>
#include <utility>
#include <functional>

double calculateIntegral(const std::vector<std::pair<double, double>>& _scope, const size_t& _n,
const std::function<double(const std::vector<double>&)>& f, const size_t& _nRec = 2);

double calculateIntegralSequential(const std::vector<std::pair<double, double>>& _scope, const size_t& _n,
const std::function<double(const std::vector<double>&)>& f, const size_t& _nRec = 2);

double calculateRecusionLevel(std::vector<double> _fixedVar, size_t _levelNumber,
const std::vector<std::pair<double, double>>& _scope,
const std::function<double(const std::vector<double>&)>& f, const size_t& _n);

void scopeCheck(const std::vector<std::pair<double, double>>& scope);

#endif  // MODULES_TASK_3_SAVOSINA_A_SIMPSON_METHOD_SIMPSON_METHOD_H_

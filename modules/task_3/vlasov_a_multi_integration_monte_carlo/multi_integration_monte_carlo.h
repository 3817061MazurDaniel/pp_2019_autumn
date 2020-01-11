// Copyright 2019 Vlasov Andrey
#ifndef MODULES_TASK_3_VLASOV_A_MULTI_INTEGRATION_MONTE_CARLO_MULTI_INTEGRATION_MONTE_CARLO_H_
#define MODULES_TASK_3_VLASOV_A_MULTI_INTEGRATION_MONTE_CARLO_MULTI_INTEGRATION_MONTE_CARLO_H_

#include <mpi.h>
#include <vector>

double getIntegralMonteCarloSequential(double(*f)(std::vector<double>), const std::vector<double>& a,
  const std::vector<double>& b, int n);

double getIntegralMonteCarloParallel(double(*f)(std::vector<double>), const std::vector<double>& a,
  const std::vector<double>& b, int n);

#endif  // MODULES_TASK_3_VLASOV_A_MULTI_INTEGRATION_MONTE_CARLO_MULTI_INTEGRATION_MONTE_CARLO_H_

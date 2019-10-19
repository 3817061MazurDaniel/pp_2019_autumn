// Copyright 2019 Kriukov Dmitry
#ifndef MODULES_TASK_1_KRIUKOV_D_MONTE_KARLO_INTEGRATION_MONTE_CARLO_INTEGRATION_H_
#define MODULES_TASK_1_KRIUKOV_D_MONTE_KARLO_INTEGRATION_MONTE_CARLO_INTEGRATION_H_


double monteCarloIntegration(double lower_limit, double upper_limit, double(*pfunc)(double), int point_count);

#endif  // MODULES_TASK_1_KRIUKOV_D_MONTE_KARLO_INTEGRATION_MONTE_CARLO_INTEGRATION_H_

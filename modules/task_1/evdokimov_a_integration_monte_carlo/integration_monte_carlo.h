// Copyright 2019 Evdokimov Artem
#ifndef MODULES_TASK_1_EVDOKIMOV_A_INTEGRATION_MONTE_CARLO_INTEGRATION_MONTE_CARLO_H_
#define MODULES_TASK_1_EVDOKIMOV_A_INTEGRATION_MONTE_CARLO_INTEGRATION_MONTE_CARLO_H_

double getResultOfIntegral(
    double upper_limit,
    double lower_limit,
    double count_of_dots,
    double (*function)(double));

#endif  // MODULES_TASK_1_EVDOKIMOV_A_INTEGRATION_MONTE_CARLO_INTEGRATION_MONTE_CARLO_H_


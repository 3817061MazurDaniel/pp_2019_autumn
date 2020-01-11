// Copyright 2019 Sadikov Artem
#include "../../../modules/task_3/sadikov_a_global_search/global_search.h"
#include <mpi.h>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

bool equal(std::vector<double> actual, std::vector<double> expected) {
    for (int i = 0; i < 2; i++) {
        if (!(std::fabs(actual[i] - expected[i]) < 1e-2)) return false;
    }
    return true;
}

std::vector<double> split_vec(double a, double b, int proc) {
    double range = static_cast<double>(b - a) / static_cast<double>(proc + 1);
    std::vector<double> res(proc);

    for (int i = 0; i < proc; i++) {
        res[i] = a + (i + 1) * range;
    }

    return res;
}

double calc_M(double r, std::vector<double> intervals,
                std::vector<double> test_points) {
    double m = 0.0;
    double exp;

    for (int i = 1; i < static_cast<int>(intervals.size()); i++) {
        exp = static_cast<double>(fabs(test_points[i] - test_points[i - 1])) /
                static_cast<double>(intervals[i] - intervals[i - 1]);
        m = std::max(exp, m);
    }

    return m > 0.0 ? r * m : 1.0;
}

double interval_R(double a, double b, double x_l, double x_r,
                    double func_x_l, double func_x_r, double M) {
    double delta = x_r - x_l;
    double res;
    if (a == x_l) {
        double res = 2 * (delta) - 4 *
                    (static_cast<double>(func_x_r) / static_cast<double>(M));
        return res;
    } else if (b == x_r) {
        double res = 2 * (delta) - 4 *
                    (static_cast<double>(func_x_l) / static_cast<double>(M));
        return res;
    }

    res = x_r - x_l +
         static_cast<double>(std::pow(func_x_r - func_x_l, 2)) /
         static_cast<double>(std::pow(M, 2) * delta) - 2 *
         (static_cast<double>(func_x_r + func_x_l) / static_cast<double>(M));

    return res;
}

double calc_new_point(double x_l, double x_r, double func_x_l,
                        double func_x_r, double M, int max_num, int size) {
    if (max_num == 1 || max_num == size) {
        return static_cast<double>(x_r + x_l) / 2.0;
    }

    double res = (static_cast<double>(x_r + x_l) / 2.0) -
                 (static_cast<double>(func_x_r - func_x_l) / (2.0 * M));
    return res;
}

std::vector<double> get_res(double a, double b, double r, double error,
                            double (*func)(double)) {
    if (b <= a) throw "WRONG_LEN";
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> recv_points(size);
    std::vector<int> max_intervals(size);
    std::vector<double> all_points(size + 2);
    std::vector<double> func_points;
    std::vector<double> split(size);
    std::vector<double> res(2);
    double res_point;
    double res_func_point;
    double M;
    int iter = 1;
    std::vector<double> desc_vec;
    double norm;

    if (rank == 0) {
        split = split_vec(a, b, size);
        all_points[0] = a;

        for (int i = 0; i < size; i++) {
            all_points[i + 1] = split[i];
        }

        all_points[size + 1] = b;
    }

    while (1) {
        if (rank == 0) {
            func_points.resize(all_points.size());
            for (int i = 0; i < static_cast<int>(func_points.size()); i++) {
                func_points[i] = func(all_points[i]);
            }

            M = calc_M(r, all_points, func_points);
        }

        MPI_Bcast(&M, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            desc_vec.resize(size + iter);
            for (int i = 0; i < size + iter; i++) {
                desc_vec[i] = interval_R(a, b,
                                         all_points[i], all_points[i + 1],
                                         func_points[i], func_points[i + 1],
                                         M);
            }

            for (int i = 0; i < size; i++) {
                max_intervals[i] = 0;
            }

            double m;
            for (int i = 0; i < size + iter; i++) {
                m = *std::min_element(max_intervals.begin(),
                                     max_intervals.end());
                for (int j = 0; j < size; j++) {
                    if (max_intervals[j] == m &&
                        (desc_vec[i] > max_intervals[j] || max_intervals[j] == 0.0)) {
                            max_intervals[j] = i + 1;
                            break;
                    }
                }
            }
            std::sort(max_intervals.begin(), max_intervals.end());

            norm = 0.0;
            for (int i = 0; i < size; i++) {
                double dVal = all_points[max_intervals[i]] -
                                all_points[max_intervals[i] - 1];
                if (dVal > norm) {
                    norm = dVal;
                    res_point = all_points[max_intervals[i]];
                    res_func_point = func_points[max_intervals[i]];
                }
            }
        }

        MPI_Bcast(&norm, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        if (norm <= error) break;


        int max_num;

        if (rank == 0) {
            for (int proc = 1; proc < size; proc++) {
                MPI_Send(&max_intervals[0] + proc, 1, MPI_INT, proc, 0,
                         MPI_COMM_WORLD);
            }
        }

        if (rank == 0) {
            max_num = max_intervals[0];
        } else {
            MPI_Status status;
            MPI_Recv(&max_num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
                     &status);
        }

        if (rank == 0) {
            for (int proc = 1; proc < size; proc++) {
                MPI_Send(&all_points[0] + max_intervals[proc] - 1, 2,
                         MPI_DOUBLE, proc, 0, MPI_COMM_WORLD);
            }
        }

        std::vector<double> local_range(2);

        if (rank == 0) {
            for (int i = 0; i < 2; i++) {
                local_range[i] = all_points[max_num - 1 + i];
            }
        } else {
            MPI_Status status;
            MPI_Recv(&local_range[0], 2, MPI_DOUBLE, 0, 0,
                     MPI_COMM_WORLD, &status);
        }

        if (rank == 0) {
            for (int proc = 1; proc < size; proc++) {
                MPI_Send(&func_points[0] + max_intervals[proc] - 1, 2,
                         MPI_DOUBLE, proc, 0, MPI_COMM_WORLD);
            }
        }

        std::vector<double> local_func_range(2);

        if (rank == 0) {
            for (int i = 0; i < 2; i++) {
                local_func_range[i] = func_points[max_num - 1 + i];
            }
        } else {
            MPI_Status status;
            MPI_Recv(&local_func_range[0], 2, MPI_DOUBLE, 0, 0,
                     MPI_COMM_WORLD, &status);
        }

        double new_point = calc_new_point(local_range[0], local_range[1],
                                            local_func_range[0],
                                            local_func_range[1],
                                            M, max_num, size + iter);

        MPI_Gather(&new_point, 1, MPI_DOUBLE, &recv_points[0], 1, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);

        if (rank == 0) {
            for (int i = 0; i < size; i++) {
                all_points.emplace_back(recv_points[i]);
            }
            std::sort(all_points.begin(), all_points.end());
            iter++;
        }
    }

    if (rank == 0) {
        res = { res_func_point, res_point };
    }

    MPI_Bcast(&res[0], 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    return res;
}

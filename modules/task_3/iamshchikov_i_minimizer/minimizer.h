// Copyright 2019 Iamshchikov Ivan
#ifndef MODULES_TASK_3_IAMSHCHIKOV_I_MINIMIZER_MINIMIZER_H__
#define MODULES_TASK_3_IAMSHCHIKOV_I_MINIMIZER_MINIMIZER_H__

#include <mpi.h>
#include <cmath>
#include <map>
#include <queue>
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

struct point {
    double x;
    double y;
    point(double _x, double _y);
};

struct result {
    double x;
    double y;
    double z;
};

struct characteristics {
    double z;
    double R;
    characteristics(double _z, double _R);
    explicit characteristics(double _R);
};

struct interval {
    std::pair<double, double> first_point;
    std::pair<double, double> second_point;
    characteristics _ch;
    interval(std::pair<double, double> _f_point,
             std::pair<double, double> _s_point, double _R);
};

struct CompareR {
    bool operator()(interval const& i1, interval const& i2) {
        return i1._ch.R < i2._ch.R;
    }
};

class One_Dimensional_Minimizer {
 protected:
    std::map<double, characteristics>::iterator left_point;
    std::map<double, characteristics>::iterator right_point;
    result res;
    double a;
    double b;
    double curr_x;
    double(*function) (double _x, double _y);
    double eps;
    int N_max;
    double r_p;
    double min_interval_length;
    double m;
    double M_Max;
    std::map<double, characteristics>* values;
    std::priority_queue<interval, std::vector<interval>, CompareR>* pq;
    void reset();
    void go_Next_Interval();
    void go_new_left_interval(double new_point);
    double get_M();
    double get_m();
    double get_R();
    void calculate_R(double new_point, double new_m);
    double get_new_point(interval i);
    void insert_to_map(double _y, double _z, double _R);
    void compare_interval_len(double new_point);
    void compare_M(double new_point);
    void delete_containers();
    void do_first_iteration();

 public:
    One_Dimensional_Minimizer(double _a, double _b, double _curr_x,
                              double(*f)(double x, double y),
                              double _eps = 0.001, int _N_max = 500,
                              double _r_par = 2.0);
    void set_experiment(const double _a, const double _b, double _curr_x,
                        double(*f)(double x, double y),
                        const double _eps = 0.001, const int _N_max = 500,
                        const double _r_par = 2.0);
    result solve();
};

class Minimizer : public One_Dimensional_Minimizer {
 private:
    bool valIsMin;
    double curr_y;
    double upper_y;
    double(*function) (double _x, double _y);
    int procrank;
    int procnum;
    void do_first_iteration(One_Dimensional_Minimizer* odm, result* tmp_res);
    void share_interval();

 public:
    bool isMin();
    int get_procnum();
    int get_rank();
    void insert_to_map(double _x, double _y, double _z, double _R);
    result get_result();
    Minimizer(double _a, double _b, double _curr_y, double _upper_y,
              double(*f)(double x, double y), double _eps = 0.001,
              int _N_max = 500, double _r_par = 2.0);
    void set_experiment(const double _a, const double _b, double _curr_y,
                        double _upper_y, double(*f)(double x, double y),
                        const double _eps = 0.001, const int _N_max = 500,
                        const double _r_par = 2.0);
    void solve();
};

#endif  // MODULES_TASK_3_IAMSHCHIKOV_I_MINIMIZER_MINIMIZER_H__

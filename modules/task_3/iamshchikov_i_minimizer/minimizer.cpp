// Copyright 2019 Iamshchikov Ivan

#include <map>
#include <queue>
#include <utility>
#include <vector>
#include "../../../modules/task_3/iamshchikov_i_minimizer/minimizer.h"

point::point(double _x, double _y): x(_x), y(_y) {}

characteristics::characteristics(double _z, double _R) : z(_z), R(_R) {}

characteristics::characteristics(double _R) : R(_R) {}

interval::interval(std::pair<double, double> _f_point,
                   std::pair<double, double> _s_point, double _R) :
                   first_point(_f_point), second_point(_s_point), _ch(_R) {}

One_Dimensional_Minimizer::One_Dimensional_Minimizer(double _a, double _b,
    double _curr_x, double(*f)(double x, double y), double _eps, int _N_max,
    double _r_par) : a(_a), b(_b), curr_x(_curr_x), function(f),
eps(_eps), N_max(_N_max), r_p(_r_par) {
    if (a > b)
        throw "b is a right border, must be more than a";
    values = new std::map<double, characteristics>;
    pq = new std::priority_queue<interval, std::vector<interval>, CompareR>;
}

void One_Dimensional_Minimizer::reset() {
    left_point = values->begin();
    right_point = left_point; right_point++;
}

void One_Dimensional_Minimizer::go_Next_Interval() {
    left_point++; right_point++;
}

void One_Dimensional_Minimizer::go_new_left_interval(double new_point) {
    right_point = values->find(new_point);
    left_point = right_point; left_point--;
}

double One_Dimensional_Minimizer::get_M() {
    return std::abs(((*right_point).second.z - (*left_point).second.z) /
                    ((*right_point).first - (*left_point).first));
}

double One_Dimensional_Minimizer::get_m() {
    if (M_Max > 0) {
        return r_p * M_Max;
    } else if (M_Max == 0) {
        return 1;
    } else {
        throw - 1;
    }
}

double One_Dimensional_Minimizer::get_R() {
    double tmp = m * ((*right_point).first - (*left_point).first);
    return tmp + (std::pow((*right_point).second.z - (*left_point).second.z, 2)
    / tmp) - 2 * ((*right_point).second.z + (*left_point).second.z);
}

void One_Dimensional_Minimizer::calculate_R(double new_point, double new_m) {
    if (new_m != m) {
        if (!pq->empty()) {
            delete pq;
            pq = new std::priority_queue<interval, std::vector<interval>,
                                         CompareR>;
        }
        m = new_m;
        reset();
        for (; right_point != values->end(); go_Next_Interval()) {
            (*left_point).second.R = get_R();
            pq->push(interval({ (*left_point).first, (*left_point).second.z },
                { (*right_point).first, (*right_point).second.z },
                (*left_point).second.R));
        }
    } else {
        go_new_left_interval(new_point);
        for (int i = 0; i < 2; ++i, go_Next_Interval()) {
            (*left_point).second.R = get_R();
            pq->push(interval({ (*left_point).first, (*left_point).second.z },
                { (*right_point).first, (*right_point).second.z },
                (*left_point).second.R));
        }
    }
}

double One_Dimensional_Minimizer::get_new_point(interval i) {
    return 0.5*(i.first_point.first + i.second_point.first) -
    ((i.second_point.second) - i.first_point.second) / (2 * m);
}

void One_Dimensional_Minimizer::insert_to_map(double _y, double _z,
                                              double _R) {
    characteristics _ch(_z, _R);
    values->insert({ _y, _ch });
    if (_z < res.z) {
        res.y = _y;
        res.z = _z;
    }
}

void One_Dimensional_Minimizer::compare_interval_len(double new_point) {
    go_new_left_interval(new_point);
    double interval_length;
    for (int i = 0; i < 2; ++i, go_Next_Interval()) {
        interval_length = (*right_point).first - (*left_point).first;
        if (interval_length < min_interval_length)
            min_interval_length = interval_length;
    }
}

void One_Dimensional_Minimizer::compare_M(double new_point) {
    go_new_left_interval(new_point);
    double M;
    for (int i = 0; i < 2; ++i, go_Next_Interval()) {
        M = get_M();
        if (M >= M_Max)
            M_Max = M;
    }
}

void One_Dimensional_Minimizer::delete_containers() {
    delete values;
    values = nullptr;
    delete pq;
    pq = nullptr;
}

void One_Dimensional_Minimizer::do_first_iteration() {
    min_interval_length = b - a;
    res.x = curr_x;
    res.y = a; res.z = (*function)(curr_x, a);
    if (a != b) {
        insert_to_map(res.y, res.z, 0);
        insert_to_map(b, (*function)(curr_x, b), 0);
        reset();
        M_Max = get_M();
        m = -1;
        pq->push(interval({ (*left_point).first, (*left_point).second.z },
        { (*right_point).first, (*right_point).second.z }, 0));
    }
}

void One_Dimensional_Minimizer::set_experiment(double _a, double _b,
    double _curr_x, double(*f)(double x, double y), double _eps, int _N_max,
    double _r_par) {
    if (a > b)
        throw "b is a right border, must be more than a";
    a = _a;
    b = _b;
    curr_x = _curr_x;
    function = f;
    eps = _eps;
    N_max = _N_max;
    r_p = _r_par;
    if (values == nullptr)
        values = new std::map<double, characteristics>;
    if (pq == nullptr)
        pq = new std::priority_queue<interval, std::vector<interval>,
                                     CompareR>;
}

result One_Dimensional_Minimizer::solve() {
    std::pair<double, double> new_point;
    double new_m;

    do_first_iteration();

    while (min_interval_length > eps) {
        new_m = get_m();
        calculate_R(new_point.first, new_m);
        new_point.first = get_new_point(pq->top()); pq->pop();
        new_point.second = (*function)(curr_x, new_point.first);
        insert_to_map(new_point.first, new_point.second, 0);
        compare_interval_len(new_point.first);
        compare_M(new_point.first);
    }

    delete_containers();
    return res;
}

Minimizer::Minimizer(double _a, double _b, double _curr_y, double _upper_y,
                     double(*f)(double x, double y),
    double _eps, int _N_max, double _r_par) :
    One_Dimensional_Minimizer(_a, _b, 0, f, _eps, _N_max, _r_par),
    curr_y(_curr_y), upper_y(_upper_y), function(f) {
    if (a > b)
        throw "b is a right border, must be more than a";
    if (curr_y > upper_y)
        throw "upper_y is a upper border, must be more than curr_y";
    MPI_Comm_rank(MPI_COMM_WORLD, &procrank);
    MPI_Comm_size(MPI_COMM_WORLD, &procnum);
    valIsMin = false;
}

void Minimizer::do_first_iteration(One_Dimensional_Minimizer* odm,
                                   result* tmp_res) {
    min_interval_length = b - a;
    odm->set_experiment(curr_y, upper_y, a, function);

    *tmp_res = odm->solve();
    res.x = a;
    res.y = tmp_res->y;
    res.z = tmp_res->z;
    insert_to_map(res.x, res.y, res.z, 0);

    if (a != b) {
        odm->set_experiment(curr_y, upper_y, b, function);
        *tmp_res = odm->solve();
        insert_to_map(b, tmp_res->y, tmp_res->z, 0);

        reset();
        M_Max = get_M();
        m = -1;
        pq->push(interval({ (*left_point).first, (*left_point).second.z },
                 { (*right_point).first, (*right_point).second.z }, 0));
    }
}

void Minimizer::share_interval() {
    double part = (b - a) / procnum;
    a = a + (part * procrank);
    b = a + part;
}

bool Minimizer::isMin() {
    return valIsMin;
}

int Minimizer::get_procnum() {
    return procnum;
}

int Minimizer::get_rank() {
    return procrank;
}

void Minimizer::insert_to_map(double _x, double _y, double _z, double _R) {
    characteristics _ch(_z, _R);
    values->insert({ _x, _ch });
    if (_z < res.z) {
        res.x = _x;
        res.y = _y;
        res.z = _z;
    }
}

result Minimizer::get_result() {
    return res;
}

void Minimizer::set_experiment(const double _a, const double _b,
    double _curr_y, double _upper_y, double(*f)(double x, double y),
    const double _eps, const int _N_max, const double _r_par) {
    a = _a;
    b = _b;
    curr_y = _curr_y;
    upper_y = _upper_y;
    function = f;
    eps = _eps;
    N_max = _N_max;
    r_p = _r_par;
    valIsMin = false;
    if (values == nullptr)
        values = new std::map<double, characteristics>;
    if (pq == nullptr)
        pq = new std::priority_queue<interval, std::vector<interval>,
                                     CompareR>;
}

void Minimizer::solve() {
    double sendbuf, recvbuf;
    One_Dimensional_Minimizer odm(0, 0, 0, nullptr), *podm = &odm;
    std::pair<double, double> new_point;
    double new_m;
    result tmp_res, *ptmp_res = &tmp_res;

    share_interval();
    do_first_iteration(podm, ptmp_res);

    while (min_interval_length > eps) {
        new_m = get_m();
        calculate_R(new_point.first, new_m);
        new_point.first = get_new_point(pq->top()); pq->pop();
        odm.set_experiment(curr_y, upper_y, new_point.first, function);
        tmp_res = odm.solve();
        new_point.second = tmp_res.z;
        insert_to_map(new_point.first, tmp_res.y, new_point.second, 0);
        compare_interval_len(new_point.first);
        compare_M(new_point.first);
    }
    delete_containers();

    sendbuf = res.z;
    MPI_Allreduce(&sendbuf, &recvbuf, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    if (res.z == recvbuf)
        valIsMin = true;
}

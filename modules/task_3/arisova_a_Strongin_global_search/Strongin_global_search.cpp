  // Copyright 2019 Arisova Anastasiia
#include <mpi.h>
#include <vector>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <cmath>
#include "../../../modules/task_3/arisova_a_Strongin_global_search/Strongin_global_search.h"

#define eps 1.2e-02

double Algorithm(double a, double b, double(*Q)(double)) {
    double Rt, Ri;  // Перемененные для подсчета характеристики
    std::vector<double> x;   // вектор значений xk
    std::vector<double> Qx;   // вектор значений Q(xk)


      // Шаг 1.
      // Задаем х1 = а Q1 = Q(х1) х2 = b Q2 = Q(х2)
    x.push_back(a); Qx.push_back(Q(a));
    x.push_back(b); Qx.push_back(Q(b));

      // Вычисляем Q* = min{Q1,Q2}  х* = arg min{Q1,Q2}
    double QQ, xx;   // QQ = Q* xx = x*
    if (Qx[0] <= Qx[1]) {
        QQ = Qx[0];
        xx = x[0];
    } else {
        QQ = Qx[1];
        xx = x[1];
    }

      // k = 2
    int k = 2;   // количество рассматриваемых точек

    int t = 0;   // t - номер интервала на ктр подсчитана минимальная характеристика Rt
    int ind;   // индекс массивов х и Qх, обеспечивающий упорядоченность точек х по возрастанию координаты
    double deltaQi;   // deltaQi = Qx[i + 1] - Qx[i]
    double deltaxi;   // deltaxi = x[i + 1] - x[i]
    double Lk, lk;   // Lk - константа Липшица, lk - оценка снизу для Lk
    double r = 2.0;   // параметр надежности
    double xt;   // точка для нового измерения
    bool flag = true;
    while (flag) {
          // Вычисление lk = max { |deltaQi| / deltaxi }
        double temp;
        lk = 100;
        for (int i = 0; i < k - 1; i++) {
            deltaQi = Qx[i + 1] - Qx[i];
            deltaxi = x[i + 1] - x[i];
            temp = std::abs(deltaQi)/ deltaxi;
            if (temp > lk) {
                lk = temp;
            }
        }
          // Вычисление Lk
        if (lk > 0)
            Lk = r * lk;
        if (lk == 0)
            Lk = 1;
          // Шаг 3. Определить подынтервал [x[t]; x[t+1]] с минимальным значением характеристики
        Rt = 100;
        for (int i = 0; i < k - 1; i++) {
            deltaQi = Qx[i + 1] - Qx[i];
            deltaxi = x[i + 1] - x[i];
              // Ri - значение характеристики на i-ом подынтервале
              // Rt = min {Ri: i = 1, ... k - 1}
            Ri = (Qx[i] + Qx[i + 1] - (1.0 + ((deltaQi / deltaxi) / Lk) * ((deltaQi / deltaxi) / Lk)) * Lk * deltaxi);
            Ri = Ri * 1.0 / 2.0;
            if (Ri < Rt) {
                Rt = Ri;
                t = i;
            }
        }
          // Шаг 4.
          // Если выполнен критерий останова, принять QQ и xx за ответ
          // Если нет -> переходим на шаг 5.

        if (x[t + 1] - x[t] < eps) {
            flag = false;
        } else {
              // Шаг 5.
              // Проводим новое измерение в точке х_k+1 = x_t
            xt = 1.0 / 2.0 * (x[t] + x[t + 1] - deltaQi / Lk);
            ind = 0;

            while (xt > x[ind]) {
                ind++;
            }
            auto iter1 = x.cbegin();
            auto iter2 = Qx.cbegin();
            x.emplace(iter1 + ind, xt);
            Qx.emplace(iter2 + ind, Q(xt));
              // Положить Q*_k+1 = ... x*_k+1
            if (Qx[ind] < QQ) {
                QQ = Qx[ind];
                xx = x[ind];
            }
              // k = k + 1
            k = k + 1;
        }
    }
    // std::cout<<Q(xx)<<std::endl;
    return xx;
}
double goParallelAlgorithm(double a, double b, double(*Q)(double)) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    std::vector<double>gl_vec;

    double h = std::abs(b - a) / (static_cast<double> (size));
    gl_vec.push_back(a);
    for (int i = 1; i < size; i++) {
        gl_vec.push_back(a + i * h);
        gl_vec.push_back(a + i * h);
    }
    gl_vec.push_back(b);

    int l_N = 2;

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(&gl_vec[0] + i * l_N, l_N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
    }
    std::vector<double>l_vec(l_N);
    if (rank == 0) {
        l_vec = std::vector<double>(gl_vec.begin(), gl_vec.begin() + l_N);
    } else {
        MPI_Status status;
        MPI_Recv(&l_vec[0], l_N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }
    double answer = 0.0;
    double l_answer = Algorithm(l_vec[0], l_vec[1], Q);
    l_answer = Q(l_answer);
    MPI_Op op_code;
    op_code = MPI_MIN;
    MPI_Reduce(&l_answer, &answer, 1, MPI_DOUBLE, op_code, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(&answer, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&answer, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }
    return answer;
}

// Copyright 2019 Savosina
#include <mpi.h>
#include <utility>
#include <vector>
#include <stdexcept>
#include "../../../modules/task_3/savosina_a_simpson_method/simpson_method.h"

double calculateIntegral(const std::vector<std::pair<double, double>>& _scope, const size_t& _n,
const std::function<double(const std::vector<double>&)>& f, const size_t& _nRec) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double res = 0;
    if (size == 1) {
        res = calculateIntegralSequential(_scope, _n, f, _nRec);
    } else {
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Status status;
        int levelNumber = 0;
        if (rank == 0) {
            double h = (_scope[levelNumber].second - _scope[levelNumber].first) / (2 * _n);
            std::vector<double> funcResults;
            int rankItr = 1;
            double mes;
            if (size > static_cast<int>(_n * 2)) {
                for (double i = _scope[levelNumber].first; i <= _scope[levelNumber].second; i = i + h) {
                    MPI_Send(&i, 1, MPI_DOUBLE, rankItr, 1, MPI_COMM_WORLD);
                    rankItr++;
                }
                for (int i = 1; i < rankItr; i++) {
                    MPI_Recv(&mes, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
                    funcResults.push_back(mes);
                }
            } else {
                int needToSolve = _n * 2 / size;
                double iter = _scope[levelNumber].first;
                iter += needToSolve * h;
                for (rankItr = 1; rankItr < size; rankItr++)
                    for (int i = 0; i < needToSolve; i++) {
                        MPI_Send(&iter, 1, MPI_DOUBLE, rankItr, 1, MPI_COMM_WORLD);
                        iter += h;
                    }
                for (rankItr = 1; rankItr <= static_cast<int>(_n * 2 % size); rankItr++) {
                    MPI_Send(&iter, 1, MPI_DOUBLE, rankItr, 1, MPI_COMM_WORLD);
                    iter += h;
                }
                for (double i = _scope[levelNumber].first; i < _scope[levelNumber].first + needToSolve * h; i = i + h) {
                    std::vector<double> fixedVar = { i };
                    if (static_cast<int>(_scope.size()) == levelNumber + 1) {
                        funcResults.push_back(f(fixedVar));
                    } else {
                        funcResults.push_back(calculateRecusionLevel(fixedVar, levelNumber + 1, _scope, f, _nRec));
                    }
                }
                for (rankItr = 1; rankItr < size; rankItr++)
                    for (int i = 0; i < needToSolve; i++) {
                        MPI_Recv(&mes, 1, MPI_DOUBLE, rankItr, 1, MPI_COMM_WORLD, &status);
                        funcResults.push_back(mes);
                    }
                for (rankItr = 1; rankItr <= static_cast<int>(_n * 2 % size); rankItr++) {
                    MPI_Recv(&mes, 1, MPI_DOUBLE, rankItr, 1, MPI_COMM_WORLD, &status);
                    funcResults.push_back(mes);
                }
            }
            mes = -1;
            for (rankItr = 1; rankItr < size; rankItr++) {
                MPI_Send(&mes, 1, MPI_DOUBLE, rankItr, 2, MPI_COMM_WORLD);
            }
            double tempRes = funcResults[0];
            res = tempRes;
            tempRes = 0;
            for (size_t i = 1; i <= _n; i++)
                tempRes += funcResults[2*i - 1];
            res += 4 * tempRes;
            tempRes = 0;
            for (size_t i = 1; i <= _n - 1; i++)
                tempRes += funcResults[2*i];
            res += 2 * tempRes;
            res += funcResults[2 * _n - 1];
            res = res * h / 3;
        } else {
            bool terminate = false;
            while (!terminate) {
                double mes;
                MPI_Recv(&mes, 1, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                if (status.MPI_TAG == 2) {
                    terminate = true;
                } else {
                    std::vector<double> fixedVar = { mes };
                    if (static_cast<int>(_scope.size()) == levelNumber + 1) {
                        res = f(fixedVar);
                    } else {
                        res = calculateRecusionLevel(fixedVar, levelNumber + 1, _scope, f, _nRec);
                    }
                    MPI_Send(&res, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
                }
            }
        }
    }
    return res;
}

double calculateIntegralSequential(const std::vector<std::pair<double, double>>& _scope, const size_t& _n,
const std::function<double(const std::vector<double>&)>& f, const size_t& _nRec) {
    int levelNumber = 0;
    double h = (_scope[levelNumber].second - _scope[levelNumber].first) / (2 * _n);
    std::vector<double> funcResults;
    for (double i = _scope[levelNumber].first; i <= _scope[levelNumber].second; i = i + h) {
        std::vector<double> fixedVar = { i };
        if (static_cast<int>(_scope.size()) == levelNumber + 1) {
            funcResults.push_back(f(fixedVar));
        } else {
            funcResults.push_back(calculateRecusionLevel(fixedVar, levelNumber + 1, _scope, f, _nRec));
        }
    }
    double tempRes = funcResults[0];
    double res = tempRes;
    tempRes = 0;
    for (size_t i = 1; i <= _n; i++)
        tempRes += funcResults[2*i - 1];
    res += 4 * tempRes;
    tempRes = 0;
    for (size_t i = 1; i <= _n - 1; i++)
        tempRes += funcResults[2*i];
    res += 2 * tempRes;
    res += funcResults[2 * _n - 1];
    res = res * h / 3;
    return res;
}

double calculateRecusionLevel(std::vector<double> _fixedVar, size_t _levelNumber,
const std::vector<std::pair<double, double>>& _scope,
const std::function<double(const std::vector<double>&)>& f, const size_t& _n) {
    double h = (_scope[_levelNumber].second - _scope[_levelNumber].first) / (2 * _n);
    std::vector<double> funcResults;
    _fixedVar.push_back(0);
    if (_levelNumber + 1 == _scope.size()) {
        for (double i = _scope[_levelNumber].first; i <= _scope[_levelNumber].second; i = i + h) {
            _fixedVar[_levelNumber] = i;
            funcResults.push_back(f(_fixedVar));
        }
    } else {
        for (double i = _scope[_levelNumber].first; i <= _scope[_levelNumber].second; i = i + h) {
            _fixedVar[_levelNumber] = i;
            funcResults.push_back(calculateRecusionLevel(_fixedVar, _levelNumber+1, _scope, f, _n));
        }
    }
    double tempRes = funcResults[0];
    double res = tempRes;
    tempRes = 0;
    for (size_t i = 1; i <= _n; i++)
        tempRes += funcResults[2*i - 1];
    res += 4 * tempRes;
    tempRes = 0;
    for (size_t i = 1; i <= _n - 1; i++)
        tempRes += funcResults[2*i];
    res += 2 * tempRes;
    res += funcResults[2 * _n - 1];
    res = res * h / 3;
    return res;
}

void scopeCheck(const std::vector<std::pair<double, double>>& scope) {
    for (auto iter = scope.begin(); iter != scope.end(); iter++) {
        if (iter->first > iter->second)
            throw std::runtime_error("Invalide scope");
    }
}

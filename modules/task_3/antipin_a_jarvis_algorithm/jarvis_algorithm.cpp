// Copyright 2019 Antipin Alexander

#include "../../../modules/task_3/antipin_a_jarvis_algorithm/jarvis_algorithm.h"
#include <vector>
#include <limits>

point::point() {
    x = 0.0;
    y = 0.0;
}

point::point(const point & p) {
    x = p.x;
    y = p.y;
}

point::point(const double x, const double y) {
    this->x = x;
    this->y = y;
}

point::~point() {
}

void point::setX(const double x) {
    this->x = x;
}

void point::setY(const double y) {
    this->y = y;
}

double point::getX() const {
    return x;
}

double point::getY() const {
    return y;
}

bool point::operator==(const point & p) const {
    if (x == p.x && y == p.y) {
        return true;
    } else {
        return false;
    }
}

bool point::operator<=(const point & p) const {
    if (x < p.x) {
        return true;
    } else if (x == p.x && y <= p.y) {
        return true;
    } else {
        return false;
    }
}

bool point::operator>(const point & p) const {
    if (x > p.x) {
        return true;
    } else if (x == p.x && y > p.y) {
        return true;
    } else {
        return false;
    }
}

void getRandomFieldOfPoints(std::vector<point>* field, const int maxX, const int minX,
    const int maxY, const int minY) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(NULL)));
    for (unsigned int i = 0; i < field->size(); ++i) {
        (*field)[i].setX(static_cast<double>(gen() % (maxX - minX) + minX));
        (*field)[i].setY(static_cast<double>(gen() % (maxY - minY) + minY));
    }
}

bool isMaxRightPoint(const std::vector<point>& field, const int min, const int start, const int end) {
    if (field[start] == field[min]) {
        return true;
    }
    double k = (field[end].getY() - field[start].getY()) / (field[end].getX() - field[start].getX());
    double k2 = (field[min].getY() - field[start].getY()) / (field[min].getX() - field[start].getX());
    double b = field[end].getY() - k * field[end].getX();
    bool isUp = field[start].getY() < field[end].getY();
    double newX;

    // if the direction is vertical

    if (k == std::numeric_limits<double>::infinity() || k == -std::numeric_limits<double>::infinity()) {
        newX = field[end].getX();
        if (field[end].getX() == field[min].getX()) {
            if (k == std::numeric_limits<double>::infinity()) {
                return field[end].getY() > field[min].getY() ? true : false;
            } else if (k == -std::numeric_limits<double>::infinity()) {
                return field[end].getY() < field[min].getY() ? true : false;
            }
        }
    } else if (k == 0.0) {  // if the direction is horizontal
        newX = field[min].getX();
        if (field[start].getX() > field[end].getX()) {
            if (field[min].getY() != field[end].getY()) {
                return point(newX, field[end].getY()) <= field[min] ? false : true;
            } else {
                return field[min] > field[end] ? true : false;
            }
        } else if (field[start].getX() < field[end].getX()) {
            if (field[min].getY() != field[end].getY()) {
                return point(newX, field[end].getY()) <= field[min] ? true : false;
            } else {
                return field[min] > field[end] ? false : true;
            }
        }
    } else if (k == k2) {  // if 2 points on one line
        if (isUp) {
            return field[end].getY() > field[min].getY() ? true : false;
        } else {
            return field[end].getY() < field[min].getY() ? true : false;
        }
    } else {  // simple happend
        newX = (field[min].getY() - b) / k;
    }
    if (isUp && k > 0.0) {
        return point(newX, field[min].getY()) <= field[min] ? false : true;
    } else if (isUp && k < 0.0) {
        return point(newX, field[min].getY()) > field[min] ? true : false;
    } else if (!isUp && k > 0.0) {
        return point(newX, field[min].getY()) <= field[min] ? true : false;
    } else if (!isUp && k < 0.0) {
        return point(newX, field[min].getY()) <= field[min] ? true : false;
    }
    return false;
}

void getSequentialSolution(const std::vector<point>& field, std::vector<int>* result) {
    if (field.size() < 3) {
        throw 1;
    }
    int startPoint = 0;
    int currPoint = 0;
    for (unsigned int i = 1; i < field.size(); ++i) {
        if (field[i] <= field[startPoint]) {
            startPoint = i;
        }
    }
    currPoint = startPoint;
    int j = 1;
    (*result).resize(field.size());
    (*result)[0] = currPoint;
    while (true) {
        int currMin = currPoint == 0 ? 1 : 0;
        for (unsigned int i = 0; i < field.size(); ++i) {
            if (i == static_cast<unsigned int>(currPoint)) {
                continue;
            }
            if (field[i] == field[currPoint]) {
                continue;
            }
            if (isMaxRightPoint(field, currMin, currPoint, i)) {
                currMin = i;
            }
        }
        (*result)[j] = currMin;
        currPoint = currMin;
        j++;
        if (currPoint == startPoint || field[currPoint] == field[startPoint]) {
            break;
        }
    }
    (*result).resize(j - 1);
}

void getParallelSolution(const std::vector<point>& field, std::vector<int>* result) {
    if (field.size() < 3) {
        throw 1;
    }
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (field.size() < static_cast<unsigned int>(size)) {
        if (rank == 0) {
            getSequentialSolution(field, result);
            return;
        } else {
            return;
        }
    }
    int startPoint = 0;
    int currPoint = 0;
    if (rank == 0) {
        for (unsigned int i = 1; i < field.size(); ++i) {
            if (field[i] <= field[startPoint]) {
                startPoint = i;
            }
        }
    }
    MPI_Bcast(&startPoint, 1, MPI_INT, 0, MPI_COMM_WORLD);
    currPoint = startPoint;
    int j = 1;
    (*result).resize(field.size());
    int pointCount = rank == size - 1 ? field.size() / size + field.size() % size : field.size() / size;
    (*result)[0] = currPoint;
    while (true) {
        int currMin = static_cast<unsigned int>(currPoint) == (field.size() / size) * static_cast<unsigned int>(rank) ?
            (field.size() / size) * rank + 1 : (field.size() / size) * rank;
        for (unsigned int i = currMin; i < (rank == (size - 1) ? field.size() : pointCount * (rank + 1)); ++i) {
            if (i == static_cast<unsigned int>(currPoint)) {
                continue;
            }
            if (field[i] == field[currPoint]) {
                continue;
            }
            if (isMaxRightPoint(field, currMin, currPoint, i)) {
                currMin = i;
            }
        }
        if (rank == 0) {
            for (int i = 1; i < size; ++i) {
                int locPoint;
                MPI_Status status;
                MPI_Recv(&locPoint, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                if (isMaxRightPoint(field, currMin, currPoint, locPoint)) {
                    currMin = locPoint;
                }
            }
            (*result)[j] = currMin;
            currPoint = currMin;
            j++;
        } else {
            MPI_Send(&currMin, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        MPI_Bcast(&currPoint, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (currPoint == startPoint || field[currPoint] == field[startPoint]) {
            break;
        }
    }
    (*result).resize(j - 1);
}

/*
 * Copyright (C) 2019 LOOGIN. All Rights Reserved.
 * Available types: char(Int8), int(Int32), float(Float32) and double(Float64).
 */
#ifndef MODULES_TASK_3_LUGIN_M_SHELL_SORT_SHELLSORT_H_
#define MODULES_TASK_3_LUGIN_M_SHELL_SORT_SHELLSORT_H_

#include <mpi.h>
#include <vector>
#include <cstdlib>
#include <string>
#include <ctime>
#include <utility>
#include <random>
#include <typeinfo>

template <class T>
std::string OutPutVector(std::vector<T> v) {
    std::string vec;
    vec = '{';
    for (const auto &curV : v) {
        vec += std::to_string(curV);
        vec += ',';
    }
    vec += '}';
    return vec;
}

template <class T>
MPI_Datatype MPI_Get_Datatype(T type) {
    if (typeid(type).name() == typeid(static_cast<double>(1.)).name()) {
        return MPI_DOUBLE;
    }
    if (typeid(type).name() == typeid(static_cast<float>(1.)).name()) {
        return MPI_FLOAT;
    }
    if (typeid(type).name() == typeid(static_cast<char>('1')).name()) {
        return MPI_CHAR;
    }
    if (typeid(type).name() == typeid(static_cast<int>(1)).name()) {
        return MPI_INT;
    }
    throw "Invalid Datatype";
}

template <typename Iterator, typename Compare>
void ShellSort(Iterator first, Iterator last, Compare comp) {
    for (typename std::iterator_traits<Iterator>::difference_type d = (last - first) / 2; d != 0; d /= 2)
        for (Iterator i = first + d; i != last; ++i)
            for (Iterator j = i; j - first >= d && comp(*j, *(j - d)); j -= d)
                std::swap(*j, *(j - d));
}

template <typename T, typename Compare>
std::vector<T> Merge(const std::vector<T>& mainVec, T massive[], int size, Compare comp) {
    int i = 0;
    std::vector<T> mergedVect(mainVec.begin(), mainVec.end());
    auto IT = mergedVect.begin();
    for (int pos = 0; (IT + pos) != mergedVect.end() && i < size; pos++)
        if (comp(massive[i], *(IT + pos)) ||
            (!comp(massive[i], *(IT + pos)) &&
            !comp(*(IT + pos), massive[i]))) {
            mergedVect.insert((IT + pos), massive[i++]);
            IT = mergedVect.begin();
        }

    while (i < size)
        mergedVect.push_back(massive[i++]);

    return mergedVect;
}

template <class T, typename Compare>
std::vector<T> IShellSort(std::vector<T> massive, Compare comp) {
    int sizeOfMassive = massive.size();
    if (sizeOfMassive < 2)
        return massive;

    MPI_Datatype curDatatype = MPI_Get_Datatype(massive[0]);
    int procNum, procRank;

    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    std::vector<T> buffer;

    if (procRank < sizeOfMassive) {
        for (int i = procRank; i < sizeOfMassive; i += procNum) {
            buffer.push_back(massive[i]);
        }

        ShellSort(buffer.begin(), buffer.end(), comp);
    } else {
        return massive;
    }

    if (procNum == 1)
        return buffer;

    int mergeRange = 1;

    do {
        mergeRange *= 2;

        if (procRank % mergeRange == mergeRange / 2 && buffer.size()) {
            MPI_Ssend(&buffer[0], buffer.size(), curDatatype, procRank - mergeRange / 2, 0, MPI_COMM_WORLD);
            return buffer;
        }
        if (procRank % mergeRange == 0 && (procRank + mergeRange / 2) < procNum && buffer.size()) {
            int sizeCurBuffer;
            MPI_Status status;

            MPI_Probe(procRank + mergeRange / 2, 0, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, curDatatype, &sizeCurBuffer);
            T *curBuffer = new T[sizeCurBuffer];

            MPI_Recv(curBuffer, sizeCurBuffer, curDatatype, procRank + mergeRange / 2, 0, MPI_COMM_WORLD, &status);

            buffer = Merge(buffer, curBuffer, sizeCurBuffer, comp);
            delete[] curBuffer;
        }
    } while (mergeRange < procNum);

        return buffer;
}

template <typename T>
std::vector<T> getSortedRandomArrayBottomUp(T type, int size, int _percentSkipped = 20, int _percentRepeated = 35) {
    int procRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::mt19937 generator(time(NULL));
    std::uniform_int_distribution<int> percentSkipped(0, _percentSkipped);
    std::uniform_int_distribution<int> percentRepeated(0, _percentRepeated);
    std::vector<T> array;
    if (procRank == 0) {
        if (typeid(type).name() == typeid(static_cast<double>(1.)).name()) {
            array.push_back(0.);
            for (int pos = 0; pos < size - 1; pos++) {
                double curElem = array.back();
                if (percentRepeated(generator)) {
                    array.push_back(curElem);
                    continue;
                }
                while (percentSkipped(generator))
                    curElem++;
                array.push_back(curElem);
            }
        }
        if (typeid(type).name() == typeid(static_cast<float>(1.)).name()) {
            array.push_back(0.);
            for (int pos = 0; pos < size - 1; pos++) {
                float curElem = array.back();
                if (percentRepeated(generator)) {
                    array.push_back(curElem);
                    continue;
                }
                while (percentSkipped(generator))
                    curElem++;
                array.push_back(curElem);
            }
        }
        if (typeid(type).name() == typeid(static_cast<char>('1')).name()) {
            for (char pos = 0; pos < size; pos++) {
                int curCount = pos < size % 256 ? size / 256 + 1 : size / 256;
                while ( curCount-- > 0)
                    array.push_back(pos);
            }
        }
        if (typeid(type).name() == typeid(static_cast<int>(1)).name()) {
            array.push_back(0);
            for (int pos = 0; pos < size - 1; pos++) {
                int curElem = array.back();
                if (percentRepeated(generator)) {
                    array.push_back(curElem);
                    continue;
                }
                while (percentSkipped(generator))
                    curElem++;
                curElem++;
                array.push_back(curElem);
            }
        }
    }
    if (procRank != 0)
        array.resize(size);

    MPI_Bcast(&array[0], size, MPI_Get_Datatype(type), 0, MPI_COMM_WORLD);

    return array;
}

template <typename T>
std::vector<T> getSortedRandomArrayTopDown(T type, int size, int _percentSkipped = 20, int _percentRepeated = 35) {
    int procRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::mt19937 generator(time(NULL));
    std::uniform_int_distribution<int> percentSkipped(0, _percentSkipped);
    std::uniform_int_distribution<int> percentRepeated(0, _percentRepeated);
    std::vector<T> array;
    if (procRank == 0) {
        if (typeid(type).name() == typeid(static_cast<double>(1.)).name()) {
            array.push_back(0.);
            for (int pos = 0; pos < size - 1; pos++) {
                double curElem = array.front();
                if (percentRepeated(generator)) {
                    array.insert(array.begin(), curElem);
                    continue;
                }
                while (percentSkipped(generator))
                    curElem++;
                array.insert(array.begin(), curElem);
            }
        }
        if (typeid(type).name() == typeid(static_cast<float>(1.)).name()) {
            array.push_back(0.);
            for (int pos = 0; pos < size - 1; pos++) {
                float curElem = array.front();
                if (percentRepeated(generator)) {
                    array.insert(array.begin(), curElem);
                    continue;
                }
                while (percentSkipped(generator))
                    curElem++;
                array.insert(array.begin(), curElem);
            }
        }
        if (typeid(type).name() == typeid(static_cast<char>('1')).name()) {
            for (char pos = 0; pos < size; pos++) {
                int curCount = pos < size % 256 ? size / 256 + 1 : size / 256;
                while (curCount-- > 0)
                array.insert(array.begin(), pos);
            }
        }
        if (typeid(type).name() == typeid(static_cast<int>(1)).name()) {
            array.push_back(0);
            for (int pos = 0; pos < size - 1; pos++) {
                int curElem = array.front();
                if (percentRepeated(generator)) {
                    array.insert(array.begin(), curElem);
                    continue;
                }
                while (percentSkipped(generator))
                    curElem++;
                array.insert(array.begin(), curElem);
            }
        }
    }

    if (procRank != 0)
        array.resize(size);

    MPI_Bcast(&array[0], size, MPI_Get_Datatype(type), 0, MPI_COMM_WORLD);

    return array;
}

template <typename T>
std::vector<T> RandomizeArray(std::vector<T> array, int iterations) {
    int procRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::mt19937 generator(time(NULL));
    std::uniform_int_distribution<int> pos(0, array.size() - 1);
    std::vector<T> randomArray(array.begin(), array.end());
    if (procRank == 0) {
        int size = array.size();
        while (iterations--)
            for (int i = 0; i < size ; i++)
                std::swap(randomArray[i], randomArray[pos(generator)]);
    } else {
        randomArray.resize(array.size());
    }

    MPI_Bcast(&randomArray[0], randomArray.size(), MPI_Get_Datatype(randomArray[0]), 0, MPI_COMM_WORLD);

    return randomArray;
}

#endif  // MODULES_TASK_3_LUGIN_M_SHELL_SORT_SHELLSORT_H_

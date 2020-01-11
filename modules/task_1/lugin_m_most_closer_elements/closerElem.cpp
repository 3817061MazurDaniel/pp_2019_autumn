/*
 * Copyright (C) 2019 LOOGIN. All Rights Reserved.
 */

#include <../../../modules/task_1/lugin_m_most_closer_elements/closerElem.h>
#include <string>
#include <vector>

std::string OutPutVector(std::vector<int> v) {
    std::string vec;
    vec = '{';
    for (const auto &curV : v) {
        vec += std::to_string(curV);
        vec += ',';
    }
    vec += '}';
    return vec;
}

std::vector<int> ISearch(const std::vector<int>& vect) {
    int size = vect.size();
    if (size < 2)
        return std::vector<int>();
    int procNum, procRank;

    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    std::vector<int> buffer;
    int beginIndex;
    if (procRank < size) {
        int range = size / procNum == 0 ? 1 : size / procNum;
        auto IT = vect.begin();
        beginIndex = procRank * range;
        if (procRank < procNum - 1 && procRank != size - 1) {
            buffer = std::vector<int>((IT + beginIndex), (IT + beginIndex + range + 1));
        } else {
            buffer = std::vector<int>((IT + beginIndex), vect.end());
        }
    }

    int compareRange = 1;

    if (buffer.size() > 1) {
        buffer = Search(buffer, beginIndex);
        int procLast = size < procNum ? size - 1 : procNum;

        do {
            compareRange *= 2;

            if (procRank % compareRange == compareRange / 2) {
                MPI_Ssend(&buffer[0], 4, MPI_INT, procRank - compareRange / 2, 0, MPI_COMM_WORLD);
                return buffer;
            }
            if (procRank % compareRange == 0 && (procRank + compareRange / 2) < procLast) {
                MPI_Status status;
                int *curBuffer = new int[4];
                MPI_Recv(curBuffer, 4, MPI_INT, procRank + compareRange / 2, 0, MPI_COMM_WORLD, &status);

                buffer = Compare(buffer, curBuffer);
                delete[] curBuffer;
            }
        } while (compareRange < procNum);
    }

    return buffer;
}

std::vector<int> Search(const std::vector<int>& vect, int posFirstElem) {
    int size = vect.size();
    if (size < 2)
        throw("Vector have less 2 elements");
    std::vector<int> minInfo = {abs(vect[0] - vect[1]), vect[0], vect[1], posFirstElem};
    for (int pos = 1; pos < size - 1; pos++) {
        if (abs(vect[pos] - vect[pos + 1]) < minInfo[0]) {
            minInfo = {abs(vect[pos] - vect[pos + 1]), vect[pos], vect[pos + 1], posFirstElem + pos};
        }
    }

    return minInfo;
}

std::vector<int> Compare(const std::vector<int>& curVect, int comp[]) {
    if (comp[0] < curVect.front()) {
        std::vector<int> newVect;
        for (int pos = 0; pos < 4; pos++)
            newVect.push_back(comp[pos]);
        return newVect;
    }
    return curVect;
}

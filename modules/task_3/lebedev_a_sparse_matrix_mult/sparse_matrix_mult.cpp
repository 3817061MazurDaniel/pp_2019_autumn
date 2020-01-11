// Copyright 2019 Lebedev Alexander

#include <iostream>
#include <vector>
#include <set>
#include "../../../modules/task_3/lebedev_a_sparse_matrix_mult/sparse_matrix_mult.h"

ccsMatrix::ccsMatrix(int nn, std::vector<double> val, std::vector<int> ind, std::vector<int> offs)
    :n(nn),
    value(val),
    index(ind),
    offset(offs) {
    // Empty constructor
}

void ccsMatrix::init(int nn, std::vector<double> val, std::vector<int> ind, std::vector<int> offs) {
    n = nn;
    value = val;
    index = ind;
    offset = offs;
}

void ccsMatrix::init(const ccsMatrix& copy) {
    n = copy.n;
    value = copy.value;
    index = copy.index;
    offset = copy.offset;
}

void ccsMatrix::clear() noexcept {
    value.clear();
    index.clear();
    offset.clear();
}

void ccsMatrix::transpanent() {
    std::vector<std::vector<double>> newValue(n);
    std::vector<std::vector<int>>    newIndex(n);
    for (int i = 0; i < n; i++) {
        for (int j = offset[i]; j < offset[i + 1]; j++) {
            newValue[index[j]].push_back(value[j]);
            newIndex[index[j]].push_back(i);
        }
    }

    this->clear();
    offset.push_back(0);
    int last = 0;

    for (int i = 0; i < n; i++) {
        int len = static_cast<int>(newValue[i].size());
        last = last + len;
        offset.push_back(last);
        for (int j = 0; j < len; j++) {
            value.push_back(newValue[i][j]);
            index.push_back(newIndex[i][j]);
        }
    }
}

bool operator ==(const ccsMatrix& x, const ccsMatrix& y) {
    return (y.n == x.n && y.index == x.index && y.value == x.value && y.offset == x.offset);
}

ccsMatrix generateRandomMatrix(int n, int lowerBound, int upperBound) {
    std::random_device rd;
    std::mt19937 rnd(rd());

    ccsMatrix result;
    result.n = n;
    result.offset.push_back(0);

    int maxElementPerCol = (n > 5 ? n >> 2 : n >> 1);
    if (n <= 3) maxElementPerCol = n;

    std::uniform_int_distribution<int> valGen(lowerBound, upperBound);
    std::uniform_int_distribution<int> cntGen(0, n - 1);
    std::uniform_int_distribution<int> lenGen(0, maxElementPerCol);

    for (int i = 0; i < n; i++) {
        int count = lenGen(rnd);
        result.offset.push_back(result.offset[i] + count);
        std::set<int> indexes;

        while (static_cast<int>(indexes.size()) != count) {
            indexes.insert(cntGen(rnd));
        }
        for (auto x : indexes) {
            result.index.push_back(x);
            result.value.push_back(valGen(rnd));
        }
    }
    return result;
}

ccsMatrix sparseMatrixMultSequential(ccsMatrix A, ccsMatrix B, double eps) {
    A.transpanent();
    int n = A.n;
    ccsMatrix C(n, {}, {}, {});
    C.offset.push_back(0);
    for (int i = 0; i < n; i++) {
        int newOffset = 0;
        for (int j = 0; j < n; j++) {
            double partSum = 0.;
            int    bElem   = B.offset[j];
            int    bBound  = B.offset[j + 1];
            for (int aElem = A.offset[i]; aElem < A.offset[i + 1]; aElem++) {
                while (bElem < bBound && B.index[bElem] < A.index[aElem]) {
                    bElem++;
                }
                if (bElem >= bBound) {
                    break;
                }
                if (B.index[bElem] == A.index[aElem]) {
                    partSum += B.value[bElem] * A.value[aElem];
                }
            }
            if (fabs(partSum) > eps) {
                C.index.push_back(j);
                C.value.push_back(partSum);
                newOffset++;
            }
        }
        C.offset.push_back(newOffset + C.offset[i]);
    }
    return C;
}

ccsMatrix sparseMatrixMultParallel(ccsMatrix A, ccsMatrix B, double eps, MPI_Comm comm) {
    int rank;
    int commSize;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &commSize);

    int n    = 0;
    int root = 0;

    if (rank == root) {
        A.transpanent();
        n = A.n;
    }
    MPI_Bcast(&n, 1, MPI_INT, root, comm);

    int size = commSize;

    std::vector<double> currAValue(2 * n);
    std::vector<double> currBValue(2 * n);
    std::vector<int>    currAIndex(2 * n);
    std::vector<int>    currBIndex(2 * n);
    std::vector<int>    currBOffset(2 * n);
    ccsMatrix           result;
    int                 elementsPerProc = n / size;
    int                 elementsCurProc = elementsPerProc + (rank == size - 1 ? n % size : 0);
    int                 currBOffsetLen  = 0;
    int                 currBValueLen   = 0;
    int                 currBIndexLen   = 0;

    if (rank == root) {
        result.n = n;
        result.offset.push_back(0);
    }

    // Shareing parths of B to other processes
    if (rank == root) {
        for (int ind = 1; ind < size; ind++) {
            int leftIndex  = ind * elementsPerProc;
            int rightIndex = leftIndex + elementsPerProc + (ind == size - 1 ? n % size : 0);
            int l          = B.offset[leftIndex];
            int r          = B.offset[rightIndex];

            MPI_Send(&(B.offset[leftIndex]), rightIndex - leftIndex + 1, MPI_INT, ind, 0, comm);
            if (r - l > 0) {
                MPI_Send(&(B.index[l]), r - l, MPI_INT, ind, 0, comm);
                MPI_Send(&(B.value[l]), r - l, MPI_DOUBLE, ind, 0, comm);
            }
        }
        currBOffsetLen = elementsCurProc + 1;

        currBOffset.assign(B.offset.begin(), B.offset.begin() + currBOffsetLen);

        currBIndexLen = currBOffset[elementsCurProc] - currBOffset[0];
        currBValueLen = currBIndexLen;

        currBIndex.assign(B.index.begin(), B.index.begin() + currBIndexLen);
        currBValue.assign(B.value.begin(), B.value.begin() + currBValueLen);
    } else {
        currBOffsetLen = elementsCurProc + 1;

        MPI_Recv(&currBOffset[0], currBOffsetLen, MPI_INT, root, 0, comm, MPI_STATUS_IGNORE);
        currBIndexLen  = currBOffset[elementsCurProc] - currBOffset[0];
        currBValueLen  = currBIndexLen;
        if (currBIndexLen > 0) {
            MPI_Recv(&(currBIndex[0]), currBIndexLen, MPI_INT, root, 0, comm, MPI_STATUS_IGNORE);
            MPI_Recv(&(currBValue[0]), currBValueLen, MPI_DOUBLE, root, 0, comm, MPI_STATUS_IGNORE);
        }
    }
    // Execution process and Sharing A
    for (int i = 0; i < n; i++) {
        int blockSize = 0;
        if (rank == root) {
            int l = A.offset[i];
            int r = A.offset[i + 1];
            blockSize = r - l;
            if (blockSize > 0) {
                currAValue.assign(A.value.begin() + l, A.value.begin() + r);
                currAIndex.assign(A.index.begin() + l, A.index.begin() + r);
            }
        }
        MPI_Bcast(&blockSize, 1, MPI_INT, root, comm);
        if (blockSize <= 0) {
            if (rank == root) {
                result.offset.push_back(*(--result.offset.end()));
            }
        } else {
            MPI_Bcast(&currAValue[0], blockSize, MPI_DOUBLE, root, comm);
            MPI_Bcast(&currAIndex[0], blockSize, MPI_INT, root, comm);
            std::vector<double> currentResult;
            std::vector<int>    currentResIndex;

            for (int j = 0; j < currBOffsetLen - 1; j++) {
                double partSum = 0.;
                int    bElem   = currBOffset[j] - currBOffset[0];
                int    bBound  = currBOffset[j + 1] - currBOffset[0];

                for (int aElem = 0; aElem < blockSize; aElem++) {
                    while (bElem < bBound && currBIndex[bElem] < currAIndex[aElem]) {
                        bElem++;
                    }

                    if (bElem >= bBound) {
                        break;
                    }

                    if (currBIndex[bElem] == currAIndex[aElem]) {
                        partSum += currBValue[bElem] * currAValue[aElem];
                    }
                }

                if (fabs(partSum) >= eps) {
                    currentResult.push_back(partSum);
                    currentResIndex.push_back(j + elementsPerProc * rank);
                }
            }
            // Sending result from current proces
            if (rank != root) {
                int len = static_cast<int>(currentResult.size());
                MPI_Send(&len, 1, MPI_INT, root, 0, comm);
                if (len != 0) {
                    MPI_Send(&currentResult[0], len, MPI_DOUBLE, root, 0, comm);
                    MPI_Send(&currentResIndex[0], len, MPI_INT, root, 0, comm);
                }
            } else {
                std::vector<double> getterValue(2 * n);
                std::vector<int>    getterIndex(2 * n);

                int counter = 0;
                int len = currentResult.size();
                counter += len;

                for (int j = 0; j < len; j++) {
                    result.index.push_back(currentResIndex[j]);
                    result.value.push_back(currentResult[j]);
                }

                for (int ind = 1; ind < size; ind++) {
                    int len = 0;
                    MPI_Recv(&len, 1, MPI_INT, ind, 0, comm, MPI_STATUS_IGNORE);

                    counter += len;

                    if (len != 0) {
                        MPI_Recv(&getterValue[0], len, MPI_DOUBLE, ind, 0, comm, MPI_STATUS_IGNORE);
                        MPI_Recv(&getterIndex[0], len, MPI_INT, ind, 0, comm, MPI_STATUS_IGNORE);
                        for (int j = 0; j < len; j++) {
                            result.index.push_back(getterIndex[j]);
                            result.value.push_back(getterValue[j]);
                        }
                    }
                }
                result.offset.push_back(counter + *(--result.offset.end()));
            }
        }
    }
    return result;
}

void printMatr(const ccsMatrix& check) {
    for (auto x : check.offset) std::cerr << x << ' ';
    std::cerr << std::endl;
    for (auto x : check.index) std::cerr << x << ' ';
    std::cerr << std::endl;
    for (auto x : check.value) std::cerr << x << ' ';
    std::cerr << std::endl;
}

// Copyright 2019 Zhivaev Artem
#include "../../../modules/task_3/zhivaev_a_strassen_alg/strassen_alg.h"
#include <mpi.h>
#include <iostream>
#include <algorithm>
#include <functional>

using std::copy;
using std::transform;
using std::plus;
using std::minus;

void addMatrix(double* mA, double* mB, double* mC, int mSide) {
    transform(mA, mA + mSide * mSide, mB, mC, plus<double>());
}

void subMatrix(double* mA, double* mB, double* mC, int mSide) {
    transform(mA, mA + mSide * mSide, mB, mC, minus<double>());
}

void splitMatrix(double* m, double* m11, double* m12, double* m21, double* m22, int mSide) {
    int fSide = mSide / 2;
    for (int i = 0; i < fSide; i++) {
        copy(m, m + fSide, m11);
        m += fSide;
        m11 += fSide;
        copy(m, m + fSide, m12);
        m += fSide;
        m12 += fSide;
    }
    for (int i = 0; i < fSide; i++) {
        copy(m, m + fSide, m21);
        m += fSide;
        m21 += fSide;
        copy(m, m + fSide, m22);
        m += fSide;
        m22 += fSide;
    }
}

void formMatrix(double* m, double* m11, double* m12, double* m21, double* m22, int mSide) {
    int fSide = mSide / 2;
    for (int i = 0; i < fSide; i++) {
        copy(m11, m11 + fSide, m);
        m11 += fSide;
        m += fSide;
        copy(m12, m12 + fSide, m);
        m12 += fSide;
        m += fSide;
    }
    for (int i = 0; i < fSide; i++) {
        copy(m21, m21 + fSide, m);
        m21 += fSide;
        m += fSide;
        copy(m22, m22 + fSide, m);
        m22 += fSide;
        m += fSide;
    }
}

void multSeq(double* mA, double* mB, double* mC, int mSide) {
    for (int i = 0; i < mSide; i++) {
        for (int j = 0; j < mSide; j++) {
            mC[i * mSide + j] = 0;
            for (int k = 0; k < mSide; k++) {
                mC[i * mSide + j] += mA[i * mSide + k] * mB[k * mSide + j];
            }
        }
    }
}

void multStrassen(double* mA, double* mB, double* mC, int mSide, MPI_Comm comm) {
    int size;
    MPI_Comm_size(comm, &size);
    if (size == 1) {
        multStrassenOneProc(mA, mB, mC, mSide);
    } else if (size < 4) {
        multStrassenTwoProc(mA, mB, mC, mSide, comm);
    } else {
        multStrassenFourProc(mA, mB, mC, mSide, comm);
    }
}

void multStrassenTwoProc(double* mA, double* mB, double* mC, int mSide, MPI_Comm comm) {
    int rank;
    MPI_Comm_rank(comm, &rank);

    MPI_Request requests[4], request;
    MPI_Status statuses[4];
    MPI_Datatype subArrayType;
    int fSide = mSide / 2;
    int fLeng = fSide * fSide;

    MPI_Type_vector(fSide, fSide, mSide, MPI_DOUBLE, &subArrayType);
    MPI_Type_commit(&subArrayType);

    double* localA1 = new double[fLeng * 6];
    double* localA2 = localA1 + fLeng;
    double* localB1 = localA2 + fLeng;
    double* localB2 = localB1 + fLeng;
    double* localC1 = localB2 + fLeng;
    double* localC2 = localC1 + fLeng;

    if (rank == 0) {
        double* localB21 = new double[fLeng * 3];
        double* localA22 = localB21 + fLeng;
        double* localB22 = localA22 + fLeng;
        double* localT1 = new double[fLeng * 2];
        double* localT2 = localT1 + fLeng;
        double* localM1 = new double[fLeng * 4];
        double* localM2 = localM1 + fLeng;
        double* localM3 = localM2 + fLeng;
        double* localM4 = localM3 + fLeng;

        MPI_Isend(mA,                         1, subArrayType, 0, 0, comm, &request);  // Sending A11 to rank0
        MPI_Isend(mB,                         1, subArrayType, 0, 1, comm, &request);  // Sending B11 to rank0
        MPI_Isend(mA + fSide,                 1, subArrayType, 0, 2, comm, &request);  // Sending A12 to rank0
        MPI_Isend(mB + fSide,                 1, subArrayType, 0, 3, comm, &request);  // Sending B12 to rank0

        MPI_Isend(mA + fSide * mSide,         1, subArrayType, 1, 0, comm, &request);  // Sending A21 to rank1
        MPI_Isend(mB + fSide * mSide,         1, subArrayType, 1, 1, comm, &request);  // Sending B21 to rank1
        MPI_Isend(mA + fSide * mSide + fSide, 1, subArrayType, 1, 2, comm, &request);  // Sending A22 to rank1
        MPI_Isend(mB + fSide * mSide + fSide, 1, subArrayType, 1, 3, comm, &request);  // Sending B22 to rank1

        MPI_Irecv(localA1, fLeng, MPI_DOUBLE, 0, 0, comm, &requests[0]);  // Recieving A11 from rank0
        MPI_Irecv(localB1, fLeng, MPI_DOUBLE, 0, 1, comm, &requests[1]);  // Recieving B11 from rank0
        MPI_Irecv(localA2, fLeng, MPI_DOUBLE, 0, 2, comm, &requests[2]);  // Recieving A12 from rank0
        MPI_Irecv(localB2, fLeng, MPI_DOUBLE, 0, 3, comm, &requests[3]);  // Recieving B12 from rank0
        MPI_Waitall(4, requests, statuses);

        MPI_Isend(localA1, fLeng, MPI_DOUBLE, 1, 0, comm, &request);  // Sending A11 to rank1
        MPI_Isend(localB1, fLeng, MPI_DOUBLE, 1, 1, comm, &request);  // Sending B11 to rank1
        MPI_Isend(localB2, fLeng, MPI_DOUBLE, 1, 2, comm, &request);  // Sending B12 to rank1

        MPI_Irecv(localB21, fLeng, MPI_DOUBLE, 1, 0, comm, &requests[0]);  // Recieving B21 from rank1
        MPI_Irecv(localA22, fLeng, MPI_DOUBLE, 1, 1, comm, &requests[1]);  // Recieving A22 from rank1
        MPI_Irecv(localB22, fLeng, MPI_DOUBLE, 1, 2, comm, &requests[2]);  // Recieving B22 from rank1
        MPI_Waitall(3, requests, statuses);

        // Calculating
        // C11 = (A11 + A22)(B11 + B22) + A22(B21 - B11) - (A11 + A12)B22 + (A12 - A22)(B21 + B22)
        // (A11 + A22)(B11 + B22)
        addMatrix(localA1, localA22, localT1, fSide);
        addMatrix(localB1, localB22, localT2, fSide);
        multSeq(localT1, localT2, localM1, fSide);
        // A22(B21 - B11)
        subMatrix(localB21, localB1, localT2, fSide);
        multSeq(localA22, localT2, localM2, fSide);
        // (A11 + A12)B22
        addMatrix(localA1, localA2, localT1, fSide);
        multSeq(localT1, localB22, localM3, fSide);
        // (A12 - A22)(B21 + B22)
        subMatrix(localA2, localA22, localT1, fSide);
        addMatrix(localB21, localB22, localT2, fSide);
        multSeq(localT1, localT2, localM4, fSide);
        // C11
        addMatrix(localM1, localM2, localT1, fSide);
        subMatrix(localM4, localM3, localT2, fSide);
        addMatrix(localT1, localT2, localC1, fSide);
        // C12 = A11(B12 - B22) + (A11 + A12)B22
        // A11(B12 - B22)
        subMatrix(localB2, localB22, localT2, fSide);
        multSeq(localA1, localT2, localM1, fSide);
        // (A11 + A12)B22
        addMatrix(localA1, localA2, localT1, fSide);
        multSeq(localT1, localB22, localM2, fSide);
        // C12
        addMatrix(localM1, localM2, localC2, fSide);

        delete[] localB21;
        delete[] localT1;
        delete[] localM1;

        MPI_Isend(localC1, fLeng, MPI_DOUBLE, 0, 0, comm, &request);
        MPI_Isend(localC2, fLeng, MPI_DOUBLE, 0, 1, comm, &request);

        MPI_Irecv(mC,                         1, subArrayType, 0, 0, comm, &requests[0]);  // Recieving C11 from rank0
        MPI_Irecv(mC + fSide,                 1, subArrayType, 0, 1, comm, &requests[1]);  // Recieving C12 from rank0
        MPI_Irecv(mC + fSide * mSide,         1, subArrayType, 1, 0, comm, &requests[2]);  // Recieving C21 from rank1
        MPI_Irecv(mC + fSide * mSide + fSide, 1, subArrayType, 1, 1, comm, &requests[3]);  // Recieving C22 from rank1
        MPI_Waitall(4, requests, statuses);
    } else if (rank == 1) {
        double* localA11 = new double[fLeng * 3];
        double* localB11 = localA11 + fLeng;
        double* localB12 = localB11 + fLeng;
        double* localT1 = new double[fLeng * 2];
        double* localT2 = localT1 + fLeng;
        double* localM1 = new double[fLeng * 4];
        double* localM2 = localM1 + fLeng;
        double* localM3 = localM2 + fLeng;
        double* localM4 = localM3 + fLeng;

        MPI_Irecv(localA1, fLeng, MPI_DOUBLE, 0, 0, comm, &requests[0]);  // Recieving A21 from rank1
        MPI_Irecv(localB1, fLeng, MPI_DOUBLE, 0, 1, comm, &requests[1]);  // Recieving B21 from rank1
        MPI_Irecv(localA2, fLeng, MPI_DOUBLE, 0, 2, comm, &requests[2]);  // Recieving A22 from rank1
        MPI_Irecv(localB2, fLeng, MPI_DOUBLE, 0, 3, comm, &requests[3]);  // Recieving B22 from rank1
        MPI_Waitall(4, requests, statuses);

        MPI_Isend(localB1, fLeng, MPI_DOUBLE, 0, 0, comm, &request);  // Sending B21 to rank0
        MPI_Isend(localA2, fLeng, MPI_DOUBLE, 0, 1, comm, &request);  // Sending A22 to rank0
        MPI_Isend(localB2, fLeng, MPI_DOUBLE, 0, 2, comm, &request);  // Sending B22 to rank0

        MPI_Irecv(localA11, fLeng, MPI_DOUBLE, 0, 0, comm, &requests[0]);  // Recieving A11 from rank0
        MPI_Irecv(localB11, fLeng, MPI_DOUBLE, 0, 1, comm, &requests[1]);  // Recieving B11 from rank0
        MPI_Irecv(localB12, fLeng, MPI_DOUBLE, 0, 2, comm, &requests[2]);  // Recieving B12 from rank0
        MPI_Waitall(3, requests, statuses);

        // Calculating
        // C21 = (A21 + A22)B11 + A22(B21 - B11)
        // (A21 + A22)B11
        addMatrix(localA1, localA2, localT1, fSide);
        multSeq(localT1, localB11, localM1, fSide);
        // A22(B21 - B11)
        subMatrix(localB1, localB11, localT2, fSide);
        multSeq(localA2, localT2, localM2, fSide);
        // C21
        addMatrix(localM1, localM2, localC1, fSide);
        // C22 = (A11 + A22)(B11 + B22) - (A21 + A22)B11 + A11(B12 - B22) + (A21 - A11)(B11 + B12)
        // (A11 + A22)(B11 + B22)
        addMatrix(localA11, localA2, localT1, fSide);
        addMatrix(localB11, localB2, localT2, fSide);
        multSeq(localT1, localT2, localM1, fSide);
        // (A21 + A22)B11
        addMatrix(localA1, localA2, localT1, fSide);
        multSeq(localT1, localB11, localM2, fSide);
        // A11(B12 - B22)
        subMatrix(localB12, localB2, localT2, fSide);
        multSeq(localA11, localT2, localM3, fSide);
        // (A21 - A11)(B11 + B12)
        subMatrix(localA1, localA11, localT1, fSide);
        addMatrix(localB11, localB12, localT2, fSide);
        multSeq(localT1, localT2, localM4, fSide);
        // C22
        subMatrix(localM1, localM2, localT1, fSide);
        addMatrix(localM3, localM4, localT2, fSide);
        addMatrix(localT1, localT2, localC2, fSide);

        delete[] localA11;
        delete[] localT1;
        delete[] localM1;

        MPI_Isend(localC1, fLeng, MPI_DOUBLE, 0, 0, comm, &request);
        MPI_Isend(localC2, fLeng, MPI_DOUBLE, 0, 1, comm, &request);
    }

    MPI_Type_free(&subArrayType);

    delete[] localA1;
}

void multStrassenOneProc(double* mA, double* mB, double* mC, int mSide) {
    if (mSide <= 32) {
        multSeq(mA, mB, mC, mSide);
        return;
    }
    int fSide = mSide / 2;
    int fLeng = fSide * fSide;

    double* mA11 = new double[fLeng * 4];
    double* mA12 = mA11 + fLeng;
    double* mA21 = mA12 + fLeng;
    double* mA22 = mA21 + fLeng;
    double* mB11 = new double[fLeng * 4];
    double* mB12 = mB11 + fLeng;
    double* mB21 = mB12 + fLeng;
    double* mB22 = mB21 + fLeng;
    double* mC11 = new double[fLeng * 4];
    double* mC12 = mC11 + fLeng;
    double* mC21 = mC12 + fLeng;
    double* mC22 = mC21 + fLeng;

    double* mM1 = new double[fLeng * 7];
    double* mM2 = mM1 + fLeng;
    double* mM3 = mM2 + fLeng;
    double* mM4 = mM3 + fLeng;
    double* mM5 = mM4 + fLeng;
    double* mM6 = mM5 + fLeng;
    double* mM7 = mM6 + fLeng;

    double* mT1 = new double[fLeng * 2];
    double* mT2 = mT1 + fLeng;

    splitMatrix(mA, mA11, mA12, mA21, mA22, mSide);
    splitMatrix(mB, mB11, mB12, mB21, mB22, mSide);

    // M1 = (A11 + A22)(B11 + B22)
    // M2 = (A21 + A22)B11
    // M3 = A11(B12 + B22)
    // M4 = A22(B21 - B11)
    // M5 = (A11 + A12)B22
    // M6 = (A21 - A11)(B11 + B12)
    // M7 = (A12 - A22)(B21 + B22)

    // Calculate M1
    addMatrix(mA11, mA22, mT1, fSide);
    addMatrix(mB11, mB22, mT2, fSide);
    multStrassenOneProc(mT1, mT2, mM1, fSide);

    // Calculate M2
    addMatrix(mA21, mA22, mT1, fSide);
    multStrassenOneProc(mT1, mB11, mM2, fSide);

    // Calculate M3
    subMatrix(mB12, mB22, mT2, fSide);
    multStrassenOneProc(mA11, mT2, mM3, fSide);

    // Calculate M4
    subMatrix(mB21, mB11, mT2, fSide);
    multStrassenOneProc(mA22, mT2, mM4, fSide);

    // Calculate M5
    addMatrix(mA11, mA12, mT1, fSide);
    multStrassenOneProc(mT1, mB22, mM5, fSide);

    // Calculate M6
    subMatrix(mA21, mA11, mT1, fSide);
    addMatrix(mB11, mB12, mT2, fSide);
    multStrassenOneProc(mT1, mT2, mM6, fSide);

    // Calculate M7
    subMatrix(mA12, mA22, mT1, fSide);
    addMatrix(mB21, mB22, mT2, fSide);
    multStrassenOneProc(mT1, mT2, mM7, fSide);

    // C11 = M1 + M4 - M5 + M7
    // C12 = M3 + M5
    // C21 = M2 + M4
    // C22 = M1 - M2 + M3 + M6

    // Calculate C11
    addMatrix(mM1, mM4, mT1, fSide);
    subMatrix(mM7, mM5, mT2, fSide);
    addMatrix(mT1, mT2, mC11, fSide);

    // Calculate C12
    addMatrix(mM3, mM5, mC12, fSide);

    // Calculate C21
    addMatrix(mM2, mM4, mC21, fSide);

    // Calculate C22
    subMatrix(mM1, mM2, mT1, fSide);
    addMatrix(mM3, mM6, mT2, fSide);
    addMatrix(mT1, mT2, mC22, fSide);

    formMatrix(mC, mC11, mC12, mC21, mC22, mSide);

    delete[] mA11;
    delete[] mB11;
    delete[] mC11;
    delete[] mM1;
}

void multStrassenFourProc(double* mA, double* mB, double* mC, int mSide, MPI_Comm comm) {
    int rank;
    MPI_Comm_rank(comm, &rank);

    MPI_Request requests[4], request;
    MPI_Status statuses[4], status;
    MPI_Datatype subArrayType;
    int fSide = mSide / 2;
    int fLeng = fSide * fSide;

    MPI_Type_vector(fSide, fSide, mSide , MPI_DOUBLE, &subArrayType);
    MPI_Type_commit(&subArrayType);

    double* localA = new double[fLeng * 3];
    double* localB = localA + fLeng;
    double* localC = localB + fLeng;

    if (rank == 0) {
        double* localA12 = new double[fLeng * 4];
        double* localB21 = localA12 + fLeng;
        double* localA22 = localB21 + fLeng;
        double* localB22 = localA22 + fLeng;
        double* localT1 = new double[fLeng * 2];
        double* localT2 = localT1 + fLeng;
        double* localM1 = new double[fLeng * 4];
        double* localM2 = localM1 + fLeng;
        double* localM3 = localM2 + fLeng;
        double* localM4 = localM3 + fLeng;

        MPI_Isend(mA,                         1, subArrayType, 0, 0, comm, &request);  // Sending A11 to itself
        MPI_Isend(mB,                         1, subArrayType, 0, 1, comm, &request);  // Sending B11 to itself

        MPI_Isend(mA + fSide,                 1, subArrayType, 1, 0, comm, &request);  // Sending A12 to rank 1
        MPI_Isend(mB + fSide,                 1, subArrayType, 1, 1, comm, &request);  // Sending B12 to rank 1

        MPI_Isend(mA + fSide * mSide,         1, subArrayType, 2, 0, comm, &request);  // Sending A21 to rank 2
        MPI_Isend(mB + fSide * mSide,         1, subArrayType, 2, 1, comm, &request);  // Sending B21 to rank 2

        MPI_Isend(mA + fSide * mSide + fSide, 1, subArrayType, 3, 0, comm, &request);  // Sending A22 to rank 3
        MPI_Isend(mB + fSide * mSide + fSide, 1, subArrayType, 3, 1, comm, &request);  // Sending B22 to rank 3

        MPI_Irecv(localA, fLeng, MPI_DOUBLE, 0, 0, comm, &requests[0]);  // Receiving A11
        MPI_Irecv(localB, fLeng, MPI_DOUBLE, 0, 1, comm, &requests[1]);  // Receiving B11
        MPI_Waitall(2, requests, statuses);

        MPI_Isend(localA, fLeng, MPI_DOUBLE, 1, 0, comm, &request);  // Sending A11 to rank1
        MPI_Isend(localB, fLeng, MPI_DOUBLE, 2, 0, comm, &request);  // Sending B11 to rank2
        MPI_Isend(localA, fLeng, MPI_DOUBLE, 3, 0, comm, &request);  // Sending A11 to rank3
        MPI_Isend(localB, fLeng, MPI_DOUBLE, 3, 1, comm, &request);  // Sending B11 to rank3

        MPI_Irecv(localA12, fLeng, MPI_DOUBLE, 1, 0, comm, &requests[0]);  // Recieving A12 from rank1
        MPI_Irecv(localB21, fLeng, MPI_DOUBLE, 2, 0, comm, &requests[1]);  // Recieving B21 from rank2
        MPI_Irecv(localA22, fLeng, MPI_DOUBLE, 3, 0, comm, &requests[2]);  // Recieving A22 from rank3
        MPI_Irecv(localB22, fLeng, MPI_DOUBLE, 3, 1, comm, &requests[3]);  // Recieving B22 from rank3
        MPI_Waitall(4, requests, statuses);

        // Calculating
        // C11 = (A11 + A22)(B11 + B22) + A22(B21 - B11) - (A11 + A12)B22 + (A12 - A22)(B21 + B22)
        // (A11 + A22)(B11 + B22)
        addMatrix(localA, localA22, localT1, fSide);
        addMatrix(localB, localB22, localT2, fSide);
        multStrassenOneProc(localT1, localT2, localM1, fSide);
        // A22(B21 - B11)
        subMatrix(localB21, localB, localT2, fSide);
        multStrassenOneProc(localA22, localT2, localM2, fSide);
        // (A11 + A12)B22
        addMatrix(localA, localA12, localT1, fSide);
        multStrassenOneProc(localT1, localB22, localM3, fSide);
        // (A12 - A22)(B21 + B22)
        subMatrix(localA12, localA22, localT1, fSide);
        addMatrix(localB21, localB22, localT2, fSide);
        multStrassenOneProc(localT1, localT2, localM4, fSide);
        // C11
        addMatrix(localM1, localM2, localT1, fSide);
        subMatrix(localM4, localM3, localT2, fSide);
        addMatrix(localT1, localT2, localC, fSide);

        delete[] localA12;
        delete[] localT1;
        delete[] localM1;

        // Assemble matrix C
        MPI_Isend(localC, fLeng, MPI_DOUBLE, 0, 0, comm, &request);

        MPI_Irecv(mC,                         1, subArrayType, 0, 0, comm, &requests[0]);  // Recieving C11
        MPI_Irecv(mC + fSide,                 1, subArrayType, 1, 0, comm, &requests[1]);  // Recieving C12
        MPI_Irecv(mC + fSide * mSide,         1, subArrayType, 2, 0, comm, &requests[2]);  // Recieving C21
        MPI_Irecv(mC + fSide * mSide + fSide, 1, subArrayType, 3, 0, comm, &requests[3]);  // Recieving C22

        MPI_Waitall(4, requests, statuses);
    } else if (rank == 1) {
        double* localA11 = new double[fLeng * 2];
        double* localB22 = localA11 + fLeng;
        double* localT = new double[fLeng];
        double* localM1 = new double[fLeng * 2];
        double* localM2 = localM1 + fLeng;

        MPI_Irecv(localA, fLeng, MPI_DOUBLE, 0, 0, comm, &requests[0]);  // Recieving A12
        MPI_Irecv(localB, fLeng, MPI_DOUBLE, 0, 1, comm, &requests[1]);  // Recieving B12
        MPI_Waitall(2, requests, statuses);

        MPI_Isend(localA, fLeng, MPI_DOUBLE, 0, 0, comm, &request);  // Sending A12 to rank0
        MPI_Isend(localB, fLeng, MPI_DOUBLE, 3, 0, comm, &request);  // Sending B12 to rank3

        MPI_Irecv(localA11, fLeng, MPI_DOUBLE, 0, 0, comm, &requests[0]);  // Recieving A11 from rank0
        MPI_Irecv(localB22, fLeng, MPI_DOUBLE, 3, 0, comm, &requests[1]);  // Recieving B22 from rank3
        MPI_Waitall(2, requests, statuses);

        // Calculating
        // C12 = A11(B12 - B22) + (A11 + A12)B22
        // A11(B12 - B22)
        subMatrix(localB, localB22, localT, fSide);
        multStrassenOneProc(localA11, localT, localM1, fSide);
        // (A11 + A12)B22
        addMatrix(localA11, localA, localT, fSide);
        multStrassenOneProc(localT, localB22, localM2, fSide);
        // C12
        addMatrix(localM1, localM2, localC, fSide);

        delete[] localA11;
        delete[] localT;
        delete[] localM1;

        // Assemble matrix C
        MPI_Isend(localC, fLeng, MPI_DOUBLE, 0, 0, comm, &request);
        MPI_Wait(&request, &status);
    } else if (rank == 2) {
        double* localB11 = new double[fLeng * 2];
        double* localA22 = localB11 + fLeng;
        double* localT = new double[fLeng];
        double* localM1 = new double[fLeng * 2];
        double* localM2 = localM1 + fLeng;

        MPI_Irecv(localA, fLeng, MPI_DOUBLE, 0, 0, comm, &requests[0]);  // Recieving A21
        MPI_Irecv(localB, fLeng, MPI_DOUBLE, 0, 1, comm, &requests[1]);  // Recieving B21
        MPI_Waitall(2, requests, statuses);

        MPI_Isend(localB, fLeng, MPI_DOUBLE, 0, 0, comm, &request);  // Sending B21 to rank0
        MPI_Isend(localA, fLeng, MPI_DOUBLE, 3, 0, comm, &request);  // Sending A21 to rank3

        MPI_Irecv(localB11, fLeng, MPI_DOUBLE, 0, 0, comm, &requests[0]);  // Recieving B11 from rank0
        MPI_Irecv(localA22, fLeng, MPI_DOUBLE, 3, 0, comm, &requests[1]);  // Recieving A22 from rank3
        MPI_Waitall(2, requests, statuses);

        // Calculating
        // C21 = (A21 + A22)B11 + A22(B21 - B11)
        // (A21 + A22)B11
        addMatrix(localA, localA22, localT, fSide);
        multStrassenOneProc(localT, localB11, localM1, fSide);
        // A22(B21 - B11)
        subMatrix(localB, localB11, localT, fSide);
        multStrassenOneProc(localA22, localT, localM2, fSide);
        // C21
        addMatrix(localM1, localM2, localC, fSide);

        delete[] localB11;
        delete[] localT;
        delete[] localM1;

        // Assemble matrix C
        MPI_Isend(localC, fLeng, MPI_DOUBLE, 0, 0, comm, &request);
        MPI_Wait(&request, &status);
    } else if (rank == 3) {
        double* localA11 = new double[fLeng * 4];
        double* localB11 = localA11 + fLeng;
        double* localB12 = localB11 + fLeng;
        double* localA21 = localB12 + fLeng;
        double* localT1 = new double[fLeng * 2];
        double* localT2 = localT1 + fLeng;
        double* localM1 = new double[fLeng * 4];
        double* localM2 = localM1 + fLeng;
        double* localM3 = localM2 + fLeng;
        double* localM4 = localM3 + fLeng;

        MPI_Irecv(localA, fLeng, MPI_DOUBLE, 0, 0, comm, &requests[0]);  // Recieving A22
        MPI_Irecv(localB, fLeng, MPI_DOUBLE, 0, 1, comm, &requests[1]);  // Recieving B22
        MPI_Waitall(2, requests, statuses);

        MPI_Isend(localA, fLeng, MPI_DOUBLE, 0, 0, comm, &request);  // Sending A22 to rank0
        MPI_Isend(localB, fLeng, MPI_DOUBLE, 0, 1, comm, &request);  // Sending B22 to rank0
        MPI_Isend(localB, fLeng, MPI_DOUBLE, 1, 0, comm, &request);  // Sending B22 to rank1
        MPI_Isend(localA, fLeng, MPI_DOUBLE, 2, 0, comm, &request);  // Sending A22 to rank0

        MPI_Irecv(localA11, fLeng, MPI_DOUBLE, 0, 0, comm, &requests[0]);  // Recieving A11 from rank0
        MPI_Irecv(localB11, fLeng, MPI_DOUBLE, 0, 1, comm, &requests[1]);  // Recieving B11 from rank0
        MPI_Irecv(localB12, fLeng, MPI_DOUBLE, 1, 0, comm, &requests[2]);  // Recieving B12 from rank1
        MPI_Irecv(localA21, fLeng, MPI_DOUBLE, 2, 0, comm, &requests[3]);  // Recieving A21 from rank2
        MPI_Waitall(4, requests, statuses);

        // Calculating
        // C22 = (A11 + A22)(B11 + B22) - (A21 + A22)B11 + A11(B12 - B22) + (A21 - A11)(B11 + B12)
        // (A11 + A22)(B11 + B22)
        addMatrix(localA11, localA, localT1, fSide);
        addMatrix(localB11, localB, localT2, fSide);
        multStrassenOneProc(localT1, localT2, localM1, fSide);
        // (A21 + A22)B11
        addMatrix(localA21, localA, localT1, fSide);
        multStrassenOneProc(localT1, localB11, localM2, fSide);
        // A11(B12 - B22)
        subMatrix(localB12, localB, localT2, fSide);
        multStrassenOneProc(localA11, localT2, localM3, fSide);
        // (A21 - A11)(B11 + B12)
        subMatrix(localA21, localA11, localT1, fSide);
        addMatrix(localB11, localB12, localT2, fSide);
        multStrassenOneProc(localT1, localT2, localM4, fSide);
        // C22
        subMatrix(localM1, localM2, localT1, fSide);
        addMatrix(localM3, localM4, localT2, fSide);
        addMatrix(localT1, localT2, localC, fSide);

        delete[] localA11;
        delete[] localT1;
        delete[] localM1;

        // Assemble matrix C
        MPI_Isend(localC, fLeng, MPI_DOUBLE, 0, 0, comm, &request);
        MPI_Wait(&request, &status);
    }

    MPI_Type_free(&subArrayType);

    delete[] localA;
}



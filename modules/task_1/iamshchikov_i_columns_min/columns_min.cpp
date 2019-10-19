// Copyright 2019 Iamshchikov Ivan
#include <random>
#include <vector>
#include <ctime>
#include "../../../modules/task_1/iamshchikov_i_columns_min/columns_min.h"

void getRandomMatrix(std::vector<std::vector<int> >* matrix, int n) {
    if (n > 0 && matrix->size() > 0) {
        std::mt19937 gen;
        gen.seed(static_cast<unsigned int>(time(0)));
        for (size_t i = 0; i < matrix->size(); ++i)
            for (int j = 0; j < n; ++j)
                matrix->at(i).push_back(gen() % 100);
    } else {
         throw - 1;
    }
}

void matrixToRow(const std::vector<std::vector<int> >* matrix,
                 std::vector<int>* row, int row_num, int col_num) {
    int k = 0;
    for (int i = 0; i < col_num; ++i)
        for (int j = 0; j < row_num; ++j, ++k)
            row->at(k) = (matrix->at(j))[i];
}

int getMinElem(int* arr, int n) {
    if (n > 0) {
        int min = arr[0];
        for (int i = 1; i < n; ++i)
            if (arr[i] < min)
                min = arr[i];
        return min;
    } else {
        throw -1;
    }
}

int getInvolvedProcNumber(int n, int procnum) {
    if (n / procnum == 0) return n;
    else if (n / procnum > 0) return procnum;
    else
        throw -1;
}

MPI_Comm chooseComm(int n, int procnum, MPI_Comm c) {
    if (c != MPI_COMM_NULL) return c;
    else
        return MPI_COMM_WORLD;
}

void setSendcount(std::vector<int>* elem_count, std::vector<int>* col_count,
                  int k, int n, int m) {
    int count = n / k;
    if (n%k != 0) {
        for (int i = 0; i < n%k; ++i) {
            elem_count->push_back((count + 1)*m);
            col_count->push_back(count + 1);
        }
        for (int i = n % k; i < k; ++i) {
            elem_count->push_back(count*m);
            col_count->push_back(count);
        }
    } else {
        for (int i = 0; i < k; ++i) {
            elem_count->push_back(count*m);
            col_count->push_back(count);
        }
    }
}

void setDispl(std::vector<int>* elem_displ, std::vector<int>* elem_count,
              std::vector<int>* col_displ, std::vector<int>* col_count) {
    int tmp_e = 0;
    int tmp_c = 0;
    elem_displ->push_back(0);
    col_displ->push_back(0);
    for (size_t i = 1; i < elem_count->size(); ++i) {
        tmp_e += elem_count->at(i - 1);
        tmp_c += col_count->at(i - 1);
        elem_displ->push_back(tmp_e);
        col_displ->push_back(tmp_c);
    }
}

std::vector<int> getMinElementsSequential(
                 const std::vector<std::vector<int> >* matrix,
                 int row_num, int col_num) {
    int min;
    std::vector<int> res(col_num);
    for (int i = 0; i < col_num; ++i) {
        min = (matrix->at(0))[i];
        for (int j = 1; j < row_num; ++j)
            if ((matrix->at(j))[i] < min) min = (matrix->at(j))[i];
        res[i] = min;
    }
    return res;
}

std::vector<int> getMinElementsParallel(
                 const std::vector<std::vector<int> >* matrix,
                 int row_num, int col_num) {
    MPI_Comm new_comm = MPI_COMM_NULL;
    MPI_Group group_world, new_group;
    int ProcRank, ProcNum, involved_proc_num;
    const int m = row_num, n = col_num;
    std::vector<int> global_res(n), matrix_in_row(m*n), local_buf, local_res,
                    elem_count, col_count, elem_displ, col_displ, member;
    std::vector<int> *pmatrix_in_row = &matrix_in_row,
                    *plocal_buf = &local_buf, *plocal_res = &local_res,
                    *pelem_count = &elem_count, *pcol_count = &col_count,
                    *pelem_displ = &elem_displ, *pcol_displ = &col_displ,
                    *pmember = &member;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    involved_proc_num = getInvolvedProcNumber(n, ProcNum);
    if (ProcRank == 0) {
        matrixToRow(matrix, pmatrix_in_row, m, n);
    }
    if (involved_proc_num < ProcNum) {
        MPI_Comm_group(MPI_COMM_WORLD, &group_world);
        for (int i = 0; i < involved_proc_num; ++i)
            pmember->push_back(i);
        MPI_Group_incl(group_world, involved_proc_num, pmember->data(),
                       &new_group);
        MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);
    }
    setSendcount(pelem_count, pcol_count, involved_proc_num, n, m);
    setDispl(pelem_displ, pelem_count, pcol_displ, pcol_count);
    if (ProcRank < involved_proc_num) {
        plocal_buf->resize(pelem_count->at(ProcRank));
        MPI_Scatterv(pmatrix_in_row->data(), pelem_count->data(),
                     pelem_displ->data(), MPI_INT, plocal_buf->data(),
                     pelem_count->at(ProcRank), MPI_INT, 0,
                     chooseComm(involved_proc_num, ProcNum, new_comm));
        for (int i = 0; i < pcol_count->at(ProcRank); ++i)
            plocal_res->push_back(getMinElem(plocal_buf->data() + m * i, m));
        MPI_Gatherv(plocal_res->data(), pcol_count->at(ProcRank), MPI_INT,
                    global_res.data(), pcol_count->data(), pcol_displ->data(),
                    MPI_INT, 0, chooseComm(involved_proc_num, ProcNum, new_comm));
    }
    return global_res;
}

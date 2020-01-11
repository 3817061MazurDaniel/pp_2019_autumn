// Copyright 2019 Vedrukov Pavel

#include <mpi.h>
#include <string>
#include "../../../modules/task_1/vedrukov_p_freq_symbol_in_a_str/freq_symb.h"

double getFreq(std::string str, char symb) {
    int Proc_num;
    int Proc_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &Proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &Proc_rank);
    if (Proc_num <= 2) {
        int num = 0;
        for (int i = 0; i < static_cast<int>(str.length()); i++) {
            if (str[i] == symb)
                num++;
        }
        return static_cast<double>(num) / str.length();
    }
    int substrlen = static_cast<int>(str.length()) / (Proc_num - 2);
    int last_substrlen = static_cast<int>(str.length()) % (Proc_num - 2);
    int num_symb = 0;
    int all_num_sumb = 0;
    std::string substr("", substrlen);
    MPI_Status status;
    MPI_Barrier(MPI_COMM_WORLD);
    if (Proc_rank == 0) {
        for (int i = 1; i < Proc_num - 1; i++) {
            MPI_Send(&str[(i - 1) * substrlen], substrlen, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
        if (last_substrlen != 0) {
            MPI_Send(&str[substrlen * (Proc_num - 2)], last_substrlen, MPI_CHAR, Proc_num - 1, 1, MPI_COMM_WORLD);
        }
        for (int i = 1; i < Proc_num; i++) {
            MPI_Recv(&num_symb, 1, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status);
            all_num_sumb += num_symb;
        }
    } else {
        num_symb = 0;
        if (Proc_rank > 0 && Proc_rank < Proc_num - 1) {
            MPI_Recv(&substr[0], substrlen, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
            for (int i = 0; i < substrlen; i++) {
                if (substr[i] == symb)
                    num_symb++;
            }
        }
        if (last_substrlen != 0) {
            if (Proc_rank == Proc_num - 1) {
                substr.resize(last_substrlen);
                MPI_Recv(&substr[0], last_substrlen, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
                for (int i = 0; i < substrlen; i++) {
                    if (substr[i] == symb)
                        num_symb++;
                }
            }
        }
        MPI_Send(&num_symb, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }
    return static_cast<double>(all_num_sumb) / str.length();
}

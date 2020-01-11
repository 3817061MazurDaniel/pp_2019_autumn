// Copyright 2019 Andronov Maxim
#include <random>
#include <vector>
#include <utility>
#include "../../../modules/task_3/andronov_m_radix_with_batcher_split/radix_with_batcher_split.h"

std::vector<int> GetRandomVector(int size) {
    if (size < 1)
        throw - 1;

    std::vector <int> vec;

    vec.resize(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution <int> dist(-100, 100);

    for (int i = 0; i < size; i++)
        vec[i] = dist(gen);

    return vec;
}

unsigned char GetByte(int value, int number) {
    return (value >> (number * 8)) & 255;
}

std::vector<int> RadixSort(std::vector<int> array) {
    if (array.size() < 1)
        throw - 1;
    if (array.size() == 1)
        return array;

    std::vector<int> tmp(array.size());
    std::vector<int> counter(256, 0);
    std::vector<int> offset(256, 0);
    bool flag = true;

    for (int i = 0; i < 3; i++) {
        for (size_t j = 0; j < array.size(); j++) {
            unsigned char byte = GetByte(array[j], i);
            counter[byte]++;
        }
        offset[0] = 0;
        for (int k = 1; k < 256; k++) {
            offset[k] = counter[k - 1] + offset[k - 1];
        }
        for (size_t j = 0; j < array.size(); j++) {
            if (flag) {
                unsigned char value = GetByte(array[j], i);
                tmp[offset[value]] = array[j];
                offset[value]++;
            } else {
                int value = GetByte(tmp[j], i);
                array[offset[value]] = tmp[j];
                offset[value]++;
            }
        }
        flag = !flag;
        for (int k = 0; k < 256; k++) {
            counter[k] = 0;
        }
    }
    for (size_t j = 0; j < array.size(); j++) {
        char byte = static_cast<char>(GetByte(array[j], 3));
        counter[byte + 128]++;
    }
    offset[0] = 0;
    for (int k = 1; k < 256; k++) {
        offset[k] = counter[k - 1] + offset[k - 1];
    }
    for (size_t j = 0; j < array.size(); j++) {
        char value = GetByte(tmp[j], 3);
        array[offset[value + 128]] = tmp[j];
        offset[value + 128]++;
    }

    return array;
}

std::vector<int> EvenSplit(std::vector<int> arr1, std::vector<int> arr2) {
    int arr1_size = arr1.size();
    int arr2_size = arr2.size();
    int result_size = arr1_size / 2 + arr2_size / 2
                            + arr1_size % 2 + arr2_size % 2;
    std::vector<int> result(result_size);
    int i = 0;
    int j = 0, k = 0;

    while ((j < arr1_size) && (k < arr2_size)) {
        if (arr1[j] <= arr2[k]) {
            result[i] = arr1[j];
            j += 2;
        } else {
            result[i] = arr2[k];
            k += 2;
        }
        i++;
    }

    if (j >= arr1_size) {
        for (int l = k; l < arr2_size; l += 2) {
            result[i] = arr2[l];
            i++;
        }
    } else {
        for (int l = j; l < arr1_size; l += 2) {
            result[i] = arr1[l];
            i++;
        }
    }

    return result;
}

std::vector<int> OddSplit(std::vector<int> arr1, std::vector<int> arr2) {
    int arr1_size = arr1.size();
    int arr2_size = arr2.size();
    int result_size = arr1_size / 2 + arr2_size / 2;
    std::vector<int> result(result_size);
    int i = 0;
    int j = 1, k = 1;

    while ((j < arr1_size) && (k < arr2_size)) {
        if (arr1[j] <= arr2[k]) {
            result[i] = arr1[j];
            j += 2;
        } else {
            result[i] = arr2[k];
            k += 2;
        }
        i++;
    }

    if (j >= arr1_size) {
        for (int l = k; l < arr2_size; l += 2) {
            result[i] = arr2[l];
            i++;
        }
    } else {
        for (int l = j; l < arr1_size; l += 2) {
            result[i] = arr1[l];
            i++;
        }
    }

    return result;
}

// arr1 - even elements | arr2 - odd elements
std::vector<int> EvenOddSplit(std::vector<int> arr1, std::vector<int> arr2) {
    int arr1_size = arr1.size();
    int arr2_size = arr2.size();
    int result_size = arr1_size + arr2_size;
    std::vector<int> result(result_size);
    int i = 0, j = 0, k = 0;

    while ((j < arr1_size) && (k < arr2_size)) {
        result[i] = arr1[j];
        result[i + 1] = arr2[k];
        i += 2;
        j++;
        k++;
    }

    if ((k >= arr2_size) && (j < arr1_size)) {
        for (int l = i; l < result_size; l++) {
            result[l] = arr1[j];
            j++;
        }
    }

    for (int i = 0; i < result_size - 1; i++) {
        if (result[i] > result[i + 1])
            std::swap(result[i], result[i + 1]);
    }

    return result;
}

std::vector<int> ParallelRadixSortBatcherSplit(std::vector<int> array,
                                                        int size_arr) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size == 1)
        return RadixSort(array);

    MPI_Comm MPI_COMM_TASK = MPI_COMM_NULL;

    if (size_arr < size) {
        MPI_Group global_group, new_group;
        MPI_Comm_group(MPI_COMM_WORLD, &global_group);
        std::vector<int> new_group_ranks;
        for (int i = 0; i < size_arr; i++)
            new_group_ranks.push_back(i);
        MPI_Group_incl(global_group, size_arr, new_group_ranks.data(),
                                                &new_group);
        MPI_Comm_create(MPI_COMM_WORLD, new_group, &MPI_COMM_TASK);
    } else {
        MPI_COMM_TASK = MPI_COMM_WORLD;
    }

    if (MPI_COMM_TASK == MPI_COMM_NULL)
        return array;

    std::vector<int> local_array;

    if (MPI_COMM_TASK != MPI_COMM_NULL) {
        MPI_Comm_size(MPI_COMM_TASK, &size);
        MPI_Comm_rank(MPI_COMM_TASK, &rank);

        int delta = size_arr / size;
        int delta_rem = size_arr % size;

        std::vector<int> sendcounts(size, delta);

        for (int i = 0; i < delta_rem; i++)
            sendcounts[i]++;

        std::vector<int> displ(size, 0);
        for (int i = 1; i < size; i++) {
            displ[i] = displ[i - 1] + sendcounts[i - 1];
        }

        local_array.resize(sendcounts[rank]);
        MPI_Scatterv(array.data(), sendcounts.data(),
            displ.data(), MPI_INT, local_array.data(), sendcounts[rank],
                MPI_INT, 0, MPI_COMM_TASK);

        local_array = RadixSort(local_array);

        int iteration_size = 0;
        int bitmask_size = sizeof(int) * 8;
        if ((1 & size) != 0)
            iteration_size++;
        for (int i = 1; i < bitmask_size; i++) {
            if (((1 << i) & size) != 0)
                iteration_size += i;
        }

        int count_curr_splited_array = 0;
        int curr_last_rank = size - 1;
        int curr_split = 1;
        int current_array_count = 0;
        for (int i = 0; i < iteration_size; i++) {
            if (rank % curr_split == 0) {
                current_array_count = 0;
                for (int k = 0; k < static_cast<int>(sendcounts.size()); k++) {
                    if (sendcounts[k] != 0)
                        current_array_count++;
                }
                if (((rank / curr_split) % 2 == 0) && (rank == curr_last_rank)
                           && (current_array_count % 2 != 0)) {
                    count_curr_splited_array = 0;
                    for (int j = 0; j < size; j += (curr_split * 2)) {
                        if (j + curr_split < size) {
                            sendcounts[j] += sendcounts[j + curr_split];
                            sendcounts[j + curr_split] = 0;
                            count_curr_splited_array++;

                            if ((j + curr_split * 2) >= size) {
                                if ((count_curr_splited_array - 1) % 2 == 0)
                                    curr_last_rank = j;
                                else
                                    curr_last_rank = -1;
                            }
                        } else {
                            if (count_curr_splited_array % 2 == 0)
                                curr_last_rank = j;
                            else
                                curr_last_rank = -1;
                        }
                    }
                    curr_split *= 2;
                    continue;
                }
                if ((rank / curr_split) % 2 == 0) {
                    MPI_Status status;
                    std::vector<int> even_elem(sendcounts[rank + curr_split]);

                    MPI_Send(local_array.data(), sendcounts[rank],
                        MPI_INT, rank + curr_split,
                            curr_split, MPI_COMM_TASK);
                    MPI_Recv(even_elem.data(), sendcounts[rank + curr_split],
                        MPI_INT, rank + curr_split,
                            curr_split, MPI_COMM_TASK, &status);
                    even_elem = EvenSplit(local_array, even_elem);
                    int odd_count = sendcounts[rank] / 2
                            + sendcounts[rank + curr_split] / 2;
                    std::vector<int> odd_result(odd_count);
                    MPI_Recv(odd_result.data(), odd_count,
                        MPI_INT, rank + curr_split,
                            curr_split, MPI_COMM_TASK, &status);
                    local_array = EvenOddSplit(even_elem, odd_result);
                } else {
                    MPI_Status status;
                    std::vector<int> odd_elem(sendcounts[rank - curr_split]);

                    MPI_Recv(odd_elem.data(), sendcounts[rank - curr_split],
                        MPI_INT, rank - curr_split, curr_split,
                            MPI_COMM_TASK, &status);
                    MPI_Send(local_array.data(), sendcounts[rank],
                        MPI_INT, rank - curr_split, curr_split,
                            MPI_COMM_TASK);
                    odd_elem = OddSplit(odd_elem, local_array);
                    int odd_count = sendcounts[rank] / 2
                            + sendcounts[rank - curr_split] / 2;
                    MPI_Send(odd_elem.data(), odd_count,
                        MPI_INT, rank - curr_split, curr_split, MPI_COMM_TASK);
                }
            } else {
                return local_array;
            }

            count_curr_splited_array = 0;
            for (int j = 0; j < size; j += (curr_split*2)) {
                if (j + curr_split < size) {
                    sendcounts[j] += sendcounts[j + curr_split];
                    sendcounts[j + curr_split] = 0;
                    count_curr_splited_array++;

                    if ((j + curr_split * 2) >= size) {
                        if ((count_curr_splited_array - 1) % 2 == 0)
                            curr_last_rank = j;
                        else
                            curr_last_rank = -1;
                    }
                } else {
                    if (count_curr_splited_array % 2 == 0)
                        curr_last_rank = j;
                    else
                        curr_last_rank = -1;
                }
            }
            curr_split *= 2;
        }
    }
    return local_array;
}


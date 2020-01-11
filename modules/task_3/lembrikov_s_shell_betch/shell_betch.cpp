// Copyright 2019 Lembrikov Stepan
#include <../../../modules/task_3/lembrikov_s_shell_betch/shell_betch.h>
#include <algorithm>
#include <vector>
#include <iomanip>

std::vector<int> getOtrVector(int n) {
    std::vector <int> a(n);
    int k = 1;
    for (int i = 0; i < n; i++) {
        k = -k;
        a[i] = 1 * k;
    }
    return a;
}

std::vector<int> getRandomVector(int n) {
    // std::mt19937 engine;
    std::vector<int> a(n);
    std::mt19937 gen(time(0));
    // std::uniform_real_distribution<> urd(0, 1);
    std::uniform_int_distribution <int> urd(0, n);
    // engine.seed(n);
    // int k = 1;
    for (int i = 0; i < n; i++) {
        // k = -k;
        a[i] = urd(gen);
    }
    return a;
}

std::vector <int> Chet_Betch(const std::vector <int> &mas_1,
    const std::vector <int> &mas_2) {
    int size1 = mas_1.size();
    int size2 = mas_2.size();
    int size_res = size1 / 2 + size2 / 2 + size1 % 2 + size2 % 2;
    std::vector <int> mas_res(size_res);
    int it1 = 0;
    int it2 = 0;
    int i = 0;

    while ((it1 < size1) && (it2 < size2)) {
        if (mas_1[it1] <= mas_2[it2]) {
            mas_res[i] = mas_1[it1];
            it1 += 2;
        } else {
            mas_res[i] = mas_2[it2];
            it2 += 2;
        }
        i++;
    }

    if (it1 >= size1) {
        for (int j = it2; j < size2; j += 2) {
            mas_res[i] = mas_2[j];
            i++;
        }
    } else {
        for (int j = it1; j < size1; j += 2) {
            mas_res[i] = mas_1[j];
            i++;
        }
    }
    return mas_res;
}

std::vector <int> Nechet_Betch(const std::vector <int> &mas_1,
    const std::vector <int> &mas_2) {
    int size1 = mas_1.size();
    int size2 = mas_2.size();
    int size_res = size1 / 2 + size2 / 2;
    std::vector <int> mas_res(size_res);
    int it1 = 1;
    int it2 = 1;
    int i = 0;

    while ((it1 < size1) && (it2 < size2)) {
        if (mas_1[it1] <= mas_2[it2]) {
            mas_res[i] = mas_1[it1];
            it1 += 2;
        } else {
            mas_res[i] = mas_2[it2];
            it2 += 2;
        }
        i++;
    }

    if (it1 >= size1) {
        for (int j = it2; j < size2; j += 2) {
            mas_res[i] = mas_2[j];
            i++;
        }
    } else {
        for (int j = it1; j < size1; j += 2) {
            mas_res[i] = mas_1[j];
            i++;
        }
    }
    return mas_res;
}

std::vector <int> Sravnenie_Chet_Nechet(const std::vector <int> &mas_res_1,
    const std::vector <int> &mas_res_2) {
    int size1 = mas_res_1.size();
    int size2 = mas_res_2.size();
    int flag = 0;
    if (size1 - size2 == 2)
        flag = 1;
    if (size2 - size1 == 2)
        flag = 2;
    int size_res = mas_res_1.size() + mas_res_2.size();
    int size_min = size_res / 2 - 1;
    std::vector <int> mas_res(size_res);
    int buf;
    int i = 0;
    if (flag == 0) {
        for (i = 0; i < size1; i++) {
            mas_res[2 * i] = mas_res_1[i];
            mas_res[2 * i + 1] = mas_res_2[i];
        }
    }

    if ((flag == 1) || (flag == 2)) {
        for (i = 0; i < size_min; i++) {
            mas_res[2 * i] = mas_res_1[i];
            mas_res[2 * i + 1] = mas_res_2[i];
        }
    }

    if (flag == 1) {
        mas_res[2 * i] = mas_res_1[i];
        mas_res[2 * i + 1] = mas_res_1[i + 1];
    }

    if (flag == 2) {
        mas_res[2 * i] = mas_res_2[i];
        mas_res[2 * i + 1] = mas_res_2[i + 1];
    }

    for (int i = 1; i < size_res; i++) {
        if (mas_res[i] < mas_res[i - 1]) {
            buf = mas_res[i - 1];
            mas_res[i - 1] = mas_res[i];
            mas_res[i] = buf;
        }
    }

    return mas_res;
}

std::vector <int> ShellSort(const std::vector <int> &mas, int size) {
    int step, i, j, tmp;
    std::vector <int> mas_res(mas);
    for (step = size / 2; step > 0; step /= 2)
        for (i = step; i < size; i++)
            for (j = i - step; j >= 0 && mas_res[j] > mas_res[j + step]; j -= step) {
                tmp = mas_res[j];
                mas_res[j] = mas_res[j + step];
                mas_res[j + step] = tmp;
            }
    return mas_res;
}

std::vector <int> Shell(std::vector <int> mas) {
    int size;
    int rank;
    int ost;
    int flag = 0;
    int ostatok = 0;
    int k = 0;
    int size_mas = mas.size();
    if (size_mas == 1)
        return mas;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if ((size % 2 == 1) && (size > 1)) {
        flag = -1;
    } else if ((size > size_mas) || (size == 1)) {
        flag = 1;
    }
    ostatok = size_mas % size;
    k = size_mas / size;
    std::vector <int> part_mas(k + ostatok, 0);
    MPI_Status status;


    MPI_Bcast(&mas[0], size_mas, MPI_INT, 0, MPI_COMM_WORLD);


    if (flag == 1) {
        mas = ShellSort(mas, size_mas);
        return mas;
    }

    if (flag == 0) {
        for (int i = 0; i < size - 2; i += 2) {
            if (rank == i) {
                part_mas = ShellSort({ mas.cbegin() + i * k, mas.cbegin() + (i + 1) * k }, k);
                std::copy(part_mas.begin(), part_mas.begin() + k, mas.begin() + i * k);
                if (i < size - 1) {
                    MPI_Sendrecv(&part_mas[0], k, MPI_INT, i + 1, 0,
                        &mas[(i + 1) * k], k, MPI_INT, i + 1, 0, MPI_COMM_WORLD, &status);
                }
            }
        }
        for (int i = 1; i < size - 1; i += 2) {
            if (rank == i) {
                part_mas = ShellSort({ mas.cbegin() + i * k, mas.cbegin() + (i + 1) * k }, k);
                std::copy(part_mas.begin(), part_mas.begin() + k, mas.begin() + k * i);
                MPI_Sendrecv(&part_mas[0], k, MPI_INT, i - 1, 0,
                    &mas[(i - 1) * k], k, MPI_INT, i - 1, 0, MPI_COMM_WORLD, &status);
            }
        }

        if (rank == size - 2) {
            part_mas = ShellSort({ mas.cend() - 2 * k - ostatok, mas.cend() - k - ostatok }, k);
            std::copy(part_mas.begin(), part_mas.begin() + k, mas.end() - 2 * k - ostatok);
            MPI_Sendrecv(&part_mas[0], k, MPI_INT, size - 1, 0,
                &mas[(size - 1) * k], k + ostatok, MPI_INT, size - 1, 0, MPI_COMM_WORLD, &status);
        }

        if (rank == size - 1) {
            part_mas = ShellSort({ mas.cend() - k - ostatok, mas.cend() }, k + ostatok);
            std::copy(part_mas.begin(), part_mas.begin() + k + ostatok, mas.end() - k - ostatok);
            MPI_Sendrecv(&part_mas[0], k + ostatok, MPI_INT, size - 2, 0,
                &mas[(size - 2) * k], k, MPI_INT, size - 2, 0, MPI_COMM_WORLD, &status);
        }
    } else if (flag == -1) {
        for (int i = 0; i < size - 1; i += 2) {
            if (rank == i) {
                part_mas = ShellSort({ mas.cbegin() + i * k, mas.cbegin() + (i + 1) * k }, k);
                std::copy(part_mas.begin(), part_mas.begin() + k, mas.begin() + i * k);
                if (i < size - 1) {
                    MPI_Sendrecv(&part_mas[0], k, MPI_INT, i + 1, 0,
                        &mas[(i + 1) * k], k, MPI_INT, i + 1, 0, MPI_COMM_WORLD, &status);
                }
            }
        }
        for (int i = 1; i < size; i += 2) {
            if (rank == i) {
                part_mas = ShellSort({ mas.cbegin() + i * k, mas.cbegin() + (i + 1) * k }, k);
                std::copy(part_mas.begin(), part_mas.begin() + k, mas.begin() + k * i);
                MPI_Sendrecv(&part_mas[0], k, MPI_INT, i - 1, 0,
                    &mas[(i - 1) * k], k, MPI_INT, i - 1, 0, MPI_COMM_WORLD, &status);
            }
        }

        if (rank == size - 1) {
            part_mas = ShellSort({ mas.cend() - k - ostatok, mas.cend() }, k + ostatok);
            std::copy(part_mas.begin(), part_mas.begin() + k + ostatok, mas.end() - k - ostatok);
        }
        MPI_Bcast(&mas[k * (size - 1)], k + ostatok, MPI_INT, size - 1, MPI_COMM_WORLD);
    }

    int iter = 0;
    int count_iter = 0;

    int kolvo_bit = sizeof(int) * 8;
    std::vector <int> bit(kolvo_bit);
    int it_bit = 0;
    for (int i = kolvo_bit - 1; i >= 0; i--) {
        if ((static_cast<bool>((1 << i)  &  size)) == 1) {
            bit[it_bit] = pow(2, i);
            it_bit++;
        }
    }
    int flag_prim_ost = 0;
    int count_otsort_mas = 0;
    if (size > 1) {
        int p = 0;
        while (p != it_bit) {
            if (bit[p] == 1) {
                break;
            }
            k = size_mas / size;
            count_iter = 0;
            count_otsort_mas += bit[p];
            if (count_otsort_mas == size) {
                flag_prim_ost = 1;
            }
            int buf = bit[p];
            while (buf != 1) {
                buf = buf / 2;
                count_iter++;
            }
            iter = 0;

            while (iter < count_iter) {
                if ((k % 2) == 1)
                    ost = 1;
                else
                    ost = 0;
                std::vector <int> res_part_mas1(k + ost);
                std::vector <int> res_part_mas2(k - ost);
                std::vector <int> res_part_mas(2 * k);
                int it_par = (count_otsort_mas - bit[p]) / 2;
                int smesh = (count_otsort_mas - bit[p]) * (size_mas / size);
                int dobavka = 0;
                int it = bit[p] / pow(2, (iter + 1));
                for (int i = 0; i < it; i++) {
                    if (i > 0) {
                        smesh += 2 * k;
                    }
                    if (i == (it - 1)) {
                        if (flag_prim_ost == 1) {
                            if (ost == 0) {
                                res_part_mas1.resize(k + ostatok / 2 + ostatok % 2);
                                res_part_mas2.resize(k + ostatok / 2);
                            }
                            if (ost == 1) {
                                res_part_mas1.resize(k + ost + ostatok / 2);
                                res_part_mas2.resize(k - ost + ostatok / 2 + ostatok % 2);
                            }
                            res_part_mas.resize(2 * k + ostatok);
                            dobavka = ostatok;
                        }
                    }

                    if (rank == it_par * 2) {
                        res_part_mas1 = Chet_Betch({ mas.cbegin() + smesh,
            mas.cbegin() + smesh + k },
                            { mas.cbegin() + smesh + k, mas.cbegin() + smesh + 2 * k + dobavka });
                        MPI_Send(&res_part_mas1[0], res_part_mas1.size(), MPI_INT, it_par * 2 + 1, 0, MPI_COMM_WORLD);
                    }
                    if (rank == it_par * 2 + 1) {
                        MPI_Status status0;
                        res_part_mas2 = Nechet_Betch({ mas.cbegin() + smesh,
            mas.cbegin() + smesh + k },
                            { mas.cbegin() + smesh + k, mas.cbegin() + smesh + 2 * k + dobavka });
                        MPI_Recv(&res_part_mas1[0], res_part_mas1.size(), MPI_INT,
                            it_par * 2, 0, MPI_COMM_WORLD, &status0);
                        res_part_mas = Sravnenie_Chet_Nechet(res_part_mas1, res_part_mas2);
                        MPI_Send(&res_part_mas[0], res_part_mas.size(), MPI_INT, 0, 1, MPI_COMM_WORLD);
                    }
                    if (rank == 0) {
                        MPI_Status status1;
                        if ((size > 1) && ((it_par * 2 + 1) < size)) {
                            MPI_Recv(&mas[smesh], res_part_mas.size(),
                                MPI_INT, it_par * 2 + 1, 1, MPI_COMM_WORLD, &status1);
                        }
                    }
                    MPI_Bcast(&mas[smesh], res_part_mas.size(), MPI_INT, 0, MPI_COMM_WORLD);
                    it_par++;
                }
                k = k * 2;
                iter++;
            }
            p++;
        }
    }

    if ((rank == 0) && (size > 2)) {
        int l = 0;
        std::vector<int> bit_sum(it_bit, 0);
        bit_sum[0] = bit[0];
        for (int i = 1; i < it_bit; i++) {
            bit_sum[i] = bit[i] + bit_sum[i - 1];
        }
        k = size_mas / size;
        while (l < (it_bit - 2)) {
            int k123 = ((bit[l + 1] + bit_sum[l]) / 2) * (size_mas / size);
            std::vector <int> res_part_mas1(k123);
            std::vector <int> res_part_mas2(k123);
            std::vector <int> res_part_mas(2 * k123);
            res_part_mas1 = Chet_Betch({ mas.cbegin(), mas.cbegin() + k * bit_sum[l] },
                { mas.cbegin() + k * bit_sum[l], mas.cbegin() + k * (bit_sum[l] + bit[l + 1]) });
            res_part_mas2 = Nechet_Betch({ mas.cbegin(), mas.cbegin() + k * bit_sum[l] },
                { mas.cbegin() + k * bit_sum[l], mas.cbegin() + k * (bit_sum[l] + bit[l + 1]) });
            res_part_mas = Sravnenie_Chet_Nechet(res_part_mas1, res_part_mas2);
            for (int i = 0; i < 2 * k123; i++) {
                mas[i] = res_part_mas[i];
            }
            l++;
        }
        if (it_bit > 1) {
            std::vector <int> res_part_mas1(size_mas / 2 + size_mas % 2);
            std::vector <int> res_part_mas2(size_mas / 2);
            res_part_mas1 = Chet_Betch({ mas.cbegin(), mas.cbegin() + k * bit_sum[l] },
                { mas.cbegin() + k * bit_sum[l], mas.cend() });
            res_part_mas2 = Nechet_Betch({ mas.cbegin(), mas.cbegin() + k * bit_sum[l] },
                { mas.cbegin() + k * bit_sum[l], mas.cend() });
            mas = Sravnenie_Chet_Nechet(res_part_mas1, res_part_mas2);
        }
    }

    return mas;
}

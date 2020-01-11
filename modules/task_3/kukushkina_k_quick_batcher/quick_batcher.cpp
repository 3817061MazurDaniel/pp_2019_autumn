// Copyright 2019 Kukushkina Ksenia
#include <../../../modules/task_3/kukushkina_k_quick_batcher/quick_batcher.h>
#include <mpi.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

std::vector<std::pair<int, int>> comparators;
static int offset = 0;

void generateRand(std::vector<int>* vec) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)) + offset++);
  for (int i = 0; i < static_cast<int>(vec->size()); i++)
    (*vec)[i] = gen() % 100;
  return;
}

bool isSorted(const std::vector<int>& vec, int n) {
  for (int i = 0; i < n - 1; i++)
    if (vec[i] > vec[i + 1])
      return false;
  return true;
}

int partition(std::vector<int>* vec, int low, int high) {
  int pivot = (*vec)[high];
  int tmp;
  int i = (low - 1);
  for (int j = low; j <= high - 1; j++) {
    if ((*vec)[j] < pivot) {
      i++;
      tmp = (*vec)[i];
      (*vec)[i] = (*vec)[j];
      (*vec)[j] = tmp;
    }
  }
  tmp = (*vec)[i + 1];
  (*vec)[i + 1] = (*vec)[high];
  (*vec)[high] = tmp;
  return (i + 1);
}

void quickSort(std::vector<int>* vec, int low, int high) {
  if (low < high) {
    int pivot = partition(vec, low, high);
    quickSort(vec, low, pivot - 1);
    quickSort(vec, pivot + 1, high);
  }
}

void addComp(std::vector<int> upvec, std::vector<int> downvec) {
  int ressize = static_cast<int>(upvec.size())
    + static_cast<int>(downvec.size());
  if (ressize == 1) return;
  if (ressize == 2) {
    std::pair<int, int> tmp{ upvec[0], downvec[0] };
    comparators.push_back(tmp);
    return;
  }
  std::vector<int> upvec_odd, downvec_odd, upvec_even, downvec_even, vecres(ressize);
  for (int i = 0; i < static_cast<int>(upvec.size()); i++) {
    if (i % 2)
      upvec_even.push_back(upvec[i]);
    else
      upvec_odd.push_back(upvec[i]);
  }
  for (int i = 0; i < static_cast<int>(downvec.size()); i++) {
    if (i % 2)
      downvec_even.push_back(downvec[i]);
    else
      downvec_odd.push_back(downvec[i]);
  }
  addComp(upvec_odd, downvec_odd);
  addComp(upvec_even, downvec_even);

  std::copy(upvec.begin(), upvec.end(), vecres.begin());
  std::copy(downvec.begin(), downvec.end(), vecres.begin() + upvec.size());

  for (int i = 1; i < static_cast<int>(vecres.size()) - 1; i += 2) {
    std::pair<int, int> tmp{ vecres[i], vecres[i + 1] };
    comparators.push_back(tmp);
  }
}

void buildNet(std::vector<int> allranks) {
  if (allranks.size() < 2) return;
  std::vector<int> upvec(allranks.size() / 2);
  std::vector<int> downvec(allranks.size() / 2 + allranks.size() % 2);
  std::copy(allranks.begin(), allranks.begin() + upvec.size(), upvec.begin());
  std::copy(allranks.begin() + upvec.size(), allranks.end(), downvec.begin());
  buildNet(upvec);
  buildNet(downvec);
  addComp(upvec, downvec);
}

void quickBatcher(std::vector<int>* vec) {
  MPI_Status status;
  int rank, size;
  int n = static_cast<int>(vec->size());
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (n <= 0)
    throw "Negative size";

  int fictive = 0;
  while (n % size) {
    vec->push_back(1000);
    n++;
    fictive++;
  }
  int len = n / size;

  std::vector<int> allranks(size);
  for (int i = 0; i < static_cast<int>(allranks.size()); i++) {
    allranks[i] = i;
  }
  buildNet(allranks);

  std::vector<int> resvec(len), curvec(len), tmpvec(len);

  MPI_Scatter(&(*vec)[0], len, MPI_INT, &resvec[0], len, MPI_INT, 0, MPI_COMM_WORLD);
  quickSort(&resvec, 0, len - 1);
  for (int i = 0; i < static_cast<int>(comparators.size()); i++) {
    int a = comparators[i].first, b = comparators[i].second;
    if (rank == a) {
      MPI_Send(&resvec[0], len, MPI_INT, b, 0, MPI_COMM_WORLD);
      MPI_Recv(&curvec[0], len, MPI_INT, b, 0, MPI_COMM_WORLD, &status);
      for (int resi = 0, curi = 0, tmpi = 0; tmpi < len; tmpi++) {
        int res = resvec[resi];
        int cur = curvec[curi];
        if (res < cur) {
          tmpvec[tmpi] = res;
          resi++;
        } else {
          tmpvec[tmpi] = cur;
          curi++;
        }
      }
      resvec.swap(tmpvec);
    } else if (rank == b) {
      MPI_Recv(&curvec[0], len, MPI_INT, a, 0, MPI_COMM_WORLD, &status);
      MPI_Send(&resvec[0], len, MPI_INT, a, 0, MPI_COMM_WORLD);
      int start = len - 1;
      for (int resi = start, curi = start, tmpi = start; tmpi >= 0; tmpi--) {
        int res = resvec[resi];
        int cur = curvec[curi];
        if (res > cur) {
          tmpvec[tmpi] = res;
          resi--;
        } else {
          tmpvec[tmpi] = cur;
          curi--;
        }
      }
      resvec.swap(tmpvec);
    }
  }
  MPI_Gather(&resvec[0], len, MPI_INT, &(*vec)[0], len, MPI_INT, 0, MPI_COMM_WORLD);
  if (rank == 0 && fictive) {
    n -= fictive;
    vec->resize(n);
  }
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&(*vec)[0], n, MPI_INT, 0, MPI_COMM_WORLD);
}

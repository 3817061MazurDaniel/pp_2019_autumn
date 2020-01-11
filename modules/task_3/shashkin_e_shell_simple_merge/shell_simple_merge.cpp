// Copyright 2019 Shashkin Evgeny
#include "../../../modules/task_3/shashkin_e_shell_simple_merge/shell_simple_merge.h"
#include <vector>
#include <random>
#include <ctime>

std::vector<int> GetRandomVector(int size) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec(size);
  std::mt19937 gen;
  gen.seed(time(0));
  if (rank == 0) {
    for (int i = 0; i < size; ++i) {
      vec[i] = gen() % size;
    }
  }

  MPI_Bcast(&vec[0], size, MPI_INT, 0, MPI_COMM_WORLD);

  return vec;
}

std::vector<int> ShellSort(const std::vector<int> &vec, int vec_size) {
  int step, i, j, tmp;
  std::vector<int> result(vec);

  for (step = vec_size / 2; step > 0; step /= 2)
    for (i = step; i < vec_size; i++)
      for (j = i - step; j >= 0 && result[j] > result[j + step]; j -= step) {
        tmp = result[j];
        result[j] = result[j + step];
        result[j + step] = tmp;
      }
  return result;
}

std::vector<int> SimpleMerge(const std::vector<int> &vec1, const std::vector<int> &vec2, int vec1_size, int vec2_size) {
  int i = 0, j = 0;
  int vec_size = vec1_size + vec2_size;
  std::vector<int> result(vec_size);
  for (int k = 0; k < vec_size; k++) {
    if (i > vec1_size - 1) {
      int tmp = vec2[j];
      result[k] = tmp;
      j++;
    } else if (j > vec2_size - 1) {
      int tmp = vec1[i];
      result[k] = tmp;
      i++;
    } else if (vec1[i] < vec2[j]) {
      int tmp = vec1[i];
      result[k] = tmp;
      i++;
    } else {
      int tmp = vec2[j];
      result[k] = tmp;
      j++;
    }
  }

  return result;
}

std::vector<int> ShellSimpleMerge(std::vector<int> vec, int vec_size) {
  int size, rank;
  int merge_size = 1;
  MPI_Status status;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (vec_size < 2)
    return vec;

  if (size > vec_size) {
    std::vector<int> tmp;
    tmp = ShellSort(vec, vec.size());
    return tmp;
  }

  std::vector<int> result;

  if (rank < vec_size) {
    for (int i = rank; i < vec_size; i += size)
      result.push_back(vec[i]);

    result = ShellSort(result, result.size());
  } else {
    return vec;
  }

  do {
    merge_size *= 2;

    if (rank % merge_size == merge_size / 2 && result.size() != 0) {
      MPI_Send(&result[0], result.size(), MPI_INT, rank - merge_size / 2, 0, MPI_COMM_WORLD);
      return result;
    }
    if (rank % merge_size == 0 && (rank + merge_size / 2) < size && result.size() != 0) {
      int local_vec_size;
      MPI_Probe(rank + merge_size / 2, 0, MPI_COMM_WORLD, &status);
      MPI_Get_count(&status, MPI_INT, &local_vec_size);

      std::vector<int> local_vec(local_vec_size);

      MPI_Recv(&local_vec[0], local_vec_size, MPI_INT, rank + merge_size / 2, 0, MPI_COMM_WORLD, &status);

      result = SimpleMerge(result, local_vec, result.size(), local_vec.size());
    }
  } while (merge_size < size);

  return result;
}

// Copyright 2019 Poletueva Anastasia
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_1/poletueva_a_counting_the_number_of_words/poletueva_a_counting_the_number_of_words_mpi.h"

int getCountWords(std:: string str) {
  if (str.empty()) {
    throw Error_empty;
  }
  int count = 1;
  for (unsigned int i = 0; i < str.size(); i++)
    if ((str[i] == ' ')&&(str[i+1] != ' ')&&(i+1 < str.size())) count++;
  return count;
}

const std::string randomWord() {
  std::vector<std::string> s = { "cat", "dog", "word", "summer", "autumn",
    "letter", "problem", "time", "random", "fox", "ball", "somebody", "car",
    "dress", "sweets", "juice", "tea", "lion", "cucumber", "play", "do", "go",
    "can", "smile", "look", "work", "increase", "consider", "take", "think",
    "find", "give", "show", "develop", "talk", "read" };
  int strLength = s.size();

  return s[std::rand() % strLength];
}

std::string getRandomStr() {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));

  std::string str = "";

  int k = 0;

  k = gen() % 100;

  for (int i = 0; i < k; i++) {
    str.append(randomWord());
    str.append(" ");
  }
  str.append("end");
  return str;
}

int getCountWordsParallel(std::string str) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<char> vec;
  int delta, rem;

  if (rank == 0) {
    vec = std::vector<char>(str.begin(), str.end());
    delta = vec.size() / size;
    rem = vec.size() % size;
}

  MPI_Bcast(&delta, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&rem, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (delta < 1) {
    int r;
    if (rank == 0)
      r = getCountWords(str);
    MPI_Bcast(&r, delta, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    return r;
  }

  if (rank == 0) {
    for (int proc = 1; proc < size - 1; proc++) {
      MPI_Send(&delta, 1, MPI_INT, proc, 1, MPI_COMM_WORLD);
      MPI_Send(&vec[0] + proc * delta+rem, delta, MPI_CHAR, proc, 0, MPI_COMM_WORLD);
    }

    if (size > 1) {
      MPI_Send(&delta, 1, MPI_INT, size - 1, 1, MPI_COMM_WORLD);
      MPI_Send(&vec[0] + (size - 1) * delta+rem, delta, MPI_CHAR, size - 1, 0, MPI_COMM_WORLD);
    }
  }

  std::vector<char> loc_vec;
  if (rank == 0) {
    loc_vec = std::vector<char>(vec.begin(),
      vec.begin() + delta+rem);
  } else {
    MPI_Status status;
    MPI_Recv(&delta, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    loc_vec.resize(delta);
    MPI_Recv(&loc_vec[0], delta, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
  }

  int global_sum = 0;

  int local_sum = 0;
  for (unsigned int i = 0; i < loc_vec.size(); i++)
    if (loc_vec[i] == ' ') local_sum++;

  MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  return global_sum + 1;
}


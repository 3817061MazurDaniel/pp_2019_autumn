#ifndef MODULES_TASK_3_KORNEV_N_BIN_IMG_LABELING_BIN_IMG_LABELING_H_
#define MODULES_TASK_3_KORNEV_N_BIN_IMG_LABELING_BIN_IMG_LABELING_H_
// Copyright 2019 Kornev Nikita

#include <iostream>
#include "./mpi.h"

struct image {
  int m, n, count;
  int** data;

  image(int _m, int _n) : m(_m), n(_n), count(1) {
    data = new int*[m];
    for (int i = 0; i < m; i++) {
      data[i] = new int[n];
      for (int j = 0; j < n; j++) {
        data[i][j] = 0;
      }
    }
  }

  friend std::ostream& operator<< (std::ostream& os, const image& img) {
    for (int i = 0; i < img.m; i++) {
      for (int j = 0; j < img.n; j++) {
        os << img.data[i][j] << "\t";
      }
      os << std::endl;
    }
    return os;
  }
};

void labeling(image* img);

#endif  // MODULES_TASK_3_KORNEV_N_BIN_IMG_LABELING_BIN_IMG_LABELING_H_

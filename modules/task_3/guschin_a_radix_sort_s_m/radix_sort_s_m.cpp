// Copyright 2019 Guschin Alexander
#include <mpi.h>
#include <string>
#include <vector>
#include "../../../modules/task_3/guschin_a_radix_sort_s_m/radix_sort_s_m.h"

int D_heap_cntr(int root, int size) {
  if (root >= size) return 0;
  int base = 1;
  if (root * 2 + 1 < size) base += D_heap_cntr(root * 2 + 1, size);
  if (root * 2 + 2 < size) base += D_heap_cntr(root * 2 + 2, size);
  return base;
}

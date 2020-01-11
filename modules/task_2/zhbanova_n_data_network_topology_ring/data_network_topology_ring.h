
#ifndef MODULES_TASK_2_ZHBANOVA_N_DATA_NETWORK_TOPOLOGY_RING_DATA_NETWORK_TOPOLOGY_RING_H_
#define MODULES_TASK_2_ZHBANOVA_N_DATA_NETWORK_TOPOLOGY_RING_DATA_NETWORK_TOPOLOGY_RING_H_

// Copyright 2019 Zhbanova Nadezhda

#include <vector>

const int VEC_SIZE = 3;

std::vector<int> randomInputVector(int size);
std::vector<int> transferDataToCheck(int rank, int rankFrom, int rankTo, std::vector<int> vec);

// all processes initially know the recipient process
std::vector<int> sendVector(int rankFrom, int rankTo, std::vector<int> vec);
void sendByRing(std::vector<int> vec, int rank, int size);
std::vector<int> recvFromRing(int rank, int size);

#endif  // MODULES_TASK_2_ZHBANOVA_N_DATA_NETWORK_TOPOLOGY_RING_DATA_NETWORK_TOPOLOGY_RING_H_

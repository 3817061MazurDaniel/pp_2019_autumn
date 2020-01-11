// Copyright 2019 Obolenskiy Arseniy
#ifndef MODULES_TASK_3_OBOLENSKIY_A_GAUSSIAN_IMAGE_FILTERING_GAUSSIAN_IMAGE_FILTERING_H_
#define MODULES_TASK_3_OBOLENSKIY_A_GAUSSIAN_IMAGE_FILTERING_GAUSSIAN_IMAGE_FILTERING_H_
#include <vector>

struct Pixel {
    uint8_t r, g, b, a;
};

std::vector <Pixel> getRandomMatrix(int rows, int cols);
std::vector <Pixel> solveParallel(std::vector <Pixel> a, int rows, int cols);
std::vector <Pixel> solveSequential(std::vector <Pixel> a, int rows, int cols);

#endif  // MODULES_TASK_3_OBOLENSKIY_A_GAUSSIAN_IMAGE_FILTERING_GAUSSIAN_IMAGE_FILTERING_H_

// Copyright 2019 Makarikhin Semen
#ifndef MODULES_TASK_3_MAKARIKHIN_VERTICAL_GAUSS_VERTICAL_GAUSS_H_
#define MODULES_TASK_3_MAKARIKHIN_VERTICAL_GAUSS_VERTICAL_GAUSS_H_
#include <vector>

constexpr unsigned int gaussFilter[3][3]{ {1, 2, 1},
                                           {2, 4, 2},
                                           {1, 2, 1} };

std::vector<unsigned int> Rand_Img(int cols, int rows);

unsigned int Pixel(std::vector<unsigned int> _source, int x, int y, int rows, int cols);

std::vector<unsigned int> fil_img_parall(std::vector<unsigned int> source, int rows, int cols);
std::vector<unsigned int> fil_img_sequential(std::vector<unsigned int> source, int rows, int cols);

#endif  // MODULES_TASK_3_MAKARIKHIN_VERTICAL_GAUSS_VERTICAL_GAUSS_H_

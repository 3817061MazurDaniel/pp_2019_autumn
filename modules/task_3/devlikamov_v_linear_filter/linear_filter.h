// Copyright 2019 Devlikamov V
#ifndef MODULES_TASK_3_DEVLIKAMOV_V_LINEAR_FILTER_LINEAR_FILTER_H_
#define MODULES_TASK_3_DEVLIKAMOV_V_LINEAR_FILTER_LINEAR_FILTER_H_

struct rgb {
    unsigned char red, green, blue;
};

const int Mask[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};

const int sumOfElementsInMask = 16;

rgb* createMatrix(int n, int m);
rgb* createMatrix1(int n, int m);

class GaussianBlurs {
 public:
    rgb *source;
    rgb *result;
    rgb Processing(int i, int j);
    int _cols;
    int _rows;
    int a;
    int b;
    GaussianBlurs(rgb* input, int rows, int cols);
    rgb* GetResult();
    void process();
};

rgb* linear_filter_with_gauss(int n, int m, int code = 0);


#endif  // MODULES_TASK_3_DEVLIKAMOV_V_LINEAR_FILTER_LINEAR_FILTER_H_

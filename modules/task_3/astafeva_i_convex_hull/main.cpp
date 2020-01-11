// Copyright 2019 Astafeva Irina

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "../../../modules/task_3/astafeva_i_convex_hull/convex_hull.h"

TEST(Convex_Hull, build_Image_Convex_Hull_Right) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int height = 10;
    int width = 10;
    int ** image = new int*[height];
    for (int i = 0; i < height; i++) {
        image[i] = new int[width];
    }

    image[0][0] = 0; image[0][1] = 0; image[0][2] = 0; image[0][3] = 0; image[0][4] = 0;
    image[0][5] = 0; image[0][6] = 0; image[0][7] = 0; image[0][8] = 0; image[0][9] = 0;

    image[1][0] = 0; image[1][1] = 1; image[1][2] = 1; image[1][3] = 1; image[1][4] = 0;
    image[1][5] = 0; image[1][6] = 0; image[1][7] = 0; image[1][8] = 0; image[1][9] = 0;

    image[2][0] = 0; image[2][1] = 0; image[2][2] = 0; image[2][3] = 1; image[2][4] = 1;
    image[2][5] = 1; image[2][6] = 0; image[2][7] = 0; image[2][8] = 0; image[2][9] = 0;

    image[3][0] = 0; image[3][1] = 0; image[3][2] = 0; image[3][3] = 0; image[3][4] = 1;
    image[3][5] = 1; image[3][6] = 0; image[3][7] = 0; image[3][8] = 0; image[3][9] = 0;

    image[4][0] = 0; image[4][1] = 0; image[4][2] = 0; image[4][3] = 0; image[4][4] = 1;
    image[4][5] = 0; image[4][6] = 0; image[4][7] = 0; image[4][8] = 1; image[4][9] = 0;

    image[5][0] = 0; image[5][1] = 0; image[5][2] = 0; image[5][3] = 0; image[5][4] = 0;
    image[5][5] = 0; image[5][6] = 0; image[5][7] = 0; image[5][8] = 1; image[5][9] = 0;

    image[6][0] = 0; image[6][1] = 1; image[6][2] = 0; image[6][3] = 1; image[6][4] = 0;
    image[6][5] = 0; image[6][6] = 0; image[6][7] = 0; image[6][8] = 1; image[6][9] = 0;

    image[7][0] = 0; image[7][1] = 1; image[7][2] = 1; image[7][3] = 1; image[7][4] = 0;
    image[7][5] = 0; image[7][6] = 1; image[7][7] = 1; image[7][8] = 1; image[7][9] = 0;

    image[8][0] = 0; image[8][1] = 0; image[8][2] = 1; image[8][3] = 1; image[8][4] = 0;
    image[8][5] = 0; image[8][6] = 0; image[8][7] = 1; image[8][8] = 0; image[8][9] = 0;

    image[9][0] = 0; image[9][1] = 0; image[9][2] = 0; image[9][3] = 0; image[9][4] = 0;
    image[9][5] = 0; image[9][6] = 0; image[9][7] = 0; image[9][8] = 0; image[9][9] = 0;

    int ** check = new int*[height];
    for (int i = 0; i < height; i++) {
        check[i] = new int[width];
    }

    check[0][0] = 0; check[0][1] = 0; check[0][2] = 0; check[0][3] = 0; check[0][4] = 0;
    check[0][5] = 0; check[0][6] = 0; check[0][7] = 0; check[0][8] = 0; check[0][9] = 0;

    check[1][0] = 0; check[1][1] = 1; check[1][2] = 1; check[1][3] = 1; check[1][4] = 1;
    check[1][5] = 0; check[1][6] = 0; check[1][7] = 0; check[1][8] = 0; check[1][9] = 0;

    check[2][0] = 0; check[2][1] = 0; check[2][2] = 1; check[2][3] = 1; check[2][4] = 1;
    check[2][5] = 1; check[2][6] = 0; check[2][7] = 0; check[2][8] = 0; check[2][9] = 0;

    check[3][0] = 0; check[3][1] = 0; check[3][2] = 0; check[3][3] = 1; check[3][4] = 1;
    check[3][5] = 1; check[3][6] = 0; check[3][7] = 0; check[3][8] = 0; check[3][9] = 0;

    check[4][0] = 0; check[4][1] = 0; check[4][2] = 0; check[4][3] = 0; check[4][4] = 1;
    check[4][5] = 0; check[4][6] = 0; check[4][7] = 0; check[4][8] = 1; check[4][9] = 0;

    check[5][0] = 0; check[5][1] = 0; check[5][2] = 0; check[5][3] = 0; check[5][4] = 0;
    check[5][5] = 0; check[5][6] = 0; check[5][7] = 1; check[5][8] = 1; check[5][9] = 0;

    check[6][0] = 0; check[6][1] = 1; check[6][2] = 1; check[6][3] = 1; check[6][4] = 0;
    check[6][5] = 0; check[6][6] = 1; check[6][7] = 1; check[6][8] = 1; check[6][9] = 0;

    check[7][0] = 0; check[7][1] = 1; check[7][2] = 1; check[7][3] = 1; check[7][4] = 0;
    check[7][5] = 0; check[7][6] = 1; check[7][7] = 1; check[7][8] = 1; check[7][9] = 0;

    check[8][0] = 0; check[8][1] = 0; check[8][2] = 1; check[8][3] = 1; check[8][4] = 0;
    check[8][5] = 0; check[8][6] = 0; check[8][7] = 1; check[8][8] = 0; check[8][9] = 0;

    check[9][0] = 0; check[9][1] = 0; check[9][2] = 0; check[9][3] = 0; check[9][4] = 0;
    check[9][5] = 0; check[9][6] = 0; check[9][7] = 0; check[9][8] = 0; check[9][9] = 0;

    int ** result = buildImageConvexHull(image, height, width);


    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            ASSERT_EQ(result[i][j], check[i][j]);
        }
    }
}

TEST(Convex_Hull, build_Image_Convex_Hull_Right_2) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int height = 10;
    int width = 10;
    int ** image = new int*[height];
    for (int i = 0; i < height; i++) {
        image[i] = new int[width];
    }

    image[0][0] = 0; image[0][1] = 1; image[0][2] = 1; image[0][3] = 1; image[0][4] = 0;
    image[0][5] = 0; image[0][6] = 0; image[0][7] = 0; image[0][8] = 0; image[0][9] = 0;

    image[1][0] = 0; image[1][1] = 0; image[1][2] = 1; image[1][3] = 0; image[1][4] = 0;
    image[1][5] = 0; image[1][6] = 0; image[1][7] = 0; image[1][8] = 1; image[1][9] = 1;

    image[2][0] = 0; image[2][1] = 1; image[2][2] = 1; image[2][3] = 1; image[2][4] = 1;
    image[2][5] = 0; image[2][6] = 0; image[2][7] = 0; image[2][8] = 0; image[2][9] = 1;

    image[3][0] = 0; image[3][1] = 1; image[3][2] = 0; image[3][3] = 0; image[3][4] = 0;
    image[3][5] = 0; image[3][6] = 0; image[3][7] = 1; image[3][8] = 1; image[3][9] = 1;

    image[4][0] = 0; image[4][1] = 1; image[4][2] = 1; image[4][3] = 1; image[4][4] = 0;
    image[4][5] = 0; image[4][6] = 0; image[4][7] = 0; image[4][8] = 1; image[4][9] = 1;

    image[5][0] = 0; image[5][1] = 0; image[5][2] = 0; image[5][3] = 0; image[5][4] = 0;
    image[5][5] = 0; image[5][6] = 1; image[5][7] = 1; image[5][8] = 1; image[5][9] = 1;

    image[6][0] = 0; image[6][1] = 0; image[6][2] = 0; image[6][3] = 1; image[6][4] = 1;
    image[6][5] = 0; image[6][6] = 0; image[6][7] = 0; image[6][8] = 0; image[6][9] = 0;

    image[7][0] = 0; image[7][1] = 1; image[7][2] = 1; image[7][3] = 1; image[7][4] = 1;
    image[7][5] = 1; image[7][6] = 1; image[7][7] = 0; image[7][8] = 0; image[7][9] = 0;

    image[8][0] = 0; image[8][1] = 0; image[8][2] = 0; image[8][3] = 1; image[8][4] = 1;
    image[8][5] = 0; image[8][6] = 0; image[8][7] = 0; image[8][8] = 0; image[8][9] = 0;

    image[9][0] = 0; image[9][1] = 0; image[9][2] = 1; image[9][3] = 1; image[9][4] = 0;
    image[9][5] = 0; image[9][6] = 0; image[9][7] = 0; image[9][8] = 0; image[9][9] = 0;

    int ** check = new int*[height];
    for (int i = 0; i < height; i++) {
        check[i] = new int[width];
    }

    check[0][0] = 0; check[0][1] = 1; check[0][2] = 1; check[0][3] = 1; check[0][4] = 0;
    check[0][5] = 0; check[0][6] = 0; check[0][7] = 0; check[0][8] = 0; check[0][9] = 0;

    check[1][0] = 0; check[1][1] = 1; check[1][2] = 1; check[1][3] = 1; check[1][4] = 1;
    check[1][5] = 0; check[1][6] = 0; check[1][7] = 0; check[1][8] = 1; check[1][9] = 1;

    check[2][0] = 0; check[2][1] = 1; check[2][2] = 1; check[2][3] = 1; check[2][4] = 1;
    check[2][5] = 0; check[2][6] = 0; check[2][7] = 1; check[2][8] = 1; check[2][9] = 1;

    check[3][0] = 0; check[3][1] = 1; check[3][2] = 1; check[3][3] = 1; check[3][4] = 1;
    check[3][5] = 0; check[3][6] = 1; check[3][7] = 1; check[3][8] = 1; check[3][9] = 1;

    check[4][0] = 0; check[4][1] = 1; check[4][2] = 1; check[4][3] = 1; check[4][4] = 0;
    check[4][5] = 0; check[4][6] = 1; check[4][7] = 1; check[4][8] = 1; check[4][9] = 1;

    check[5][0] = 0; check[5][1] = 0; check[5][2] = 0; check[5][3] = 0; check[5][4] = 0;
    check[5][5] = 0; check[5][6] = 1; check[5][7] = 1; check[5][8] = 1; check[5][9] = 1;

    check[6][0] = 0; check[6][1] = 0; check[6][2] = 1; check[6][3] = 1; check[6][4] = 1;
    check[6][5] = 1; check[6][6] = 0; check[6][7] = 0; check[6][8] = 0; check[6][9] = 0;

    check[7][0] = 0; check[7][1] = 1; check[7][2] = 1; check[7][3] = 1; check[7][4] = 1;
    check[7][5] = 1; check[7][6] = 1; check[7][7] = 0; check[7][8] = 0; check[7][9] = 0;

    check[8][0] = 0; check[8][1] = 1; check[8][2] = 1; check[8][3] = 1; check[8][4] = 1;
    check[8][5] = 1; check[8][6] = 0; check[8][7] = 0; check[8][8] = 0; check[8][9] = 0;

    check[9][0] = 0; check[9][1] = 0; check[9][2] = 1; check[9][3] = 1; check[9][4] = 1;
    check[9][5] = 0; check[9][6] = 0; check[9][7] = 0; check[9][8] = 0; check[9][9] = 0;

    int ** result = buildImageConvexHull(image, height, width);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            ASSERT_EQ(result[i][j], check[i][j]);
        }
    }
}

TEST(Convex_Hull, build_Image_Convex_Hull_Right_3) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int height = 10;
    int width = 10;
    int ** image = new int*[height];
    for (int i = 0; i < height; i++) {
        image[i] = new int[width];
    }

    image[0][0] = 0; image[0][1] = 0; image[0][2] = 0; image[0][3] = 0; image[0][4] = 0;
    image[0][5] = 0; image[0][6] = 0; image[0][7] = 0; image[0][8] = 0; image[0][9] = 0;

    image[1][0] = 0; image[1][1] = 1; image[1][2] = 1; image[1][3] = 0; image[1][4] = 1;
    image[1][5] = 1; image[1][6] = 1; image[1][7] = 0; image[1][8] = 0; image[1][9] = 0;

    image[2][0] = 0; image[2][1] = 1; image[2][2] = 0; image[2][3] = 0; image[2][4] = 0;
    image[2][5] = 0; image[2][6] = 1; image[2][7] = 0; image[2][8] = 0; image[2][9] = 0;

    image[3][0] = 0; image[3][1] = 1; image[3][2] = 1; image[3][3] = 1; image[3][4] = 0;
    image[3][5] = 1; image[3][6] = 1; image[3][7] = 0; image[3][8] = 0; image[3][9] = 0;

    image[4][0] = 0; image[4][1] = 0; image[4][2] = 0; image[4][3] = 0; image[4][4] = 0;
    image[4][5] = 0; image[4][6] = 0; image[4][7] = 0; image[4][8] = 0; image[4][9] = 0;

    image[5][0] = 0; image[5][1] = 0; image[5][2] = 1; image[5][3] = 1; image[5][4] = 1;
    image[5][5] = 1; image[5][6] = 0; image[5][7] = 0; image[5][8] = 0; image[5][9] = 0;

    image[6][0] = 0; image[6][1] = 1; image[6][2] = 1; image[6][3] = 1; image[6][4] = 1;
    image[6][5] = 0; image[6][6] = 0; image[6][7] = 0; image[6][8] = 0; image[6][9] = 0;

    image[7][0] = 0; image[7][1] = 0; image[7][2] = 1; image[7][3] = 1; image[7][4] = 1;
    image[7][5] = 1; image[7][6] = 0; image[7][7] = 0; image[7][8] = 0; image[7][9] = 1;

    image[8][0] = 0; image[8][1] = 1; image[8][2] = 1; image[8][3] = 1; image[8][4] = 1;
    image[8][5] = 0; image[8][6] = 0; image[8][7] = 0; image[8][8] = 0; image[8][9] = 1;

    image[9][0] = 0; image[9][1] = 0; image[9][2] = 1; image[9][3] = 0; image[9][4] = 0;
    image[9][5] = 0; image[9][6] = 1; image[9][7] = 1; image[9][8] = 1; image[9][9] = 1;

    int ** check = new int*[height];
    for (int i = 0; i < height; i++) {
        check[i] = new int[width];
    }

    check[0][0] = 0; check[0][1] = 0; check[0][2] = 0; check[0][3] = 0; check[0][4] = 0;
    check[0][5] = 0; check[0][6] = 0; check[0][7] = 0; check[0][8] = 0; check[0][9] = 0;

    check[1][0] = 0; check[1][1] = 1; check[1][2] = 1; check[1][3] = 0; check[1][4] = 1;
    check[1][5] = 1; check[1][6] = 1; check[1][7] = 0; check[1][8] = 0; check[1][9] = 0;

    check[2][0] = 0; check[2][1] = 1; check[2][2] = 1; check[2][3] = 1; check[2][4] = 1;
    check[2][5] = 1; check[2][6] = 1; check[2][7] = 0; check[2][8] = 0; check[2][9] = 0;

    check[3][0] = 0; check[3][1] = 1; check[3][2] = 1; check[3][3] = 1; check[3][4] = 0;
    check[3][5] = 1; check[3][6] = 1; check[3][7] = 0; check[3][8] = 0; check[3][9] = 0;

    check[4][0] = 0; check[4][1] = 0; check[4][2] = 0; check[4][3] = 0; check[4][4] = 0;
    check[4][5] = 0; check[4][6] = 0; check[4][7] = 0; check[4][8] = 0; check[4][9] = 0;

    check[5][0] = 0; check[5][1] = 0; check[5][2] = 1; check[5][3] = 1; check[5][4] = 1;
    check[5][5] = 1; check[5][6] = 0; check[5][7] = 0; check[5][8] = 0; check[5][9] = 0;

    check[6][0] = 0; check[6][1] = 1; check[6][2] = 1; check[6][3] = 1; check[6][4] = 1;
    check[6][5] = 1; check[6][6] = 0; check[6][7] = 0; check[6][8] = 0; check[6][9] = 0;

    check[7][0] = 0; check[7][1] = 1; check[7][2] = 1; check[7][3] = 1; check[7][4] = 1;
    check[7][5] = 1; check[7][6] = 0; check[7][7] = 0; check[7][8] = 1; check[7][9] = 1;

    check[8][0] = 0; check[8][1] = 1; check[8][2] = 1; check[8][3] = 1; check[8][4] = 1;
    check[8][5] = 0; check[8][6] = 0; check[8][7] = 1; check[8][8] = 1; check[8][9] = 1;

    check[9][0] = 0; check[9][1] = 0; check[9][2] = 1; check[9][3] = 1; check[9][4] = 0;
    check[9][5] = 0; check[9][6] = 1; check[9][7] = 1; check[9][8] = 1; check[9][9] = 1;

    int ** result = buildImageConvexHull(image, height, width);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            ASSERT_EQ(result[i][j], check[i][j]);
        }
    }
}

TEST(Convex_Hull, build_Image_Convex_Hull_Right_4) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int height = 10;
    int width = 10;
    int ** image = new int*[height];
    for (int i = 0; i < height; i++) {
        image[i] = new int[width];
    }

    image[0][0] = 0; image[0][1] = 0; image[0][2] = 0; image[0][3] = 0; image[0][4] = 0;
    image[0][5] = 0; image[0][6] = 1; image[0][7] = 1; image[0][8] = 0; image[0][9] = 0;

    image[1][0] = 0; image[1][1] = 1; image[1][2] = 1; image[1][3] = 0; image[1][4] = 0;
    image[1][5] = 0; image[1][6] = 1; image[1][7] = 1; image[1][8] = 1; image[1][9] = 0;

    image[2][0] = 0; image[2][1] = 1; image[2][2] = 0; image[2][3] = 0; image[2][4] = 0;
    image[2][5] = 0; image[2][6] = 1; image[2][7] = 0; image[2][8] = 1; image[2][9] = 0;

    image[3][0] = 0; image[3][1] = 1; image[3][2] = 0; image[3][3] = 0; image[3][4] = 0;
    image[3][5] = 0; image[3][6] = 0; image[3][7] = 0; image[3][8] = 1; image[3][9] = 0;

    image[4][0] = 0; image[4][1] = 1; image[4][2] = 0; image[4][3] = 0; image[4][4] = 1;
    image[4][5] = 1; image[4][6] = 1; image[4][7] = 1; image[4][8] = 1; image[4][9] = 0;

    image[5][0] = 0; image[5][1] = 1; image[5][2] = 0; image[5][3] = 0; image[5][4] = 0;
    image[5][5] = 0; image[5][6] = 0; image[5][7] = 0; image[5][8] = 0; image[5][9] = 0;

    image[6][0] = 0; image[6][1] = 1; image[6][2] = 1; image[6][3] = 1; image[6][4] = 0;
    image[6][5] = 0; image[6][6] = 0; image[6][7] = 0; image[6][8] = 0; image[6][9] = 0;

    image[7][0] = 0; image[7][1] = 0; image[7][2] = 0; image[7][3] = 1; image[7][4] = 0;
    image[7][5] = 0; image[7][6] = 0; image[7][7] = 0; image[7][8] = 0; image[7][9] = 0;

    image[8][0] = 0; image[8][1] = 1; image[8][2] = 1; image[8][3] = 1; image[8][4] = 0;
    image[8][5] = 0; image[8][6] = 0; image[8][7] = 0; image[8][8] = 0; image[8][9] = 0;

    image[9][0] = 0; image[9][1] = 0; image[9][2] = 0; image[9][3] = 0; image[9][4] = 0;
    image[9][5] = 0; image[9][6] = 0; image[9][7] = 0; image[9][8] = 0; image[9][9] = 0;

    int ** check = new int*[height];
    for (int i = 0; i < height; i++) {
        check[i] = new int[width];
    }

    check[0][0] = 0; check[0][1] = 0; check[0][2] = 0; check[0][3] = 0; check[0][4] = 0;
    check[0][5] = 0; check[0][6] = 1; check[0][7] = 1; check[0][8] = 0; check[0][9] = 0;

    check[1][0] = 0; check[1][1] = 1; check[1][2] = 1; check[1][3] = 0; check[1][4] = 0;
    check[1][5] = 1; check[1][6] = 1; check[1][7] = 1; check[1][8] = 1; check[1][9] = 0;

    check[2][0] = 0; check[2][1] = 1; check[2][2] = 1; check[2][3] = 1; check[2][4] = 1;
    check[2][5] = 1; check[2][6] = 1; check[2][7] = 1; check[2][8] = 1; check[2][9] = 0;

    check[3][0] = 0; check[3][1] = 1; check[3][2] = 1; check[3][3] = 1; check[3][4] = 1;
    check[3][5] = 1; check[3][6] = 1; check[3][7] = 1; check[3][8] = 1; check[3][9] = 0;

    check[4][0] = 0; check[4][1] = 1; check[4][2] = 1; check[4][3] = 1; check[4][4] = 1;
    check[4][5] = 1; check[4][6] = 1; check[4][7] = 1; check[4][8] = 1; check[4][9] = 0;

    check[5][0] = 0; check[5][1] = 1; check[5][2] = 1; check[5][3] = 1; check[5][4] = 0;
    check[5][5] = 0; check[5][6] = 0; check[5][7] = 0; check[5][8] = 0; check[5][9] = 0;

    check[6][0] = 0; check[6][1] = 1; check[6][2] = 1; check[6][3] = 1; check[6][4] = 0;
    check[6][5] = 0; check[6][6] = 0; check[6][7] = 0; check[6][8] = 0; check[6][9] = 0;

    check[7][0] = 0; check[7][1] = 1; check[7][2] = 1; check[7][3] = 1; check[7][4] = 0;
    check[7][5] = 0; check[7][6] = 0; check[7][7] = 0; check[7][8] = 0; check[7][9] = 0;

    check[8][0] = 0; check[8][1] = 1; check[8][2] = 1; check[8][3] = 1; check[8][4] = 0;
    check[8][5] = 0; check[8][6] = 0; check[8][7] = 0; check[8][8] = 0; check[8][9] = 0;

    check[9][0] = 0; check[9][1] = 0; check[9][2] = 0; check[9][3] = 0; check[9][4] = 0;
    check[9][5] = 0; check[9][6] = 0; check[9][7] = 0; check[9][8] = 0; check[9][9] = 0;

    int ** result = buildImageConvexHull(image, height, width);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            ASSERT_EQ(result[i][j], check[i][j]);
        }
    }
}

TEST(Convex_Hull, build_Image_Convex_Hull_Right_5) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int height = 10;
    int width = 10;
    int ** image = new int*[height];
    for (int i = 0; i < height; i++) {
        image[i] = new int[width];
    }

    image[0][0] = 1; image[0][1] = 0; image[0][2] = 0; image[0][3] = 0; image[0][4] = 0;
    image[0][5] = 1; image[0][6] = 0; image[0][7] = 0; image[0][8] = 0; image[0][9] = 0;

    image[1][0] = 1; image[1][1] = 0; image[1][2] = 1; image[1][3] = 1; image[1][4] = 1;
    image[1][5] = 1; image[1][6] = 0; image[1][7] = 1; image[1][8] = 1; image[1][9] = 1;

    image[2][0] = 0; image[2][1] = 0; image[2][2] = 0; image[2][3] = 0; image[2][4] = 1;
    image[2][5] = 0; image[2][6] = 0; image[2][7] = 0; image[2][8] = 0; image[2][9] = 1;

    image[3][0] = 1; image[3][1] = 0; image[3][2] = 0; image[3][3] = 1; image[3][4] = 1;
    image[3][5] = 1; image[3][6] = 0; image[3][7] = 0; image[3][8] = 1; image[3][9] = 1;

    image[4][0] = 1; image[4][1] = 1; image[4][2] = 0; image[4][3] = 0; image[4][4] = 1;
    image[4][5] = 1; image[4][6] = 1; image[4][7] = 0; image[4][8] = 0; image[4][9] = 1;

    image[5][0] = 1; image[5][1] = 0; image[5][2] = 0; image[5][3] = 0; image[5][4] = 0;
    image[5][5] = 1; image[5][6] = 0; image[5][7] = 0; image[5][8] = 0; image[5][9] = 0;

    image[6][0] = 1; image[6][1] = 0; image[6][2] = 1; image[6][3] = 1; image[6][4] = 1;
    image[6][5] = 1; image[6][6] = 0; image[6][7] = 1; image[6][8] = 1; image[6][9] = 1;

    image[7][0] = 0; image[7][1] = 0; image[7][2] = 0; image[7][3] = 0; image[7][4] = 1;
    image[7][5] = 0; image[7][6] = 0; image[7][7] = 0; image[7][8] = 0; image[7][9] = 1;

    image[8][0] = 1; image[8][1] = 0; image[8][2] = 0; image[8][3] = 1; image[8][4] = 1;
    image[8][5] = 1; image[8][6] = 0; image[8][7] = 0; image[8][8] = 1; image[8][9] = 1;

    image[9][0] = 1; image[9][1] = 1; image[9][2] = 0; image[9][3] = 0; image[9][4] = 1;
    image[9][5] = 1; image[9][6] = 1; image[9][7] = 0; image[9][8] = 0; image[9][9] = 1;

    int ** check = new int*[height];
    for (int i = 0; i < height; i++) {
        check[i] = new int[width];
    }

    check[0][0] = 1; check[0][1] = 0; check[0][2] = 0; check[0][3] = 1; check[0][4] = 1;
    check[0][5] = 1; check[0][6] = 0; check[0][7] = 0; check[0][8] = 0; check[0][9] = 0;

    check[1][0] = 1; check[1][1] = 0; check[1][2] = 1; check[1][3] = 1; check[1][4] = 1;
    check[1][5] = 1; check[1][6] = 1; check[1][7] = 1; check[1][8] = 1; check[1][9] = 1;

    check[2][0] = 0; check[2][1] = 0; check[2][2] = 1; check[2][3] = 1; check[2][4] = 1;
    check[2][5] = 1; check[2][6] = 1; check[2][7] = 1; check[2][8] = 1; check[2][9] = 1;

    check[3][0] = 1; check[3][1] = 0; check[3][2] = 1; check[3][3] = 1; check[3][4] = 1;
    check[3][5] = 1; check[3][6] = 1; check[3][7] = 0; check[3][8] = 1; check[3][9] = 1;

    check[4][0] = 1; check[4][1] = 1; check[4][2] = 1; check[4][3] = 1; check[4][4] = 1;
    check[4][5] = 1; check[4][6] = 1; check[4][7] = 0; check[4][8] = 0; check[4][9] = 1;

    check[5][0] = 1; check[5][1] = 1; check[5][2] = 1; check[5][3] = 1; check[5][4] = 1;
    check[5][5] = 1; check[5][6] = 1; check[5][7] = 0; check[5][8] = 0; check[5][9] = 0;

    check[6][0] = 1; check[6][1] = 0; check[6][2] = 1; check[6][3] = 1; check[6][4] = 1;
    check[6][5] = 1; check[6][6] = 1; check[6][7] = 1; check[6][8] = 1; check[6][9] = 1;

    check[7][0] = 0; check[7][1] = 0; check[7][2] = 1; check[7][3] = 1; check[7][4] = 1;
    check[7][5] = 1; check[7][6] = 1; check[7][7] = 1; check[7][8] = 1; check[7][9] = 1;

    check[8][0] = 1; check[8][1] = 0; check[8][2] = 0; check[8][3] = 1; check[8][4] = 1;
    check[8][5] = 1; check[8][6] = 1; check[8][7] = 0; check[8][8] = 1; check[8][9] = 1;

    check[9][0] = 1; check[9][1] = 1; check[9][2] = 0; check[9][3] = 0; check[9][4] = 1;
    check[9][5] = 1; check[9][6] = 1; check[9][7] = 0; check[9][8] = 0; check[9][9] = 1;

    int ** result = buildImageConvexHull(image, height, width);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            ASSERT_EQ(result[i][j], check[i][j]);
        }
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}

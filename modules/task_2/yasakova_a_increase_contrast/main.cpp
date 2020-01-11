// Copyright 2019 Yasakova Anastasia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "../../../modules/task_2/yasakova_a_increase_contrast/increase_contrast.h"

// TEST(Increase_contrast_MPI, test0) {
//   int rank;
//   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//   int contrast = 90;
//   cv::Mat image;
//   image = cv::imread("D:\\My OpenCV Website\\gray1.jpg");
//   std::vector <int> a(image.rows * image.cols);
//   if (rank == 0) {
//       cv::namedWindow("My image");
//       imshow("My image", image);
//       for (int i = 0; i < image.rows; i++) {
//           for (int j = 0; j < image.cols; j++) {
//               a[i * image.cols + j] = image.at<Vec3b>(i, j)[0];
//           }
//       }
//   }
//   std::vector <int> ans(image.rows * image.cols), ans_seq(image.rows * image.cols);
//   ans = ParallelIncreaseContrast(a, image.rows, image.cols, contrast);
//   cv::Mat res = image.clone();
//   cv::Mat res_seq = image.clone();
//   if (rank == 0) {
//       ans_seq = SequentialIncreaseContrast(a, image.rows, image.cols, contrast);
//       for (int i = 0; i < image.rows; i++) {
//           for (int j = 0; j < image.cols; j++) {
//               for (int k = 0; k < 3; k++) {
//                   res.at<cv::Vec3b>(i, j)[k] = ans[i * image.cols + j];
//                   res_seq.at<cv::Vec3b>(i, j)[k] = ans_seq[i * image.cols + j];
//               }
//           }
//      }
//      cv::namedWindow("Parallel Increase Contrast");
//      imshow("Parallel Increase Contrast", res);
//      cv::namedWindow("Sequential Increase Contrast");
//      imshow("Sequential Increase Contrast", res_seq);
//   }
//   image.release();
//   res.release();
//   res_seq.release();
//   if (rank == 0) {
//       cv::waitKey(0);
//       ASSERT_EQ(ans_seq, ans);
//   }
// }

TEST(Increase_contrast_MPI, test1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_ANY_THROW(getImage(-1, 3));
    }
}

TEST(Increase_contrast_MPI, test2) {
    std::vector <int> a;
    int contrast = 20;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        a = getImage(5, 7);
        ASSERT_ANY_THROW(ParallelIncreaseContrast(a, 3, 2, contrast));
    }
}

TEST(Increase_contrast_MPI, test3) {
    int rows = 3, cols = 3, contrast = 20;
    std::vector <int> a(rows * cols), ans_seq(rows * cols);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        a[0] = 150;
        a[1] = 128;
        a[2] = 100;
        a[3] = 200;
        a[4] = 175;
        a[5] = 100;
        a[6] = 50;
        a[7] = 250;
        a[8] = 200;
        ans_seq[0] = 150;
        ans_seq[1] = 123;
        ans_seq[2] = 90;
        ans_seq[3] = 210;
        ans_seq[4] = 180;
        ans_seq[5] = 90;
        ans_seq[6] = 29;
        ans_seq[7] = 255;
        ans_seq[8] = 210;
    }
    std::vector <int> ans(rows * cols);
    ans = ParallelIncreaseContrast(a, rows, cols, contrast);
    if (rank == 0) {
        ASSERT_EQ(ans_seq, ans);
    }
}

TEST(Increase_contrast_MPI, test4) {
    int rows = 3, cols = 3, contrast = 60;
    std::vector <int> a(rows * cols), ans_seq(rows * cols);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        a[0] = 150;
        a[1] = 128;
        a[2] = 100;
        a[3] = 200;
        a[4] = 175;
        a[5] = 100;
        a[6] = 50;
        a[7] = 250;
        a[8] = 200;
        ans_seq[0] = 150;
        ans_seq[1] = 114;
        ans_seq[2] = 70;
        ans_seq[3] = 230;
        ans_seq[4] = 190;
        ans_seq[5] = 70;
        ans_seq[6] = 0;
        ans_seq[7] = 255;
        ans_seq[8] = 230;
    }
    std::vector <int> ans(rows * cols);
    ans = ParallelIncreaseContrast(a, rows, cols, contrast);
    if (rank == 0) {
        ASSERT_EQ(ans_seq, ans);
    }
}

TEST(Increase_contrast_MPI, test5) {
    int rows = 200, cols = 150, contrast = 20;
    std::vector <int> a(rows * cols);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        a = getImage(rows, cols);
    }
    std::vector <int> ans(rows * cols);
    ans = ParallelIncreaseContrast(a, rows, cols, contrast);
    std::vector <int> ans_seq(rows * cols);
    if (rank == 0) {
        ans_seq = SequentialIncreaseContrast(a, rows, cols, contrast);
        ASSERT_EQ(ans_seq, ans);
    }
}

TEST(Increase_contrast_MPI, test6) {
    int rows = 1000, cols = 800, contrast = 80;
    std::vector <int> a(rows * cols);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        a = getImage(rows, cols);
    }
    std::vector <int> ans(rows * cols);
    ans = ParallelIncreaseContrast(a, rows, cols, contrast);
    std::vector <int> ans_seq(rows * cols);
    if (rank == 0) {
        ans_seq = SequentialIncreaseContrast(a, rows, cols, contrast);
        ASSERT_EQ(ans_seq, ans);
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

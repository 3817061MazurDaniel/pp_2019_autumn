// Copyright 2019 Iamshchikov Ivan
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./minimizer.h"
#include "./functions.h"

const double eps = 0.01;

TEST(minimizer, can_create_minimizer) {
    ASSERT_NO_THROW(Minimizer m(0.0, 0.0, 0.0, 0.0, nullptr));
    ASSERT_NO_THROW(One_Dimensional_Minimizer odm(0.0, 0.0, 0.0, nullptr));
}

TEST(minimizer, throw_when_left_border_more_than_right_border_in_odm) {
    ASSERT_ANY_THROW(One_Dimensional_Minimizer odm(10.0, 5.0, 0.0, nullptr));
}
TEST(minimizer, throw_when_left_border_more_than_right_border) {
    ASSERT_ANY_THROW(Minimizer m(10.0, 5.0, 0.0, 0.0, nullptr));
}

TEST(minimizer, throw_when_lower_border_more_than_upper_border) {
    ASSERT_ANY_THROW(Minimizer m(0.0, 5.0, 10.0, 0.0, nullptr));
}

TEST(minimizer, can_find_min_f1) {
    double(*fptr)(double, double) = f1;
    result res;
    double delta_x, delta_y, actual_x = 0.0, actual_y = 1.0;
    point a(-5.22, -5.78), b(5.73, -5.1), c(-5.68, 5.34), d(5.08, 5.49);
    Minimizer m(a.x, b.x, a.y, c.y, fptr);
    m.solve();
    if (m.isMin()) {
        res = m.get_result();
        delta_x = std::abs(actual_x - res.x);
        delta_y = std::abs(actual_y - res.y);

        EXPECT_EQ(1, delta_x <= eps);
        EXPECT_EQ(1, delta_y <= eps);
    }
}

TEST(minimizer, can_find_min_f1_on_x_axis) {
    double(*fptr)(double, double) = f1;
    result res;
    double delta_x, delta_y, actual_x = 0.0, actual_y = 1.0;
    point a(-5.6, 1.0), b(5.8, 1.0), c(-5.1, 1.0), d(5.5, 1.0);
    Minimizer m(a.x, b.x, a.y, c.y, fptr);
    m.solve();
    if (m.isMin()) {
        res = m.get_result();
        delta_x = std::abs(actual_x - res.x);
        delta_y = std::abs(actual_y - res.y);

        EXPECT_EQ(1, delta_x <= eps);
        EXPECT_EQ(1, delta_y <= eps);
    }
}

TEST(minimizer, can_find_min_f1_on_y_axis) {
    double(*fptr)(double, double) = f1;
    result res;
    double delta_x, delta_y, actual_x = 0.0, actual_y = 1.0;
    point a(0.0, -5.1), b(0.0, -5.5), c(0.0, 5.2), d(0.0, 5.9);
    Minimizer m(a.x, b.x, a.y, c.y, fptr);
    m.solve();
    if (m.isMin()) {
        res = m.get_result();
        delta_x = std::abs(actual_x - res.x);
        delta_y = std::abs(actual_y - res.y);

        EXPECT_EQ(1, delta_x <= eps);
        EXPECT_EQ(1, delta_y <= eps);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(minimizer, can_find_min_f2) {
    double(*fptr)(double, double) = f2;
    result res;
    double delta_x, delta_y, actual_x = 1.0, actual_y = 0.0;
    point a(-4.6, -2.48), b(3.25, -2.76), c(-4.41, 5.08), d(3.74, 5.68);
    Minimizer m(a.x, b.x, a.y, c.y, fptr);
    m.solve();
    if (m.isMin()) {
        res = m.get_result();
        delta_x = std::abs(actual_x - res.x);
        delta_y = std::abs(actual_y - res.y);

        EXPECT_EQ(1, delta_x <= eps);
        EXPECT_EQ(1, delta_y <= eps);
    }
}

TEST(minimizer, can_find_min_f2_on_x_axis) {
    double(*fptr)(double, double) = f2;
    result res;
    double delta_x, delta_y, actual_x = 1.0, actual_y = 0.0;
    point a(-4.46, 0.0), b(3.07, 0.0), c(-4.04, 0.0), d(3.34, 0.0);
    Minimizer m(a.x, b.x, a.y, c.y, fptr);
    m.solve();
    if (m.isMin()) {
        res = m.get_result();
        delta_x = std::abs(actual_x - res.x);
        delta_y = std::abs(actual_y - res.y);

        EXPECT_EQ(1, delta_x <= eps);
        EXPECT_EQ(1, delta_y <= eps);
    }
}

TEST(minimizer, can_find_min_f2_on_y_axis) {
    double(*fptr)(double, double) = f2;
    result res;
    double delta_x, delta_y, actual_x = 1.0, actual_y = 0.0;
    point a(1.0, -2.07), b(1.0, -2.45), c(1.0, 5.39), d(1.0, 5.14);
    Minimizer m(a.x, b.x, a.y, c.y, fptr);
    m.solve();
    if (m.isMin()) {
        res = m.get_result();
        delta_x = std::abs(actual_x - res.x);
        delta_y = std::abs(actual_y - res.y);

        EXPECT_EQ(1, delta_x <= eps);
        EXPECT_EQ(1, delta_y <= eps);
    }
}

TEST(minimizer, can_find_min_f3) {
    double(*fptr)(double, double) = f3;
    result res;
    double delta_x, delta_y, actual_x = 1.0, actual_y = 2.0;
    point a(0.5, 1.0), b(2.5, 1.0), c(0.5, 3.5), d(2.5, 3.5);
    Minimizer m(a.x, b.x, a.y, c.y, fptr);
    m.solve();
    if (m.isMin()) {
        res = m.get_result();
        delta_x = std::abs(actual_x - res.x);
        delta_y = std::abs(actual_y - res.y);

        EXPECT_EQ(1, delta_x <= eps);
        EXPECT_EQ(1, delta_y <= eps);
    }
}

TEST(minimizer, can_find_min_f3_on_x_axis) {
    double(*fptr)(double, double) = f3;
    result res;
    double delta_x, delta_y, actual_x = 1.0, actual_y = 2.0;
    point a(0.5, 2.0), b(2.5, 2.0), c(0.5, 2.0), d(2.5, 2.0);
    Minimizer m(a.x, b.x, a.y, c.y, fptr);
    m.solve();
    if (m.isMin()) {
        res = m.get_result();
        delta_x = std::abs(actual_x - res.x);
        delta_y = std::abs(actual_y - res.y);

        EXPECT_EQ(1, delta_x <= eps);
        EXPECT_EQ(1, delta_y <= eps);
    }
}

TEST(minimizer, can_find_min_f3_on_y_axis) {
    double(*fptr)(double, double) = f3;
    result res;
    double delta_x, delta_y, actual_x = 1.0, actual_y = 2.0;
    point a(1.0, 1.0), b(1.0, 1.0), c(1.0, 3.5), d(1.0, 3.5);
    Minimizer m(a.x, b.x, a.y, c.y, fptr);
    m.solve();
    if (m.isMin()) {
        res = m.get_result();
        delta_x = std::abs(actual_x - res.x);
        delta_y = std::abs(actual_y - res.y);

        EXPECT_EQ(1, delta_x <= eps);
        EXPECT_EQ(1, delta_y <= eps);
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

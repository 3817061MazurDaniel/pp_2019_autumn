// Copyright 2019 Kukushkina Ksenia

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>

#include <string>

#include "../../../modules/task_1/kukushkina_k_lexicograph/lexicograph.h"

TEST(Lexicograph, incorrect_size) {
  std::string s;
  ASSERT_ANY_THROW(s = generateString(-13));
}

TEST(Lexicograph, string_generation) {
  std::string s;
  ASSERT_NO_THROW(s = generateString(100));
}

TEST(Lexicograph, ordered_string_testing) {
  std::string s = "08ASbf";
  ASSERT_EQ(isFragmentOrdered(s), true);
}

TEST(Lexicograph, unordered_string_testing) {
  std::string s = "08ASbf1";
  ASSERT_EQ(isFragmentOrdered(s), false);
}

TEST(Lexicograph, one_symbol_string_testing) {
  std::string s = "*";
  ASSERT_EQ(isFragmentOrdered(s), true);
}

TEST(Lexicograph, null_string_testing) {
  std::string s;
  ASSERT_EQ(isFragmentOrdered(s), true);
}

TEST(Lexicograph, two_ordered_strings_testing) {
  std::string s1 = "08ASbfz", s2 = "Aabcd";
  ASSERT_EQ(isStringOrdered(s1, s2), true);
}

TEST(Lexicograph, two_unordered_strings_testing) {
  std::string s1 = "08ASbf1", s2 = "abcd1";
  ASSERT_EQ(isStringOrdered(s1, s2), false);
}

TEST(Lexicograph, ordered_and_unordered_strings_testing) {
  std::string s1 = "08ASbf", s2 = "abcd1";
  ASSERT_EQ(isStringOrdered(s1, s2), false);
}
TEST(Lexicograph, middle_unordered_strings_testing) {
  std::string s1 = "0000212222", s2 = "1113222433354446";
  ASSERT_EQ(isStringOrdered(s1, s2), false);
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

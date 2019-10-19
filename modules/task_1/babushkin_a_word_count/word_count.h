// Copyright 2019 Babushkin Alexey

#ifndef MODULES_TASK_1_BABUSHKIN_A_WORD_COUNT_WORD_COUNT_H_
#define MODULES_TASK_1_BABUSHKIN_A_WORD_COUNT_WORD_COUNT_H_

#define ERR_EMPTY_STRING -1

#include <string>

int wordCountParallel(const std::string ref);
int wordCount(const std::string ref);

/**
 * @brief Creates string with the specified count of random words.
 * @param wordCount Count of random words in a result string.
*/
std::string randomString(const int length);

#endif  //  MODULES_TASK_1_BABUSHKIN_A_WORD_COUNT_WORD_COUNT_H_

// Copyright 2019 Guschin Alexander
#ifndef MODULES_TASK_1_GUSCHIN_A_WORD_COUNT_WORD_COUNT_H_
#define MODULES_TASK_1_GUSCHIN_A_WORD_COUNT_WORD_COUNT_H_
#include <string>
#include <vector>

void randWord(std::string* st, int add_size);
bool isLetter(char sym);
int getCount(std::string st);
int getLinearCount(std::string st, int size);

#endif  // MODULES_TASK_1_GUSCHIN_A_WORD_COUNT_WORD_COUNT_H_

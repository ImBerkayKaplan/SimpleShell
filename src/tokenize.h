#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isNumber(const char *str);
void tokenize(char inBuffer[], char *args[]);

#endif


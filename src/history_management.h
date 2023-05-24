#ifndef HISTORY_MANAGEMENT_H
#define HISTORY_MANAGEMENT_H

#include <string.h>
#include "config.h"

void save_history(char history[MAXLINE][MAXLINE], int index, int historySize, char *home_dir);
void read_history(char history[MAXLINE][MAXLINE], int *counter, int *historySize, char *home_dir);
void print_history(char history[MAXLINE][MAXLINE], int index, int historySize);

#endif


#ifndef COMMAND_MANAGEMENT_H
#define COMMAND_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "config.h"
#include "tokenize.h"

void setup(char inBuffer[], char *args[], int *is_background_process, char history[MAXLINE][MAXLINE], int index, int historySize, char* home_dir);
void handle_command_execution(char *args[], int is_background_process);
void handle_cd_command(char *args[], char *home_dir, char history[MAXLINE][MAXLINE], int index);
int handle_sethistorysize_command(char *args[]);
void handle_rr_command(char *args[], char history[MAXLINE][MAXLINE], int index);
void handle_r_number_command(char *args[], char history[MAXLINE][MAXLINE], int index, int is_background_process);

#endif

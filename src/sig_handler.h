#ifndef SIG_HANDLER_H
#define SIG_HANDLER_H

#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

void sigchld_handler(int sig);
void register_sigchld_handler(void);

#endif

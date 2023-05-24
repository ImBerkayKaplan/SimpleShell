#include "sig_handler.h"

void sigchld_handler(int sig)
{
    while (1) {
        pid_t pid = waitpid(-1, 0, WNOHANG);
        
        // If there are no more children to reap, break the loop
        if (pid <= 0) {
            break;
        }
    }
}

void register_sigchld_handler(void) {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, 0) == -1) {
        perror("sigaction");
        exit(1);
    }
}

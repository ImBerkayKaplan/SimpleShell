#include "command_management.h"

// The function where a child process exectes a command and the parent will wait
void handle_command_execution(char *args[], int is_background_process) {
    // Fork a child process using fork()
    pid_t pid;
    if ((pid = fork()) < 0) {     
        perror("Forking unsuccessful");
        exit(EXIT_FAILURE);
    }

    // The child process will invoke execute the command
    if (pid == 0) {  
        if (execvp(args[0], args) < 0) {    
            perror("Execution failed");
            exit(1);
        }
    }
    // If is_background_process == 0, the parent will wait
    else {              
        if (is_background_process == 0){
            wait(NULL);
        }
    }
}

// Process cd command
void handle_cd_command(char *args[], char *home_dir, char history[MAXLINE][MAXLINE], int index) {
    // Save the command to history before executing it
    char command[MAXLINE] = "cd ";
    if(args[1] != NULL) {
        strncat(command, args[1], MAXLINE - strlen(command) - 1);
    }
    strncpy(history[index], command, sizeof(history[index]) - 1);
    
    if (home_dir == NULL) {
        printf("ERROR: HOME environment variable not set.\n");
        exit(EXIT_FAILURE);
    }
    // No arguments to cd or "~" was passed to cd
    if (args[1] == NULL || strcmp(args[1], "~") == 0) {  
        if (chdir(home_dir) != 0) {
            perror("chdir");
        }
    }
    // ".." was passed to cd
    else if (strcmp(args[1], "..") == 0) {  
        if (chdir("..") != 0) {
            perror("chdir");
        }
    } 
    // Some other argument was passed to cd
    else {  
        if (chdir(args[1]) != 0) {
            perror("chdir");
        }
    }
}

int handle_sethistorysize_command(char *args[]) {
    if(isNumber(args[1])){
        int historyNumber = atoi(args[1]);
        if(historyNumber > 0){
            return historyNumber;
        } else {
            printf("\nNegative values or 0 are not accepted.\n\n");
        }
    }else{
    	printf("\nError find in your sethistory command. Setting the history size to its default value, 12.\n\n");
    }
    return 12;
}

void handle_rr_command(char *args[], char history[MAXLINE][MAXLINE], int index) {
    char mostRecent[MAXLINE];
    strncpy(mostRecent, history[index - 1], sizeof(mostRecent) - 1);
    char *tokenized[MAXLINE/2+1];
    tokenize(mostRecent, tokenized);

    int i = 0;
    while(tokenized[i] != NULL){
        args[i] = strdup(tokenized[i]);
        i++;
    }
    args[i] = NULL;
}

void handle_r_number_command(char *args[], char history[MAXLINE][MAXLINE], int index, int is_background_process) {
    if(isNumber(args[1])){
        int number = atoi(args[1]) - 1;  // Subtracting 1 to make it 0-based index
        if (number < 0 || number >= MAXLINE){
            printf("\nInvalid index for the history buffer\n\n");
        } else if (number > index) {
            printf("\nNumber not in the history buffer\n\n");
        } else if (!strcmp(history[number], " ")) {
            printf("\nNo such history exists.\n\n");
        } else {
            char recentint[MAXLINE];
            strncpy(recentint, history[number], sizeof(recentint) - 1);
            tokenize(recentint, args);
            // Execute the command
            handle_command_execution(args, is_background_process);
        }
    }
}

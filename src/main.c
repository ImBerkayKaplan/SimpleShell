#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <libgen.h>

#include "sig_handler.h"
#include "file_error_checks.h"
#include "tokenize.h"
#include "command_management.h"
#include "history_management.h"

int historySize;

void setup(char inBuffer[], char *args[],int *is_background_process, char history[MAXLINE][MAXLINE], int index, int historySize, char* home_dir)
{
    
    // Clear the input buffer
    memset(inBuffer, 0, MAXLINE);

    /* Read what the user enters */
    int length = read(STDIN_FILENO, inBuffer, MAXLINE - 1);  

    // Check for read error or buffer overflow
    if (length < 0) {
	perror("error reading the input command");
	exit(EXIT_FAILURE);
    }
    if (length >= MAXLINE - 2) {
	perror("The input command is too long\n");
	exit(EXIT_FAILURE);
    }
    
    // null-terminate the string
    inBuffer[length] = '\0';
    
    /* Examine every character in the input buffer */
    for (int i = 0; i < length; i++) {
        if (inBuffer[i] == '&'){
            *is_background_process = 1;
            inBuffer[i] = '\0';
            break;
        }
    }    
    
    tokenize(inBuffer, args);  /* Get next command */
    args[MAXLINE/2] = NULL; /* Just in case the input line was > 80 */
} 

int main(void)
{

    // Get the home environment variable and check if it's set
    char *home_dir = getenv("HOME");
    if (home_dir == NULL) {
    	printf("ERROR: HOME environment variable not set.\n");
    	exit(EXIT_FAILURE);
    }
    
    char inBuffer[MAXLINE]; /* Input buffer  to hold the command entered */
    int is_background_process;             /* Equals 1 if a command is followed by '&', else 0 */
    char history[MAXLINE][MAXLINE];
    int counter = 0;
    historySize = 12;
    
    // Set up signal handler
    register_sigchld_handler();
    
    // Read history from file
    read_history(history, &counter, &historySize, home_dir);
    
    int index = counter;
    
    while (1){ /* Program terminates normally inside setup */

	is_background_process = 0;
	char *args[MAXLINE/2+1];/* Command line arguments */
	int deletedCommands = 0;
	char command[MAXLINE] = "";

	printf("Enter a command: ");  /* Shell prompt */
        fflush(0);

        setup(inBuffer, args, &is_background_process, history, index, historySize, home_dir);       /* Get next command */
	
	// Convert the command into a string
	int i = 0;
	while(args[i] != NULL){
	    if ((strlen(command) + strlen(args[i]) + 1) >= MAXLINE) {
		printf("Command is too long. Try again with a shorter command.\n");
		return 1;
	    }
	    strncat(command, args[i], MAXLINE - strlen(command) - 1);
	    if(args[i] != NULL){
		strncat(command, " ", MAXLINE - strlen(command) - 1);
	    }
	    i++;
	}
	
	// (sethistory Feature) Sets the size of the history
	if(args[0] != NULL && args[1] != NULL && args[2] == NULL){
	    if (!strcmp(*args, "sethistory")) {
		historySize = handle_sethistorysize_command(args);
		continue;
	    }
	    if (!strcmp(*args, "cd")) {
		handle_cd_command(args, home_dir, history, index);
		index++;
		continue;
	    }
	    // (r # Feature) Check if the user entered r #
	    if(!strcmp(*args, "r")) {
            	handle_r_number_command(args, history, index, is_background_process);
            	continue;
    	    }
	}

	// (rr Feature and Error Checking) Execute the most recent command
	if(args[0] != NULL && args[1] == NULL){
	    if(!strcmp(*args, "rr")){
		handle_rr_command(args, history, index);
		handle_command_execution(args, is_background_process);
		continue;
	    }
	    if (!strcmp(*args, "cd")) {
		handle_cd_command(args, home_dir, history, index);
		index++;
		continue;
	    }
	    // If the user asks for the history, print it
	    if(!(strcmp(*args, "h") && strcmp(*args, "history"))){
	    	print_history(history, index, historySize);
	    	continue;
	    }
	}
	
	// Save the command to history and to file
        if (args[0] != NULL) {
            strncpy(history[index], command, sizeof(history[index]) - 1);
            save_history(history, index, historySize, home_dir);
            index++;
            // execvp should not be called if the command is cd. Cd is a built-in shell command.
            handle_command_execution(args, is_background_process);
        }
    }
}

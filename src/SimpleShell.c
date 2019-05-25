#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#define MAXLINE 80 /* 80 chars per line, per command, should be enough. */

// Checks if a given string is made up of digits
int isnumber(const char *str) 
{
    // Go through all characters to make sure they are digits.
    int length = strlen (str), result = 1, i;
    for (i = 0;i<length; i++){
        if (!isdigit(str[i]) && str[i]!='-')
        {
		result = 0;
        }
    }
    return result;
}

// Tokenizes the string into an array of strings
void tokenize(char inBuffer[], char *args[]){

    int start = -1, j = 0, len = strlen(inBuffer), i;
    
    /* Examine every character in the input buffer (Same as the function setup)*/ 
    for (i = 0; i < len; i++) {
        if(i == (len - 1)){
            if (start != -1){
                args[j] = &inBuffer[start];     
                j++;
            }
            inBuffer[i] = '\0';
            args[j] = NULL;
	}else{
            switch (inBuffer[i]){
                case ' ':  
                    if(start != -1){
                        args[j] = &inBuffer[start];    
                        j++;
                    }
                    inBuffer[i] = '\0'; 
                    start = -1;
                    break;         
                default :
                    if (start == -1)
                        start = i;
	    }
        }  
    }  
    args[j] = NULL;
}

void setup(char inBuffer[], char *args[],int *bkgd, char history[MAXLINE][MAXLINE], int index, int historySize)
{
    int length,  /* #  characters in the command line */
        start,   /* Beginning of next command parameter           */
        i,       /* Index for inBuffer arrray          */
        j;       /* Where to place the next parameter into args[] */
    /* Read what the user enters */
    length = read(STDIN_FILENO, inBuffer, MAXLINE);  

    start = -1;
    j = 0;



    if (length == 0){
	FILE *file;
	char filepath[MAXLINE];
	strcpy(filepath, getenv("HOME"));
	strcat(filepath, "/history.txt");
        file = fopen(filepath,"w+");
	int counter6;
	fprintf(file, "%d\n", historySize);
	for(counter6=0;counter6<index;counter6++){
	    fprintf(file, "%d-%s\n", counter6 + 1,history[counter6]);
	}
	fclose(file);
        exit(0);            /* Cntrl-d was entered, end of user command stream */
    }
    if (length < 0){
        perror("error reading command");
	exit(-1);           /* Terminate with error code of -1 */
    }

    /* Examine every character in the input buffer */
    for (i = 0; i < length; i++) {
        switch (inBuffer[i]){
        case ' ':
        case '\t' :          /* Argument separators */

            if(start != -1){
                args[j] = &inBuffer[start];    /* Set up pointer */
                j++;
            }

            inBuffer[i] = '\0'; /* Add a null char; make a C string */
            start = -1;
            break;

        case '\n':             /* Final char examined */
            if (start != -1){
                args[j] = &inBuffer[start];     
                j++;
            }

            inBuffer[i] = '\0';
            args[j] = NULL; /* No more arguments to this command */
            break;

        case '&':
            *bkgd = 1;
            inBuffer[i] = '\0';
            break;
            
        default :             /* Some other character */
            if (start == -1)
                start = i;
	}
 
    }    
    args[j] = NULL; /* Just in case the input line was > 80 */
} 

int main(void)
{
    char inBuffer[MAXLINE]; /* Input buffer  to hold the command entered */
    int bkgd;             /* Equals 1 if a command is followed by '&', else 0 */
    char history[MAXLINE][MAXLINE],c;
    int index = 0, historySize = 12, counter4 = 0;

    // Opening the saved file to retrive the history
    FILE *file;
    char filepath[MAXLINE];
    strcpy(filepath, getenv("HOME"));
    strcat(filepath, "/history.txt");
    if (access( filepath, F_OK ) != -1) {
	file = fopen(filepath, "r");
	char item[MAXLINE], trimmeditem[MAXLINE];
	fgets(item, sizeof(item),file);
	historySize = atoi(item);
        while (fgets(item, sizeof(item),file)!=NULL) {
	    int counter5, startfrom = 0;
	    for(counter5 = 0; counter5 < strlen(item)-1; counter5++){
		if(item[counter5] == '-'){
		    startfrom = counter5+1;
		    break;
		}
	    }
	    int len = strlen(item);
	    item[len - 1] = '\0';
	    strcpy(history[counter4], &item[startfrom]);
	    counter4++;
        }
	fclose(file);
    }
    
    index = counter4;

    while (1){ /* Program terminates normally inside setup */

	bkgd = 0;
	char *args[MAXLINE/2+1];/* Command line arguments */
	int historyChecker = 1, deletedCommands = 0, start = 0, cdchecker = 1;
	char command[MAXLINE] = "";

	printf("Enter a command: ");  /* Shell prompt */
        fflush(0);

        setup(inBuffer, args, &bkgd, history, index, historySize);       /* Get next command */
	pid_t pid;
	
	// Convert the command into a string
	int counter1 = 0;
	while(args[counter1] != NULL){
	    strcat(command, args[counter1]);
            if(args[counter1] != NULL){
		strcat(command, " ");
	    }
	    counter1++;
	}
	
	// (sethistory Feature) Sets the size of the history
	if(args[0] != NULL && args[1] != NULL && args[2] == NULL){

	    // Set the sethistory to the new variable
	    if(isnumber(args[1]) && !strcmp(*args, "sethistory")){
	        historyChecker = 0;
	        int historyNumber = atoi(args[1]);

		// Error checking to make sure the user enters a positive number
	        if(historyNumber > 0){
		    historySize = historyNumber;
	        }else{
		    printf("\nNegative values or 0 are not accepted.\n\n");
	        }
	    }

	    // Handle the request for changing directory
	    if(!strcmp(*args, "cd")){
		char currentPath[MAXLINE]; 
   		getcwd(currentPath,sizeof(currentPath));
   		strcat(currentPath,"/");
   		strcat(currentPath,args[1]);
		if(chdir(currentPath)){
		    char abspath[MAXLINE];
		    strcpy(abspath, getenv("HOME"));
		    strcat(abspath, "/");
		    strcat(abspath, args[1]);
		    if(chdir(abspath)){
			printf("Execution failed. Working directory was not changed.\n");
		    }
		}
		cdchecker = 0;
	    }
	}

	// (rr Feature and Error Checking) Execute the most recent command
	if(args[0] != NULL && args[1] == NULL && historyChecker){
	    if(!strcmp(*args, "rr")){
	        char mostRecent[MAXLINE];
	        strcpy(mostRecent,history[index-1]);
                strcpy(command, mostRecent);
	        tokenize(mostRecent, args);
	    }

	    // Handle the request for changing directory
	    if(!strcmp(*args, "cd")){
		if(chdir(getenv("HOME"))){
		    printf("Execution failed. Working directory was not changed.\n");
		}
		cdchecker = 0;
	    }
	}

	// (r # Feature) Check if the user entered r #
	if(args[0] != NULL && args[1] != NULL && args[2] == NULL && historyChecker){
	    if(!strcmp(*args, "r")){
	        if(isnumber(args[1])){
	            int number = atoi(args[1]) - 1;
	            char recentint[MAXLINE];

	            // Error checking of arg[1] and printing messages
	            if(0>number){
		        historyChecker = 0;
		        printf("\nNegative values or 0 are not accepted.\n\n");
                    }
	            if(number > index){
		        historyChecker = 0;
		        printf("\nNumber not in the history buffer\n\n");
	            }
	            if(!strcmp(history[number], " ")){
		        historyChecker = 0;
		        printf("\nNo such history exists.\n\n");
	            }
	    
	            // Execute the command if all conditions are valid
	            if(historyChecker){
		        strcpy(recentint,history[number]);
			strcpy(command, recentint);
	                tokenize(recentint, args);
	            }
	        }else{
		    // Find the most recent command that starts with args[1]
		    int arglen = strlen(args[1]), validChecker = 1;
		    char recentstring[MAXLINE];
		    int counter3;
		    for(counter3 = index - 1; counter3 > 0; counter3--){
			if(!strncmp (*(args + 1),history[counter3],arglen)){
			    strcpy(recentstring,history[counter3]);
			    strcpy(command, recentstring);
	                    tokenize(recentstring, args);
			    validChecker = 0;
			    break;
			}
		    }
	
		    // Check if the command has been found that starts with the input string
		    if(validChecker){
			printf("\nString does not match any elements in history.\n\n");
			historyChecker = 0;
		    }
		}
	    }
	}

	// (History Checker) If the command is already in history, delete the older command
	int counter2;
        for(counter2 = index; counter2 >= 0; counter2--){
	    if(!strcmp(history[counter2],command)){   
    		strcpy(history[counter2]," ");
	    }
	    if(strcmp(history[counter2]," ")){
		deletedCommands++;
	    }
	    if(deletedCommands == historySize){
		start = counter2;
	    }
        }

	// (History Present) Checks if the user requests for history
	if(args[0] != NULL && historyChecker){
	    // Put the command to the history
	    strcpy(history[index],command);
            index++;
	    if(!(strcmp(*args, "h") && strcmp(*args, "history"))){
	    // If the user asks for the history, print it
	        if(args[1] == NULL){
	            historyChecker = 0;
	            for(; start < index; start++){
		        if(strcmp(history[start]," ")){
			    printf("%d-%s\n",start + 1, history[start]);
		        }
	            }
	        }
	    }
	}

	// Fork a child process using fork()
        if ((pid = fork()) < 0) {     
            printf("Forking unsuccessful\n");
            exit(1);
        }

	// The child process will invoke execvp()
	if (pid == 0) {  
            if (execvp(args[0], args) < 0) {    
		if(historyChecker && cdchecker){
                    printf("Execution failed\n");
		}
                exit(1);
	    }
        }
        // If bkgd == 0, the parent will wait, o/w returns to the setup() function
        else {              
            if (bkgd == 0){
		wait(&pid);
	    }
        }
    }
}

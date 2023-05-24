#include "history_management.h"
#include "file_error_checks.h"

#include <stdio.h>
#include <stdlib.h>
#include "history_management.h"

void save_history(char history[MAXLINE][MAXLINE], int index, int historySize, char *home_dir) {
    FILE *file;
    char filepath[MAXLINE];
    snprintf(filepath, sizeof(filepath), "%s/%s", home_dir, "history.txt");
	
    // Check if we have access to the file
    file = fopen(filepath, "w");
    // Check if the file was successfully opened
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    	
    // Write to the file and check if the write was successfull.
    fprintf(file, "%d\n", historySize);
    checkFileWriteError(file);

    for(int i=0;i<index;i++){
        char temp[MAXLINE];
        snprintf(temp, sizeof(temp), "%d-%s\n", i + 1, history[i]);
        fprintf(file, "%s", temp);
        checkFileWriteError(file);
    }
    fclose(file);
}

// Reads history from file
void read_history(char history[MAXLINE][MAXLINE], int *counter, int *historySize, char *home_dir) {
    // Open the saved file to retrive the history
    FILE *file;
    char filepath[MAXLINE];
    snprintf(filepath, sizeof(filepath), "%s/%s", home_dir, "history.txt");
    file = fopen(filepath, "r");
    // Check if the file was successfully opened
    if (file == NULL) {
        perror("The program did not open the command history file. But, it will create a new empty one. The error is");
    } else {
        char item[MAXLINE], trimmeditem[MAXLINE];
        fgets(item, sizeof(item),file);
        checkFileReadError(file);
        *historySize = atoi(item);
        while (fgets(item, sizeof(item),file)!=NULL) {
            checkFileReadError(file);
            int startfrom = 0;
            int itemLength = strlen(item) - 1;
            for(int i = 0; i < itemLength; i++){
                if(item[i] == '-'){
                    startfrom = i+1;
                    break;
                }
            }
            int len = strlen(item);
            item[len - 1] = '\0';
            strncpy(history[*counter], &item[startfrom], sizeof(history[*counter]) - 1);
            (*counter)++;
            }
        fclose(file);
    }
}

void print_history(char history[MAXLINE][MAXLINE], int index, int historySize) {
    int start = index - historySize > 0 ? index - historySize : 0;
    for(int i = start; i < index; i++){
        printf("%d-%s\n",i + 1, history[i]);
    }
}

#include "tokenize.h"

// Checks if a given string is made up of digits
int isNumber(const char *str) {
    char *endptr;
    strtol(str, &endptr, 10);  // 10 specifies base 10 (decimal)

    // Check if conversion succeeded and the entire string was consumed
    return (*str != '\0' && *endptr == '\0');
}

// Tokenizes the string into an array of strings
void tokenize(char inBuffer[], char *args[]){
    int start = -1, j = 0, len = strlen(inBuffer);
    char quote_char = '\0'; // holds the current quote character (' or ")
    for (int i = 0; i < len; i++) {
        switch (inBuffer[i]){
            case ' ':  
            case '\t' :
            case '\n':
                if (quote_char == '\0' && start != -1) { // end of an argument
                    args[j] = &inBuffer[start];    
                    j++;
                    inBuffer[i] = '\0'; 
                    start = -1;
                }
                break;
            case '\'':
            case '\"':
                if (quote_char == '\0') { // start of a quoted argument
                    quote_char = inBuffer[i];
                    start = i + 1;
                } else if (inBuffer[i] == quote_char) { // end of a quoted argument
                    quote_char = '\0';
                    args[j] = &inBuffer[start];    
                    j++;
                    inBuffer[i] = '\0'; 
                    start = -1;
                }
                break;
            default :
                if (start == -1)
                    start = i;
        }
        if(i == (len - 1) && start != -1){
            args[j] = &inBuffer[start];     
            j++;
            inBuffer[i] = '\0';
        }  
    }  

    // Check for unmatched quote
    if (quote_char != '\0') {
        printf("ERROR: Unmatched %c\n", quote_char);
        exit(1);
    }

    args[j] = NULL;
}

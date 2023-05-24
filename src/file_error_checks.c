#include "file_error_checks.h"

void checkFileReadError(FILE *file) {
    if (ferror(file)) {
        printf("Error reading from file\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
}

void checkFileWriteError(FILE *file) {
    if (ferror(file)) {
        printf("Error writing to file\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
}

#include <stdio.h>
#include <stdlib.h>

int main() {
    int bSize = 5;

    // Allocate memory for an array of char* pointers
    char **path = (char**)malloc(bSize * sizeof(char*));

    for (int i = 0; i < bSize; i++) {
        // Allocate memory for individual strings (columns)
        *(path + i) = (char*)malloc(bSize * sizeof(char));

        for (int j = 0; j < bSize; j++) {
            // Initialize each element to '0'
            *(*(path + i) + j) = '0';
        }
    }

    // Print the 2D array using pointer arithmetic
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            printf("%c ", *(*(path + i) + j));
        }
        printf("\n");
    }

    // Free the memory for individual strings (columns)
    for (int i = 0; i < bSize; i++) {
        free(*(path + i));
    }

    // Free the memory for the array of char* pointers
    free(path);

    return 0;
}


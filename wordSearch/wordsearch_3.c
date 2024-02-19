#include <stdio.h>

#include <stdlib.h>

#include <string.h>

// Declarations of the two functions you will implement

// Feel free to declare any helper functions or global variables

void printPuzzle(char** arr);

void searchPuzzle(char** arr, char* word);

int bSize;

// Main function, DO NOT MODIFY

int main(int argc, char **argv) {

if (argc != 2) {

fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);

return 2;

}

int i, j;

FILE *fptr;

// Open file for reading puzzle

fptr = fopen(argv[1], "r");

if (fptr == NULL) {

printf("Cannot Open Puzzle File!\n");

return 0;

}

// Read the size of the puzzle block

fscanf(fptr, "%d\n", &bSize);

// Allocate space for the puzzle block and the word to be searched

char **block = (char**)malloc(bSize * sizeof(char*));

char *word = (char*)malloc(20 * sizeof(char));

// Read puzzle block into 2D arrays

for(i = 0; i < bSize; i++) {

*(block + i) = (char*)malloc(bSize * sizeof(char));

for (j = 0; j < bSize - 1; ++j) {

fscanf(fptr, "%c ", *(block + i) + j);

}

fscanf(fptr, "%c \n", *(block + i) + j);

}

fclose(fptr);

printf("Enter the word to search: ");

scanf("%s", word);

// Print out original puzzle grid

printf("\nPrinting puzzle before search:\n");

printPuzzle(block);

// Call searchPuzzle to the word in the puzzle

searchPuzzle(block, word);

return 0;

}

void printPuzzle(char** arr) {

    // This function will print out the complete puzzle grid (arr).

    // It must produce the output in the SAME format as the samples

    // in the instructions.

    // Your implementation here...


    for(int i = 0; i < bSize; i++){

    for(int j = 0; j< bSize; j++){

    printf("%c ", *(*(arr+i)+j));

    }

    printf("\n");

    }

}

void upper(char* word){

    int s_word = strlen(word);

    for(int i =0; i < s_word; i++){

    if(*(word+i) <= 'z' && *(word+i) >= 'a'){

    *(word+i) -= 32;

    }

    }

}

int search(char** arr, char * word, int row, int column, int key, char *current, char** path){
    int found;
    //not top row
    if(row != 0){
        if(*(*(arr+row-1)+column) == word[key]){
            *(current+key) = *(*(arr+row-1)+column);
            *(*(path+row-1)+column) = key+1 + '0';
            search(arr, word, row-1, column, key+1, current,path);
        }

        //not left column
        if(column != 0){
            if(*(*(arr+row-1)+column-1) == word[key]){
                *(current+key) = *(*(arr+row-1)+column-1);
                *(*(path+row-1)+column-1) = key+1 + '0';
                search(arr, word, row-1, column-1, key+1, current,path);
            }
        }
    }

    //not left column
    if(column != 0){
        if(*(*(arr+row)+column-1) == word[key]){
            *(current+key) = *(*(arr+row)+column-1);
            *(*(path+row)+column-1) = key+1 + '0';
            search(arr, word, row, column-1, key+1, current,path);
        }

        //not bottom row
        if(row != bSize-1){
            if(*(*(arr+row+1)+column-1) == word[key]){
            *(current+key) = *(*(arr+row+1)+column-1);
            *(*(path+row+1)+column-1) = key+1 + '0';
            search(arr, word, row+1, column-1, key+1, current,path);
            }
        }
    }
    // not bottom row
    if(row != bSize -1){
        if(*(*(arr+row+1)+column) == word[key]){
            *(current+key) = *(*(arr+row+1)+column);
            *(*(path+row+1)+column) = key+1 + '0';
            search(arr, word, row+1, column, key+1, current,path);

        }

        //not right column
        if(column != bSize -1){
            if(*(*(arr+row+1)+column+1) == word[key]){
                *(current+key) = *(*(arr+row+1)+column+1);
                *(*(path+row+1)+column+1) = key+1 + '0';
                search(arr, word, row+1, column+1, key+1, current,path);
            }

        }
    }
    //not right column
    if(column != bSize-1){
        if(*(*(arr+row)+column+1) == word[key]){
            *(current+key) = *(*(arr+row)+column+1);
            *(*(path+row)+column+1) = key+1 + '0';
            search(arr, word, row, column+1, key+1, current,path);
        }

    //not top row
        if(row != 0){
            if(*(*(arr+row-1)+column+1) == word[key]){ 
                *(current+key) = *(*(arr+row-1)+column+1);
                *(*(path+row-1)+column+1) = key+1 + '0';
                search(arr, word, row-1, column+1, key+1, current,path);
            }
        }
    }

    // printf("%s\n", word);
    // printf("%s,%d,%d,%d\n", current,key,row,column);
    for(int i = 0; i < strlen(word); i++){
        if(word[i] == current[i]){
            found = 1;
        }else{
            return 0;
        }
    }
    return found;
}

void searchPuzzle(char** arr, char* word) {

    // This function checks if arr contains the search word. If the
    // word appears in arr, it will print out a message and the path
    // as shown in the sample runs. If not found, it will print a
    // different message as shown in the sample runs.
    // Your implementation here...

    upper(word);

    char **path = (char**)malloc(bSize * sizeof(char*));

    for(int i = 0; i < bSize; i++){
        *(path + i) = (char*)malloc(bSize * sizeof(char));
        for(int j = 0; j < bSize; j++){

            *(*(path+i)+j) = '0';
        }
    }

    int key = 0;
    int row, column;
    int found;
    char* current;
    int final;

    for(int i = 0; i < bSize; i++){
        for(int j = 0; j< bSize; j++){
            if(*(*(arr+i)+j) == word[key]){
                row = i;
                column = j;
                key = 1;
                current = (char*)malloc(20 * sizeof(char));
                current[0] = *(*(arr+row)+column);
                found = search(arr, word, row, column, key, current,path);

                if(found == 1){
                    final = 1;
                }
                key = 0;
            }
        }
    }

    printf("\n");
    if(final == 1){
        printf("%s\n", "Word found!");
    }else{
        printf("%s\n", "Word not found!");
    }
    printPuzzle(path);
}
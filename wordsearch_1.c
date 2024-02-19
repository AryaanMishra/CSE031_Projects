/*
DONE 1. Change the input to upper case
2. Use recursion in the searchPuzzle function
3. it should recursively find each letter of the word and store it to an array
4. if and only if it finds all the letters in the words, then print it
5. if not it moves on to a different letter combination
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
void upper(char* word);
int check(char** arr, char* word, int row, int col, int index, int* isFound, char **path, int* pathIndices);
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

void case_insensitivity(char* word){
    while (*word){
        if((*word >= 'a') && (*word <= 'z')){
            *word -= 32;
        }
        word++;
    }
}

void printPuzzle(char** arr) {
    // This function will print out the complete puzzle grid (arr). 
    // It must produce the output in the SAME format as the samples 
    // in the instructions.
    // Your implementation here...

}

void searchPuzzle(char** arr, char* word) {
    // This function checks if arr contains the search word. If the 
    // word appears in arr, it will print out a message and the path 
    // as shown in the sample runs. If not found, it will print a 
    // different message as shown in the sample runs.
    // Your implementation here...

    //introduce new variables 
    int pos = 0;
    int row;
    int column;
    int isFound = 0;
    char* letters;
    char **path = (char**)malloc(bSize * sizeof(char*));


    //convert word to uppercase.
    case_insensitivity(word);

    //create a 2d array of 0s.
    //allocate memory for the array
    char **block = (char**)malloc(bSize * sizeof(char*));
    for(int i = 0; i < bSize; i++){
        //allocate memory for each string
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for(int j = 0; j < bSize - 1; j++){
            // Initialize each element to '0'
            *(*(block + i) + j) = '0';
        }
    }

    //iterate through the grid to match letter
    for(int i =0; i < bSize; i++){
        for(int j =0; j < bSize; j++){
            if(*(*(arr + i) + j) == *word){
                // Mark current cell as visited to avoid revisiting in backtracking
                *(*(arr + i) + j) = '#';
                pos = 1;
                row = i;
                column = j;
                letters = (char*)malloc(20 * sizeof(char));
                *(letters + 0) = *(*(arr+row)+column);

                if(explorePaths(arr, word, i, j, pos, &isFound, path, 0) == 1){

                }

    
            }
        }
    }
    



}

int check(char** arr, char* word, int row, int col, int index, int* isFound, char **path, int* pathIndices){

    return 1;
}
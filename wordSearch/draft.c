#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
void cap(char* word);
void printPath(int** arr_out);
void printChop(int arr_out_num);
int bSize;
int wSize = 0;
void pathDeletion(int** arr_out, int firstI, int firstJ);
bool letterFinder(char** arr, char* word, int tally, int i, int j, int** arr_out, bool first);
bool wordFinder(char** arr, char* word, int** arr_out);

// Main function, DO NOT MODIFY
int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL){
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);

    // Allocate space for the puzzle block and the word to be searched
    char **block = (char **)malloc(bSize * sizeof(char *));
    char *word = (char *)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for (i = 0; i < bSize; i++){
        *(block + i) = (char *)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j){
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

// self explanitory function
void printPuzzle(char** arr){                                   
    for (int i = 0; i < bSize; i++){
        for (int j = 0; j < bSize; j++){
            printf("%c ", *(*(arr + i) + j));
        }
        printf("\n");
    }
}

// Capitalizes the lowercase letters in the word that needs to be found
void cap(char* word){
    for(int i = 0; i < strlen(word); i++){
        if(*(word + i) >= 'a' && *(word + i) <= 'z'){
            *(word + i) = *(word + i) - 32;
        }
    }
}

// Concatenates multiple instances of repeating letters
void printChop(int arr_out_num){
    int order = 0;
    int temp = arr_out_num;

    // inputting the value that is multiplied by 10 per letter that we found (later on in the code)
    // if we divide by 10 as temp or the inputted variable is greater than 0
    // we add a tally in order
    while(temp > 0){
        temp /= 10;
        order++;
    }

    // Used for printing out path and creating spaces
    printf("%d", arr_out_num);
    printf("\t");
}

// Appends to the empty pathDeletion function to create a matrix that has the path
void printPath(int** arr_out){
    for (int i = 0; i < bSize; i++){
        for (int j = 0; j < bSize; j++){
            // if arr_out[i][j] is equal to 0
            if (*(*(arr_out + i) + j) == 0){
                printf("0");    // print 0 with the tab space that it needs
                printf("\t");
            }
            else{
                printChop(*(*(arr_out + i) + j));   // else run the value that was multiplied by 10 into the chop function
            }
        }
        printf("\n");
    }
}

// Empty matrix of zeros to append path to
void pathDeletion(int** arr_out, int starti, int startj){
    for (int i = 0; i < bSize; i++){
        for (int j = 0; j < bSize; j++){
            // if starti or startj aren't equal to the current position of i, j
            if (starti != i || startj != j){
                *(*(arr_out + i) + j) = 0;      // set the current postition of arr_out[i][j] equal to 0
            }
        }
    }
}

// Searches surrounding letters around the indexed letter
bool letterFinder(char** arr, char* word, int tally, int i, int j, int** arr_out, bool first){
    // Word index is created to accomodate for the indexing
    int wordIndex = bSize - 1;


    // Base cases to find the letter in the 2d array
    // If both i and j are larger than the wordIndex, then the word does not exist in the 2d array
    if (i > wordIndex || j > wordIndex){
        return false;
    }

    // If the letter does not exist entirely, then the word does not exist
    if (*(word + tally) != *(*(arr + i) + j)){
        return false;
    }

    // If the length of the word is equal to the sum of tally + 1
    if (strlen(word) == tally + 1){
        *(*(arr_out + i) + j) = *(*(arr_out + i) + j) * 10 + tally + 1;     // Sets the int 2d array to be equal to itself times 10, plus the sum itself as well (for the last letter)
        return true;                                                        // Return true for found letter
    }

    *(*(arr_out + i) + j) = *(*(arr_out + i) + j) * 10 + tally + 1;         // Sets the int 2d array to be equal to itself times 10, plus the sum itself as well (again, for each letter it finds)


    // 8 if statements that find the letter adjacent to the center letter found at the beginning
    // Top right from index
    if (i > 0 && j > 0){
        // if letterFinder finds the word and is not the first letter in word, then return true
        if (letterFinder(arr, word, tally + 1, i - 1, j - 1, arr_out, false)){
            return true;
        }

        // If it is the first letter in word, then run pathDeletion (backtrace)
        // In more detail, if it is true, then delete the path from the output integer 2d array
        if (first == true){
            pathDeletion(arr_out, i, j);
        }
    }

    // Above index
    if (i > 0){
        if (letterFinder(arr, word, tally + 1, i - 1, j, arr_out, false)){
            return true;
        }
        if (first == true){
            pathDeletion(arr_out, i, j);
        }
    }

    // Top left from index
    if (i > 0 && j < wordIndex){
        if (letterFinder(arr, word, tally + 1, i - 1, j + 1, arr_out, false)){
            return true;
        }
        if (first == true){
            pathDeletion(arr_out, i, j);
        }
    }

    // Left of index
    if (j > 0){
        if (letterFinder(arr, word, tally + 1, i, j - 1, arr_out, false)){
            return true;
        }
        if (first == true){
            pathDeletion(arr_out, i, j);
        }
    }
    
    // Right of index
    if (j < wordIndex){
        if (letterFinder(arr, word, tally + 1, i, j + 1, arr_out, false)){
            return true;
        }
        if (first == true){
            pathDeletion(arr_out, i, j);
        }
    }    

    // Bottom left from index
    if (j > 0 && i < wordIndex){
        if (letterFinder(arr, word, tally + 1, i + 1, j - 1, arr_out, false)){
            return true;
        }
        if(first == true){
            pathDeletion(arr_out, i, j);
        }
    }

    // Under index
    if (i < wordIndex){
        if (letterFinder(arr, word, tally + 1, i + 1, j, arr_out, false)){
            return true;
        }
        if (first == true){
            pathDeletion(arr_out, i, j);
        }
    }

    // Bottom left from index
    if (j < wordIndex && i < wordIndex){
        if (letterFinder(arr, word, tally + 1, i + 1, j + 1, arr_out, false)){
            return true;
        }
        if (first == true){
            pathDeletion(arr_out, i, j);
        }
    }

    return false;   // If all if statements fail, then letter not found, word not found
}

// Recurvisely calls the letter finder function to find the word
bool wordFinder(char** arr, char* word, int** arr_out){
    for (int i = 0; i < bSize; i++){
        for (int j = 0; j < bSize; j++){
            // If letter finder does find the first letter in the word, then return function as true
            if (letterFinder(arr, word, 0, i, j, arr_out, true) == true){
                return true;
            }
            for (int i = 0; i < bSize; i++){
                for (int j = 0; j < bSize; j++){
                    *(*(arr_out + i) + j) = 0;      // To fill in the other spaces, place 0
                }
            }
        }
    }

    //  Return false if word not found
    return false;
}

void searchPuzzle(char** arr, char* word){
    // Capitalize the word
    cap(word);

    // Creates and fills integer 2d array of the same size as the char 2d array with 0's
    int** arr_out = (int**)malloc(bSize * sizeof(int*));
    for (int i = 0; i < bSize; i++){
        *(arr_out + i) = (int *)malloc(bSize * sizeof(int));
        for (int j = 0; j < bSize; j++){
            *(*(arr_out + i) + j) = 0;
        }
    }

    // If the word finder did completely find the word, print the path
    if (wordFinder(arr, word, arr_out) == true){
        printf("\nWord found!\n");
        printf("Printing the search path: \n");

        printPath(arr_out);
    }
    // else print error message
    else{
        printf("\nWord not found!\n");
    }
}
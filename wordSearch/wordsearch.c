//Group Members: Ayjeren Gochyyeva and Aryaan Mishra

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void printPuzzle(char** arr); //print the puzzle
void searchPuzzle(char** arr, char* word); //serach for word

void toUpper(char* word); //conver word to uppercase
void showPath(int** arr_out); //show the path of the word in the puzzle
void showChop(int arr_out_num); // print a number with tab space

void deletePath(int** arr_out, int firstI, int firstJ); //delete from the output array
bool findLetter(char** arr, char* word, int count, int i, int j, int** arr_out, bool first); //find a latter in the puzzle
bool findWord(char** arr, char* word, int** arr_out); //find a word
bool checkBounds(int i, int j);  // Function to check if indices are within bounds
bool checkLetter(char** arr, char* word, int count, int i, int j);  // Function to check if a letter matches
void updatePath(int** arr_out, int count, int i, int j);  // Function to update the path
//void resetPath(int** arr_out, int i, int j);  // Function to reset the path

int bSize; //size of puzzle
int wordSize = 0; //wordsize

int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;

    fptr = fopen(argv[1], "r");
    if (fptr == NULL){
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }
    fscanf(fptr, "%d\n", &bSize);

    char **block = (char **)malloc(bSize * sizeof(char *));
    char *word = (char *)malloc(20 * sizeof(char));

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

    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);

    searchPuzzle(block, word);

    return 0;
}

// Function to print the puzzle
void printPuzzle(char** arr){                                   
    for (int i = 0; i < bSize; i++){
        for (int j = 0; j < bSize; j++){
            printf("%c ", *(*(arr + i) + j));
        }
        printf("\n");
    }
}

// Function to convert a word to uppercase
void toUpper(char* word){
    for(int i = 0; i < strlen(word); i++){
        if(*(word + i) >= 'a' && *(word + i) <= 'z'){
            *(word + i) = *(word + i) - 32;
        }
    }
}

// Function to print a number with a tab space
void showChop(int arr_out_num){
    int order = 0;
    int temp = arr_out_num;

    while(temp > 0){
        temp /= 10;
        order++;
    }

    printf("%d", arr_out_num);
    printf("\t");
}

// Function to show the path of the word in the puzzle
void showPath(int** arr_out){
    for (int i = 0; i < bSize; i++){
        for (int j = 0; j < bSize; j++){
            if (*(*(arr_out + i) + j) == 0){
                printf("0");  
                printf("\t");
            }
            else{
                showChop(*(*(arr_out + i) + j)); 
            }
        }
        printf("\n");
    }
}

// Function to delete the path from the output array
void deletePath(int** arr_out, int starti, int startj){
    for (int i = 0; i < bSize; i++){
        for (int j = 0; j < bSize; j++){
            if (starti != i || startj != j){
                *(*(arr_out + i) + j) = 0;     
            }
        }
    }
}

// Function to check if the indices are within the bounds of the puzzle
bool checkBounds(int i, int j){
    int wordIndex = bSize - 1;
    return !(i > wordIndex || j > wordIndex);
}

// Function to check if a letter matches the current letter of the word
bool checkLetter(char** arr, char* word, int count, int i, int j){
    return *(word + count) == *(*(arr + i) + j);
}

// Function to update the path in the output array
void updatePath(int** arr_out, int count, int i, int j){
    *(*(arr_out + i) + j) = *(*(arr_out + i) + j) * 10 + count + 1; //allows us to store multiple position in a single cell of an array
}

// Function to find a letter in the puzzle
//bool first checks if this is the first attempt to find the word
bool findLetter(char** arr, char* word, int count, int i, int j, int** arr_out, bool first){
    // Check if the indices are within bounds and if the letter matches
    // Checks if the letter at that position matches the count-th letter of the word
    if (!checkBounds(i, j) || !checkLetter(arr, word, count, i, j)){
        return false;
    }

    // If the entire word has been found, update the path and return true
    if (strlen(word) == count + 1){
        updatePath(arr_out, count, i, j);
        return true;
    }

    // Update the path with the current letter
    updatePath(arr_out, count, i, j);

    // Recursive calls to find the letter in all directions
    // If the letter is found, return true
    // If the letter is not found and it's the first letter, delete the path
    // The same process is repeated for all 8 directions
    if (i > 0 && j > 0){
        if (findLetter(arr, word, count + 1, i - 1, j - 1, arr_out, false)){
            return true;
        }
        if (first){
            deletePath(arr_out, i, j);
        }
    }

    if (i > 0){
        if (findLetter(arr, word, count + 1, i - 1, j, arr_out, false)){
            return true;
        }
        if (first){
            deletePath(arr_out, i, j);
        }
    }

    if (i > 0 && j < bSize - 1){
        if (findLetter(arr, word, count + 1, i - 1, j + 1, arr_out, false)){
            return true;
        }
        if (first){
            deletePath(arr_out, i, j);
        }
    }

    if (j > 0){
        if (findLetter(arr, word, count + 1, i, j - 1, arr_out, false)){
            return true;
        }
        if (first){
            deletePath(arr_out, i, j);
        }
    }
    
    if (j < bSize - 1){
        if (findLetter(arr, word, count + 1, i, j + 1, arr_out, false)){
            return true;
        }
        if (first){
            deletePath(arr_out, i, j);
        }
    }    

    if (j > 0 && i < bSize - 1){
        if (findLetter(arr, word, count + 1, i + 1, j - 1, arr_out, false)){
            return true;
        }
        if (first){
            deletePath(arr_out, i, j);
        }
    }

    if (i < bSize - 1){
        if (findLetter(arr, word, count + 1, i + 1, j, arr_out, false)){
            return true;
        }
        if (first){
            deletePath(arr_out, i, j);
        }
    }

    if (j < bSize - 1 && i < bSize - 1){
        if (findLetter(arr, word, count + 1, i + 1, j + 1, arr_out, false)){
            return true;
        }
        if (first){
            deletePath(arr_out, i, j);
        }
    }

    // Return false if the letter is not found
    return false;   
}

// Function to find a word in the puzzle
bool findWord(char** arr, char* word, int** arr_out){
    // Iterate over the entire puzzle
    for (int i = 0; i < bSize; i++){
        for (int j = 0; j < bSize; j++){
            // If the word is found, return true
            if (findLetter(arr, word, 0, i, j, arr_out, true) == true){
                return true;
            }
            // If the word is not found, reset the path
            for (int i = 0; i < bSize; i++){
                for (int j = 0; j < bSize; j++){
                    *(*(arr_out + i) + j) = 0;
                }
            }
        }
    }

    return false;
}

// Function to search the puzzle for a word
void searchPuzzle(char** arr, char* word){
    toUpper(word);

    // Initialize the output array
    int** arr_out = (int**)malloc(bSize * sizeof(int*));
    for (int i = 0; i < bSize; i++){
        *(arr_out + i) = (int *)malloc(bSize * sizeof(int));
        for (int j = 0; j < bSize; j++){
            *(*(arr_out + i) + j) = 0;
        }
    }

    // If the word is found, print the path
    if (findWord(arr, word, arr_out) == true){
        printf("\nWord found!\n");
        printf("Printing the search path: \n");

        showPath(arr_out);
    }
    else{
        printf("\nWord not found!\n");
    }
}

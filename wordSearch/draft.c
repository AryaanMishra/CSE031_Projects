#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
void toUpper(char* word);
void showPath(int** arr_out);
void showChop(int arr_out_num);
int puzzleSize;
int wordSize = 0;
void deletePath(int** arr_out, int firstI, int firstJ);
bool findLetter(char** arr, char* word, int count, int i, int j, int** arr_out, bool first);
bool findWord(char** arr, char* word, int** arr_out);
bool checkBounds(int i, int j);  // Function to check if indices are within bounds
bool checkLetter(char** arr, char* word, int count, int i, int j);  // Function to check if a letter matches
void updatePath(int** arr_out, int count, int i, int j);  // Function to update the path
//void resetPath(int** arr_out, int i, int j);  // Function to reset the path

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
    fscanf(fptr, "%d\n", &puzzleSize);

    char **block = (char **)malloc(puzzleSize * sizeof(char *));
    char *word = (char *)malloc(20 * sizeof(char));

    for (i = 0; i < puzzleSize; i++){
        *(block + i) = (char *)malloc(puzzleSize * sizeof(char));
        for (j = 0; j < puzzleSize - 1; ++j){
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

void printPuzzle(char** arr){                                   
    for (int i = 0; i < puzzleSize; i++){
        for (int j = 0; j < puzzleSize; j++){
            printf("%c ", *(*(arr + i) + j));
        }
        printf("\n");
    }
}

void toUpper(char* word){
    for(int i = 0; i < strlen(word); i++){
        if(*(word + i) >= 'a' && *(word + i) <= 'z'){
            *(word + i) = *(word + i) - 32;
        }
    }
}
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

void showPath(int** arr_out){
    for (int i = 0; i < puzzleSize; i++){
        for (int j = 0; j < puzzleSize; j++){
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

void deletePath(int** arr_out, int starti, int startj){
    for (int i = 0; i < puzzleSize; i++){
        for (int j = 0; j < puzzleSize; j++){
            if (starti != i || startj != j){
                *(*(arr_out + i) + j) = 0;     
            }
        }
    }
}

bool checkBounds(int i, int j){
    int wordIndex = puzzleSize - 1;
    return !(i > wordIndex || j > wordIndex);
}

bool checkLetter(char** arr, char* word, int count, int i, int j){
    return *(word + count) == *(*(arr + i) + j);
}

void updatePath(int** arr_out, int count, int i, int j){
    *(*(arr_out + i) + j) = *(*(arr_out + i) + j) * 10 + count + 1;
}


bool findLetter(char** arr, char* word, int count, int i, int j, int** arr_out, bool first){
    if (!checkBounds(i, j) || !checkLetter(arr, word, count, i, j)){
        return false;
    }

    if (strlen(word) == count + 1){
        updatePath(arr_out, count, i, j);
        return true;
    }

    updatePath(arr_out, count, i, j);

    // Recursive calls to find the letter in all directions
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

    if (i > 0 && j < puzzleSize - 1){
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
    
    if (j < puzzleSize - 1){
        if (findLetter(arr, word, count + 1, i, j + 1, arr_out, false)){
            return true;
        }
        if (first){
            deletePath(arr_out, i, j);
        }
    }    

    if (j > 0 && i < puzzleSize - 1){
        if (findLetter(arr, word, count + 1, i + 1, j - 1, arr_out, false)){
            return true;
        }
        if (first){
            deletePath(arr_out, i, j);
        }
    }

    if (i < puzzleSize - 1){
        if (findLetter(arr, word, count + 1, i + 1, j, arr_out, false)){
            return true;
        }
        if (first){
            deletePath(arr_out, i, j);
        }
    }

    if (j < puzzleSize - 1 && i < puzzleSize - 1){
        if (findLetter(arr, word, count + 1, i + 1, j + 1, arr_out, false)){
            return true;
        }
        if (first){
            deletePath(arr_out, i, j);
        }
    }

    return false;   
}


bool findWord(char** arr, char* word, int** arr_out){
    for (int i = 0; i < puzzleSize; i++){
        for (int j = 0; j < puzzleSize; j++){
            if (findLetter(arr, word, 0, i, j, arr_out, true) == true){
                return true;
            }
            for (int i = 0; i < puzzleSize; i++){
                for (int j = 0; j < puzzleSize; j++){
                    *(*(arr_out + i) + j) = 0;      
                }
            }
        }
    }

    return false;
}

void searchPuzzle(char** arr, char* word){
    toUpper(word);

    int** arr_out = (int**)malloc(puzzleSize * sizeof(int*));
    for (int i = 0; i < puzzleSize; i++){
        *(arr_out + i) = (int *)malloc(puzzleSize * sizeof(int));
        for (int j = 0; j < puzzleSize; j++){
            *(*(arr_out + i) + j) = 0;
        }
    }

    if (findWord(arr, word, arr_out) == true){
        printf("\nWord found!\n");
        printf("Printing the search path: \n");

        showPath(arr_out);
    }
    else{
        printf("\nWord not found!\n");
    }
}

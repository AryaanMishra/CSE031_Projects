#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//PARTNERS: ELLIS HAMPTON, JULIA WU

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
int searchNeighbors(char** arr, int wordIndex, int x, int y, char* word, int** path);

//smaller helper functions
void fixCase(char* word);
int isValidIndex(int x, int y);
void setPathValue(int** path, int value, int x, int y);

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
        for(int j = 0; j < bSize; j++){
            printf("%c ", *(*(arr + i) + j));
        }
        printf("\n");
    }
}

void searchPuzzle(char** arr, char* word) {
    // This function checks if arr contains the search word. If the 
    // word appears in arr, it will print out a message and the path 
    // as shown in the sample runs. If not found, it will print a 
    // different message as shown in the sample runs.
    // Your implementation here...

    int foundWord = 0; //boolean - if word is found, set to 1

    fixCase(word); //make word all caps

    //intialize the 2D array that will hold our search path
    int **path = (int**)malloc(bSize * sizeof(int*));

    for(int i = 0; i < bSize; i++) {
        *(path + i) = (int*)malloc(bSize * sizeof(int));
        for (int j = 0; j < bSize; ++j) {
            *(*(path + i) + j) = 0;
        }
    }

    //BEGIN SEARCH!!
    for(int i = 0; i < bSize; i++){
        for(int j = 0; j < bSize; j++){
            if( *(*(arr + i) + j) == *word){ //if we find the first letter of word in the array
                if(searchNeighbors(arr, 1, i, j, word, path)){
                    foundWord = 1; //word found!
                }       
            }
        }
    }
    //END SEARCH!!


    if(foundWord){
        printf("\nWord found!\nPrinting the search path:\n");
        
        //PRINT SEARCH PATH!!
        for(int i = 0; i < bSize; i++){
            for(int j = 0; j < bSize; j++){
                printf("%-8d", *(*(path + i) + j)); //%-8d means it is left aligned with 8 spaces (including the numbers themselves)
            }
            printf("\n");
        }
    } else {
        printf("\nWord not found!\n");
    }
}

int searchNeighbors(char** arr, int wordIndex, int x, int y, char* word, int** path){   //wordIndex is the index of the current letter of the word we're on
                                                                                        //x and y are coords,
                                                                                        //path is the array that holds our search path

    if(wordIndex >= strlen(word)){              //eventually searchNeighbors will pass the final index of the word. if this happens,
        setPathValue(path, wordIndex, x, y);    //that means we found the full word! so we return 1 and end the recursive loop (after setting the correct value in the path array)
        return 1;                  
    }

    for(int i = x-1; i <= x+1; i++){        //search in a 3x3 grid around (x, y) for neighbors
        for(int j = y-1; j <= y+1; j++){
            if( isValidIndex(i, j) && !(i == x && j == y) ){    //make sure index is valid and (i, j) =/= (x, y) so it doesn't check the original letter, only the ones around it

                if( *(*(arr + i) + j) == *(word + wordIndex)){  //check if (i, j) is the specific letter we're looking for

                    if(searchNeighbors(arr, wordIndex + 1, i, j, word, path)){ //if so, search all of that letter's neighbors (and so on, recursively)
                    //if this returns true ^, then we passed the final index of the word (which means we found the full word)
                        setPathValue(path, wordIndex, x, y);           
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

//smaller helper functions

void fixCase(char* word){ //make word all caps
    for(int i = 0; i < strlen(word); i++){
        if(*(word + i) >= 'a' && *(word + i) <= 'z'){
            *(word + i) += ('A' - 'a');
        }
    }
}

int isValidIndex(int x, int y){ //make sure index isn't out of bounds
    if( (x < 0 || x >= bSize) || (y < 0 || y >= bSize) ){
        return 0;
    }
    return 1;
}

void setPathValue(int** path, int value, int x, int y){ //set proper value in the search path array at (x, y)

    if( *(*(path + x) + y) != 0){ //if there's already a value at (x, y) - this means we have to do some math to add our value to it

        *(*(path + x) + y) *= 10;
        // ^^ why are we doing this?
        //Because if the original path value is 5 and we are trying to add a value of 3, we need the original path value to be 50 so when we add our value to it we end up with 53
        if(value >= 10){
            *(*(path + x) + y) *= 10;
        }//Just in case someone puts in a word that's double digits in length.
         //I could make this more general - being able to handle a word at any character length ( >= 100, >= 1000, etc )
         //but I don't think it's worth it - who would ever type a word that's 100 characters long?? (Also the integer limit would be reached long before then)
    }

    *(*(path + x) + y) += value;
}
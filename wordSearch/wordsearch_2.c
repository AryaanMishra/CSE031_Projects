//TO RUN:
//gcc wordsearch_1.c -o jer
//.\jer.exe puzzle1.txt


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables

struct position{
    int row;
    int column;
};

struct node{
    int wordindex;
    int row;
    int column;
    struct node *next;
};

struct node *head = NULL;

void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
int search(char** arr, int row, int column, int curr, char* word, bool* completePath);
int validPos(int row, int column);
int bSize;
void push(int wordindex,int row, int column);
void pop();


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
    for(int i =0; i < bSize; i++){ 
        for(int j = 0; j < bSize; j++){ 
            printf("%c ", *(*(arr+i)+j));
        }
        printf("\n"); 
    }
}

void uppercase(char* word){
    int length = strlen(word);
    for (int i = 0; i < length; i++){
        if(*(word+i) >= 'a' && *(word+i) <= 'z'){
            *(word+i) -= 32;
        }
    }
}

void searchPuzzle(char** arr, char* word){
    uppercase(word);
    int found = 0;
    int temp = 0; 
    int start = 1;

    for(int i = 0; i < bSize; i++){
        for(int j = 0; j < bSize; j++){
            int row = i;
            int column = j;
            if(*(word) == *(*(arr + i) + j)){
                if(strlen(word) > 1){
                    push(start, i, j);
                    temp = search(arr, row, column, start, word);
                    found += temp;
                    if(temp == 0){
                        pop();
                    }
                }else{
                    found += 1;
                }
            }

        }

    }
    if(found == 0){
        printf(("\nWord not found!\n"));
    }else{
        printf("\nWord found!\n");
        printf("\nPrinting the search path\n");
        for(int i =0; i < bSize; i++){
            for(int j = 0; j < bSize; j++){
                struct node* current = head;
                int counter = 0;
                while(current != NULL){
                    if(i == current -> row && j == current -> column){
                        printf("%d", current -> wordindex);
                        counter++;
                    }
                    current = current -> next;
                }
                if(counter == 0){
                    printf("0");
                    counter++;
                }
                for(int k = 0; k < 8-counter; k++){
                    printf(" ");
                }
            }
            printf("\n");
        }

    }

}

int validPos(int row, int column){
    if(row < 0 || row >= bSize || column < 0 || column >= bSize){
        return 0;
    }
    return 1;
}

void push(int wordindex, int row, int column){
    struct node* temp = malloc(sizeof(struct node));
    temp -> wordindex = wordindex;
    temp -> row = row;
    temp -> column = column;
    temp -> next = head;
    head = temp;
}

void pop(){
    if(head != NULL){
        struct node *temp;
        temp = head;
        head = head -> next;
        free(temp);
    }
}

int search(char** arr, int row, int column, int curr, char* word, bool* completePath) {
  int found = 0;
  *completePath = false; // Initialize completePath flag to false

  if (*(word + curr) == '\0') {
    return 1; // Found the entire word
  }

  // Check surrounding cells in a specific order (prioritize down, right, up, left)
  if (validPos(row - 1, column) == 1 && *(word + curr) == *(*(arr + row - 1) + column)) {
    push(curr + 1, row - 1, column);
    found = search(arr, row - 1, column, curr + 1, word, completePath);
    if (found == 1) {
      return found; // Found the entire word on this path, terminate further exploration
    }
    pop();
  }

  if (validPos(row, column + 1) == 1 && *(word + curr) == *(*(arr + row) + column + 1)) {
    push(curr + 1, row, column + 1);
    found = search(arr, row, column + 1, curr + 1, word);
    if (found == 1) {
      return found; // Found the entire word on this path, terminate further exploration
    }
    pop();
  }

  if (validPos(row + 1, column) == 1 && *(word + curr) == *(*(arr + row + 1) + column)) {
    push(curr + 1, row + 1, column);
    found = search(arr, row + 1, column, curr + 1, word);
    if (found == 1) {
      return found; // Found the entire word on this path, terminate further exploration
    }
    pop();
  }

  if (validPos(row, column - 1) == 1 && *(word + curr) == *(*(arr + row) + column - 1)) {
    push(curr + 1, row, column - 1);
    found = search(arr, row, column - 1, curr + 1, word);
    if (found == 1) {
      return found; // Found the entire word on this path, terminate further exploration
    }
    pop();
  }

  return found; // No match found in surrounding cells
}


// int search(char** arr, int row, int column, int curr, char* word) {
//   int found = 0;
//   int tempresult = 0;
//   if (*(word + curr) == '\0') {
//     return 1;
//   }

//   if (validPos(row - 1, column - 1) == 1) {
//     if (*(word + curr) == *(*(arr + row - 1) + column - 1)) {
//       push(curr + 1, row - 1, column - 1);
//       tempresult = search(arr, row - 1, column - 1, curr + 1, word);
//       found += tempresult;
//       // Only pop if the recursive call doesn't find the remaining characters
//       if (tempresult != 1) {
//         pop();
//       }
//     }
//   }

//   if (validPos(row - 1, column) == 1) {
//     if (*(word + curr) == *(*(arr + row - 1) + column)) {
//       push(curr + 1, row - 1, column);
//       tempresult = search(arr, row - 1, column, curr + 1, word);
//       found += tempresult;
//       if (tempresult != 1) {
//         pop();
//       }
//     }
//   }

//   if (validPos(row - 1, column + 1) == 1) {
//     if (*(word + curr) == *(*(arr + row - 1) + column + 1)) {
//       push(curr + 1, row - 1, column + 1);
//       tempresult = search(arr, row - 1, column + 1, curr + 1, word);
//       found += tempresult;
//       if (tempresult != 1) {
//         pop();
//       }
//     }
//   }

//   if (validPos(row, column - 1) == 1) {
//     if (*(word + curr) == *(*(arr + row) + column - 1)) {
//       push(curr + 1, row, column - 1);
//       tempresult = search(arr, row, column - 1, curr + 1, word);
//       found += tempresult;
//       if (tempresult != 1) {
//         pop();
//       }
//     }
//   }

//   if (validPos(row, column + 1) == 1) {
//     if (*(word + curr) == *(*(arr + row) + column + 1)) {
//       push(curr + 1, row, column + 1);
//       tempresult = search(arr, row, column + 1, curr + 1, word);
//       found += tempresult;
//       if (tempresult != 1) {
//         pop();
//       }
//     }
//   }

//   if (validPos(row + 1, column - 1) == 1) {
//     if (*(word + curr) == *(*(arr + row + 1) + column - 1)) {
//       push(curr + 1, row + 1, column - 1);
//       tempresult = search(arr, row + 1, column - 1, curr + 1, word);
//       found += tempresult;
//       if (tempresult != 1) {
//         pop();
//       }
//     }
//   }

//   if (validPos(row + 1, column) == 1) {
//     if (*(word + curr) == *(*(arr + row + 1) + column)) {
//       push(curr + 1, row + 1, column);
//       tempresult = search(arr, row + 1, column, curr + 1, word);
//       found += tempresult;
//       if (tempresult != 1) {
//         pop();
//       }
//     }
//   }

//   if (validPos(row + 1, column + 1) == 1) {
//     if (*(word + curr) == *(*(arr + row + 1) + column + 1)) {
//         push(curr + 1, row + 1, column + 1);
//         tempresult = search(arr, row + 1, column + 1, curr + 1, word);
//         found += tempresult;
//         if (tempresult != 1) {
//             pop();
//         }
//         }
//     }
//   return found;
// }


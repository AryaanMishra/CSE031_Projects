#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
int bSize;
int explorePaths(char** arr, char* word, int row, int col, int index, int* isFound, int* pathCount, int* pathIndices);
int pathIndices[1000];


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

void printPath(char** arr, int gridSize, int* pathIndices);

void printPuzzle(char** arr) {
  // Iterate through each row and column of the grid
  for (int i = 0; i < bSize; i++) {
    for (int j = 0; j < bSize; j++) {
      printf("%c ", arr[i][j]);
    }
    printf("\n");
  }
}

void searchPuzzle(char** arr, char* word) {
    int wordLen = strlen(word);
    int isFound = 0; // Flag to track if word is found
    int pathCount = 0; // Count valid paths found

    // Iterate through each cell of the grid
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            // Check if current cell matches the first letter (case-insensitive)
            if (tolower(arr[i][j]) == tolower(word[0])) {
                // Mark current cell as visited to avoid revisiting in backtracking
                arr[i][j] = '#';

                // Explore paths recursively from the current cell
                if (explorePaths(arr, word, i, j, 1, &isFound, &pathCount, 0)) {
                    // If word is found and we haven't exceeded max paths, print the path
                    if (pathCount <= 1) { // Limit to 1 path (basic requirement)
                        printf("Word found!\n");
                        printf("Printing the search path:\n");
                        printPath(arr, bSize, pathIndices); // Print path using helper
                    } else {
                        printf("More than 1 path found (bonus feature).\n");
                    }
                    pathCount = 0; // Reset path count for next iterations
                    isFound = 0; // Reset flag for next iterations
                }

                // Reset the visited cell
                arr[i][j] = word[0]; // Restore original character
            }
        }
    }

    // Final check and message if word not found
    if (!isFound) {
        printf("Word not found\n");
    }
}

// Helper function to explore paths recursively from a given cell
int explorePaths(char** arr, char* word, int row, int col, int index, int* isFound, int* pathCount, int* pathIndices) {
    // Base case: all characters of the word are found, set flag and return
    if (index == strlen(word)) {
        *isFound = 1;
        (*pathCount)++; // Increment path count

        // If limited to 1 path, store indices
        if (*pathCount <= 1) {
            pathIndices[index - 1] = row * bSize + col;
        }

        return 1;
    }

    // Check all 8 adjacent cells (up, down, left, right, diagonals)
    int directions[8][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    for (int i = 0; i < 8; i++) {
        int newRow = row + directions[i][0];
        int newCol = col + directions[i][1];

        // Check if new cell is within grid bounds and matches next character (case-insensitive)
        if (newRow >= 0 && newRow < bSize && newCol >= 0 && newCol < bSize &&
            tolower(arr[newRow][newCol]) == tolower(word[index])) {
            // Mark visited cell
            arr[newRow][newCol] = '#';

            // Explore path recursively from the new cell
            if (explorePaths(arr, word, newRow, newCol, index + 1, isFound, pathCount, pathIndices)) {
                // Backtrack: reset visited cell and return
                arr[newRow][newCol] = word[index - 1];
                return 1;
            }

            // Unmark visited cell if path not found
            arr[newRow][newCol] = word[index];
        }
    }

    return 0; // No path found from current cell
}

// Helper function to print the path based on stored indices
void printPath(char** arr, int gridSize, int* pathIndices) {
    int pathLen = sizeof(pathIndices) / sizeof(pathIndices[0]); // Assuming pathIndices is an array

    // Iterate through stored path indices
    for (int i = 0; i < pathLen; i++) {
        int index = pathIndices[i];
        int row = index / gridSize;
        int col = index % gridSize;

        // Convert row and column indices to human-readable coordinates
        int x = row + 1; // Adjust for 1-based indexing
        int y = col + 1;

        // Print coordinates with appropriate spacing
        printf("%d %d ", x, y);
    }

    printf("\n"); // Add newline at the end
}
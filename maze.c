#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h> // For _getch() function on Windows, use <curses.h> on Unix systems

#define MAX_HEIGHT 100
#define MAX_WIDTH 100

// Define maze characters
#define WALL '#'
#define PATH ' '
#define START 'S'
#define EXIT 'E'

// Define player character
#define PLAYER '@'

// Define movement keys
#define KEY_UP 'w'
#define KEY_DOWN 's'
#define KEY_LEFT 'a'
#define KEY_RIGHT 'd'
#define KEY_QUIT 'q'

// Define maze dimensions
int height, width;

// Maze array
char maze[MAX_HEIGHT][MAX_WIDTH];

// Player position
int playerRow, playerCol;

// Function prototypes
void loadMazeFromFile(char *filename);
void displayMaze();
bool isValidMove(int newRow, int newCol);
void movePlayer(char direction);

int main() {
    char filename[] = "maze.txt"; // Example filename
    loadMazeFromFile(filename);

    // Find player position
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (maze[i][j] == START) {
                playerRow = i;
                playerCol = j;
                break;
            }
        }
    }

    char key;
    do {
        system("cls"); // Clear screen (for Windows)
        displayMaze();

        // Get user input
        key = _getch(); // Use _getch() for Windows, or getch() for Unix systems

        // Move player
        switch (key) {
            case KEY_UP:
                movePlayer('U');
                break;
            case KEY_DOWN:
                movePlayer('D');
                break;
            case KEY_LEFT:
                movePlayer('L');
                break;
            case KEY_RIGHT:
                movePlayer('R');
                break;
            case KEY_QUIT:
                printf("\nQuitting game.\n");
                break;
            default:
                printf("\nInvalid input. Use 'w', 'a', 's', 'd' to move, or 'q' to quit.\n");
        }
    } while (key != KEY_QUIT);

    return 0;
}

void loadMazeFromFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%d %d", &height, &width) != 2) {
        fprintf(stderr, "Invalid maze dimensions.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Read maze into array
    for (int i = 0; i < height; i++) {
        if (fscanf(file, "%s", maze[i]) == EOF) {
            fprintf(stderr, "Error reading maze.\n");
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
}

void displayMaze() {
    // Display the maze
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == playerRow && j == playerCol) {
                printf("%c", PLAYER);
            } else {
                printf("%c", maze[i][j]);
            }
        }
        printf("\n");
    }
}

bool isValidMove(int newRow, int newCol) {
    if (newRow >= 0 && newRow < height && newCol >= 0 && newCol < width && maze[newRow][newCol] != WALL) {
        return true;
    }
    return false;
}

void movePlayer(char direction) {
    int newRow = playerRow;
    int newCol = playerCol;

    switch (direction) {
        case 'U':
            newRow--;
            break;
        case 'D':
            newRow++;
            break;
        case 'L':
            newCol--;
            break;
        case 'R':
            newCol++;
            break;
    }

    if (isValidMove(newRow, newCol)) {
        playerRow = newRow;
        playerCol = newCol;

        if (maze[playerRow][playerCol] == EXIT) {
            printf("\nCongratulations! You've reached the exit!\n");
            exit(EXIT_SUCCESS);
        }
    }
}

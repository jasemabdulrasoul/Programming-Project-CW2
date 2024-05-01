#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

// Maze array
char maze[MAX_HEIGHT][MAX_WIDTH + 1]; // +1 for null-terminator

// Maze dimensions
int height = 0, width = 0;

// Player position
int playerRow = -1, playerCol = -1;

// Function prototypes
bool loadMazeFromFile(char *filename);
void displayMaze();
bool isValidMove(int newRow, int newCol);
void movePlayer(char direction);

int main(int argc, char *argv[]) {
    if (argc != 2) { // Expecting one argument (the maze file path)
        fprintf(stderr, "Usage: %s <maze file path>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    if (!loadMazeFromFile(filename)) {
        return 1; // Failed to load the maze
    }

    // Find player start position
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (maze[i][j] == START) {
                playerRow = i;
                playerCol = j;
                break;
            }
        }
    }

    if (playerRow == -1 || playerCol == -1) {
        fprintf(stderr, "Start position not found.\n");
        return 1;
    }

    char key;
    do {
        system("cls");
        displayMaze();

        printf("Enter a movement key ('w', 's', 'a', 'd') or 'q' to quit: ");
        scanf(" %c", &key); // Space before %c to skip leading whitespace

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
                printf("Invalid input. Use 'w', 'a', 's', 'd' to move, or 'q' to quit.\n");
        }
    } while (key != KEY_QUIT);

    return 0;
}

bool loadMazeFromFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return false;
    }

    char line[MAX_WIDTH + 2]; // +2 for newline and null terminator
    height = 0;

    while (fgets(line, sizeof(line), file) != NULL && height < MAX_HEIGHT) {
        // Remove newline at the end
        size_t len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Store the row in the maze array
        strcpy(maze[height], line);
        height++;
    }

    fclose(file);

    // Determine the maze's width based on the first row
    width = strlen(maze[0]);

    return true;
}

void displayMaze() {
    // Display the maze, showing the player's current position
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
    return newRow >= 0 && newRow < height && newCol >= 0 && newCol < width && maze[newRow][newCol] != WALL;
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
    } else {
        printf("Invalid move! Hit a wall or went out of bounds.\n");
    }
}

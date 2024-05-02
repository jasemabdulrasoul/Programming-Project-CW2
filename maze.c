#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_HEIGHT 100
#define MAX_WIDTH 100
#define MIN_HEIGHT 5
#define MIN_WIDTH 5
#define MAX_DIMENSION 100
#define MIN_DIMENSION 5

// Define maze characters.
#define WALL '#'
#define PATH ' '
#define START 'S'
#define EXIT 'E'

// Define player character.
#define PLAYER 'X'

// Define movement keys.
#define KEY_UP 'w'
#define KEY_DOWN 's'
#define KEY_LEFT 'a'
#define KEY_RIGHT 'd'
#define KEY_QUIT 'q'

// Maze array.
char maze[MAX_HEIGHT][MAX_WIDTH + 1]; // +1 for null terminator.

// Maze dimensions.
int height = 0, width = 0;

// Player position.
int playerRow = -1, playerCol = -1;

bool loadMazeFromFile(char *filename);
bool validateMaze();
void displayMaze();
bool isValidMove(int newRow, int newCol);
void movePlayer(char direction);

int main(int argc, char *argv[]) {
    if (argc != 2) { // Should have one argument.
        fprintf(stderr, "Usage: %s <maze file path>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    if (!loadMazeFromFile(filename)) {
        return 2;
    }


    if (!validateMaze()) {
        return 3;
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
        return 100;
    }

    char key;
    displayMaze();
    do {

        printf("Enter a movement key ('w', 's', 'a', 'd') or 'm' for map or 'q' to quit: ");
        scanf(" %c", &key); // Space before %c to skip leading whitespace.

        switch (key) { // Switch for movement depending on input.
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
            case 'm':
            case 'M':
                displayMaze();
                break;
            default:
                printf("Invalid input. Use 'w', 'a', 's', 'd' to move, 'm' for map, or 'q' to quit.\n");
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

    height = 0;
    char line[MAX_WIDTH + 2]; // +2 for newline and null terminator.

    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove newline at the end.
        size_t len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Check if height exceeds limit.
        if (height >= MAX_HEIGHT) {
            fprintf(stderr, "Maze height exceeds the maximum limit of %d.\n", MAX_HEIGHT);
            fclose(file);
            return false;
        }

        strcpy(maze[height], line); // Store row in maze array.
        height++;
    }

    fclose(file);

    if (height < MIN_HEIGHT) {
        fprintf(stderr, "Maze height is below the minimum limit of %d.\n", MIN_HEIGHT);
        return false;
    }

    width = strlen(maze[0]);

    if (width < MIN_WIDTH || width > MAX_WIDTH) {
        fprintf(stderr, "Maze width must be between %d and %d.\n", MIN_WIDTH, MAX_WIDTH);
        return false;
    }

    return true;
}

bool validateMaze() {
    // Make sure that each row is of the same width.
    for (int i = 0; i < height; i++) {
        if (strlen(maze[i]) != width) {
            fprintf(stderr, "Inconsistent height or width.\n");
            return false;
        }
    }

    // Check for invalid characters.
    for (int j = 0; j < width; j++) {
        int columnHeight = 0;

        for (int i = 0; i < height; i++) {
            char c = maze[i][j];
            if (c != WALL && c != PATH && c != START && c != EXIT) {
                fprintf(stderr, "Invalid character found in maze.\n");
                return false;
            }

            if (c != '\0') {
                columnHeight++;
            }
        }

        if (columnHeight != height) { // Make sure that every column is of the same height.
            fprintf(stderr, "Inconsistent height or width.\n");
            return false;
        }
    }

    return true;
}

void displayMaze() {
    // Display the maze showing the player's current position.
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

bool isValidMove(int newRow, int newCol) { // Check if the move is valid.
    return newRow >= 0 && newRow < height && newCol >= 0 && newCol < width && maze[newRow][newCol] != WALL;
}

void movePlayer(char direction) {
    int newRow = playerRow;
    int newCol = playerCol;

    switch (direction) { // Switch to move the player depending on the direction that we got from the input.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defines for max and min permitted dimensions
#define MAX_DIM 100
#define MIN_DIM 5

// Defines for the required autograder exit codes
#define EXIT_SUCCESS 0
#define EXIT_ARG_ERROR 1
#define EXIT_FILE_ERROR 2
#define EXIT_MAZE_ERROR 3

typedef struct __Coord {
    int x;
    int y;
} coord;

typedef struct __Maze {
    char **map;
    int height;
    int width;
    coord start;
    coord end;
} maze;

/**
 * @brief Initialise a maze object - allocate memory and set attributes
 *
 * @param this pointer to the maze to be initialised
 * @param height height to allocate
 * @param width width to allocate
 * @return int 0 on success, 1 on fail
 */
int create_maze(maze *this, int height, int width) {
    this->height = height;
    this->width = width;

    this->map = (char **)malloc(sizeof(char *) * height);
    if (this->map == NULL) {
        return 1;
    }

    for (int i = 0; i < height; i++) {
        this->map[i] = (char *)malloc(sizeof(char) * width);
        if (this->map[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(this->map[j]);
            }
            free(this->map);
            return 1;
        }
    }

    return 0;
}

/**
 * @brief Free the memory allocated to the maze struct
 *
 * @param this the pointer to the struct to free
 */
void free_maze(maze *this) {
    for (int i = 0; i < this->height; i++) {
        free(this->map[i]);
    }
    free(this->map);
}

/**
 * @brief Validate and return the width of the mazefile
 *
 * @param file the file pointer to check
 * @return int a valid width (5-100) or 0 for error
 */
int get_width(FILE *file) {
    int width;
    if (fscanf(file, "%d", &width) != 1) {
        return 0;
    }
    if (width < MIN_DIM || width > MAX_DIM) {
        return 0;
    }
    return width;
}

/**
 * @brief Validate and return the height of the mazefile
 *
 * @param file the file pointer to check
 * @return int a valid height (5-100) or 0 for error
 */
int get_height(FILE *file) {
    int height;
    if (fscanf(file, "%d", &height) != 1) {
        return 0;
    }
    if (height < MIN_DIM || height > MAX_DIM) {
        return 0;
    }
    return height;
}

/**
 * @brief read in a maze file into a struct
 *
 * @param this Maze struct to be used
 * @param file Maze file pointer
 * @return int 0 on success, 1 on fail
 */
int read_maze(maze *this, FILE *file) {
    int height = get_height(file);
    int width = get_width(file);

    if (height == 0 || width == 0) {
        return 1;
    }

    if (create_maze(this, height, width) != 0) {
        return 1;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            char c;
            if (fscanf(file, " %c", &c) != 1) {
                free_maze(this);
                return 1;
            }
            if (c == 'S') {
                this->start.x = j;
                this->start.y = i;
            } else if (c == 'E') {
                this->end.x = j;
                this->end.y = i;
            }
            this->map[i][j] = c;
        }
    }

    return 0;
}

/**
 * @brief Validates and performs a movement in a given direction
 *
 * @param this Maze struct
 * @param player The player's current position
 * @param direction The desired direction to move in
 */
void move(maze *this, coord *player, char direction) {
    int new_x = player->x;
    int new_y = player->y;

    if (direction == 'w') {
        new_y--;
    } else if (direction == 's') {
        new_y++;
    } else if (direction == 'a') {
        new_x--;
    } else if (direction == 'd') {
        new_x++;
    } else {
        return; // Invalid direction
    }

    // Check if the new position is within bounds and is not a wall
    if (new_x >= 0 && new_x < this->width && new_y >= 0 && new_y < this->height) {
        if (this->map[new_y][new_x] != '#') {
            player->x = new_x;
            player->y = new_y;
        }
    }
}

/**
 * @brief Check whether the player has won and return a pseudo-boolean
 *
 * @param this current maze
 * @param player player position
 * @return int 0 for false, 1 for true
 */
int has_won(maze *this, coord *player) {
    return (player->x == this->end.x && player->y == this->end.y);
}

/**
 * @brief Prints the maze out - code provided to ensure correct formatting
 *
 * @param this pointer to maze to print
 * @param player the current player location
 */
void print_maze(maze *this, coord *player) {
    // Make sure we have a leading newline
    printf("\n");
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (player->x == j && player->y == i) {
                printf("X"); // Player's current position
            } else {
                printf("%c", this->map[i][j]);
            }
        }
        printf("\n");
    }
}

/**
 * @brief Maze game loop
 */
void game_loop(maze *this_maze, coord *player) {
    char direction;

    while (1) {
        print_maze(this_maze, player);

        // Read a direction from standard input
        printf("Move (w/a/s/d to move, 'q' to quit): ");
        if (scanf(" %c", &direction) != 1 || direction == 'q') {
            printf("Exiting game.\n");
            break;
        }

        // Move player and check for win
        move(this_maze, player, direction);
        if (has_won(this_maze, player)) {
            printf("Congratulations! You've reached the end of the maze!\n");
            break;
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <maze_file>\n", argv[0]);
        return EXIT_ARG_ERROR;
    }

    maze this_maze;
    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("Error opening file: %s\n", argv[1]);
        return EXIT_FILE_ERROR;
    }

    if (read_maze(&this_maze, f) != 0) {
        printf("Error reading maze from file.\n");
        fclose(f);
        return EXIT_MAZE_ERROR;
    }

    fclose(f);

    coord player = this_maze.start;

    game_loop(&this_maze, &player);

    free_maze(&this_maze);

    return EXIT_SUCCESS;
}

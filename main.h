#ifndef MAIN_H
#define MAIN_H
#include <termios.h>

#define ROWS 15
#define COLS 20

// Define constants for keyboard inputs
#define LEFT   68
#define RIGHT  67
#define ROTATE 32
#define DOWN   66
#define EXIT   113
#define GAME_SPEED 600
#define PIECE_SIZE 4

// Structure to represent the current Tetris piece
typedef struct
{
  char x;
  char y;
  char piece[PIECE_SIZE][PIECE_SIZE];
}Current;

struct termios saved_attributes;

// Tetris piece patterns
char piece[7][PIECE_SIZE][PIECE_SIZE] = {{{1, 1, 0, 0},{1, 1, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0}}
                       ,{{1, 0, 0, 0},{1, 0, 0, 0},{1, 0, 0, 0},{1, 0, 0, 0}}
                       ,{{1, 1, 0, 0},{1, 0, 0, 0},{1, 0, 0, 0},{0, 0, 0, 0}}
                       ,{{0, 1, 1, 0},{0, 0, 1, 1},{0, 0, 0, 0},{0, 0, 0, 0}}
                       ,{{0, 1, 1, 0},{1, 1, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0}}
                       ,{{1, 1, 0, 0},{0, 1, 0, 0},{0, 1, 0, 0},{0, 0, 0, 0}}
                       ,{{0, 1, 0, 0},{1, 1, 1, 0},{0, 0, 0, 0},{0, 0, 0, 0}}};

// Game field representing the play area
char FIELD[ROWS][COLS];

// Function declarations
void afficher();
void fin();

#endif /* MAIN_H */

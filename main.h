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
#define PIECE_SIZE 4


#define LEVEL_UP_THRESHOLD 5
#define LEVEL_SPEED_INCREASE 5

// Structure to represent the current Tetris piece
typedef struct
{
  char x;
  char y;
  char piece[PIECE_SIZE][PIECE_SIZE];
}Current;

long long current_time_in_milliseconds();
#endif /* MAIN_H */

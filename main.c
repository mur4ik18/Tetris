/*
  Tetris Game in C

  This program implements a simple Tetris game in C using a terminal interface.

  Controls:
  - Left Arrow: Move the current piece left
  - Right Arrow: Move the current piece right
  - Down Arrow: Move the current piece down
  - Space Bar: Rotate the current piece
  - 'q': Quit the game

  Rules:
  - Complete rows to score points and clear the row
  - If the pieces reach the top of the screen, the game ends

  Author: Alex Kotov
  Date: January 7, 2024
*/
#include "main.h"
#include "terminal.h"
#include "display.h"
#include "movement.h"
#include "field.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


// Main function to run the Tetris game
int main(void)
{
  struct termios saved_attributes;

  // Game field representing the play area
  char FIELD[ROWS][COLS];

  // Tetris piece patterns
  char piece[7][PIECE_SIZE][PIECE_SIZE] = {
    {{1, 1, 0, 0},{1, 1, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0}},
    {{1, 0, 0, 0},{1, 0, 0, 0},{1, 0, 0, 0},{1, 0, 0, 0}},
    {{1, 1, 0, 0},{1, 0, 0, 0},{1, 0, 0, 0},{0, 0, 0, 0}},
    {{0, 1, 1, 0},{0, 0, 1, 1},{0, 0, 0, 0},{0, 0, 0, 0}},
    {{0, 1, 1, 0},{1, 1, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0}},
    {{1, 1, 0, 0},{0, 1, 0, 0},{0, 1, 0, 0},{0, 0, 0, 0}},
    {{0, 1, 0, 0},{1, 1, 1, 0},{0, 0, 0, 0},{0, 0, 0, 0}}
  };

  // Score variable
  int score = 0;

  // Seed the random number generator with the current time
  srand(time(NULL));

  // Variable used to exit from the game loop
  char option = 1;

  // Initialize game FIELD
  // Every element of the game FIELD is set to 0, but borders are set to 1
  game_field_init(FIELD);

  // Turn off CANONICAL MODE and clear terminal
  set_input_mode(saved_attributes);

  // Initialize current piece
  Current current_piece;
  current_piece = init_piece(piece, current_piece);

  // Variable to store the last time the piece was moved
  long long last_move_time = current_time_in_milliseconds();

  // Game loop
  while (option)
    {
      // Display the game field and current piece
      afficher(FIELD, current_piece, score);

      // Check collision and move the piece down
      if (current_time_in_milliseconds() - last_move_time >= GAME_SPEED) {
        if (check_collision(FIELD, current_piece) == 1)
          {
            for (int x = 0; x < 4; x++)
              {
                for (int y = 0; y < 4; y++)
                  {
                    if (current_piece.piece[x][y] == 1) 
                      FIELD[current_piece.x + x][current_piece.y + y] = 1;
                  }
              }
            current_piece = init_piece(piece, current_piece);
            for (int y = 1; y < COLS-1; y++)
              {
                if (FIELD[ROWS-1][y] == 1) {
                  game_over();
                }
              }
          }
        current_piece.x--;
        last_move_time = current_time_in_milliseconds();
      }

      // Handle user input and update game state
      if (handle_input(FIELD, &current_piece, &score))
        break;

      // Check and clear completed lines
      int linesCleared = check_line(FIELD);
      if (linesCleared > 0)
        {
          // Update the score
          score += linesCleared;
        }
    }

  printf("\e[?25h");  // Show the cursor
  return 1;
}


// Function to get the current time in milliseconds
long long current_time_in_milliseconds(void) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL);
}

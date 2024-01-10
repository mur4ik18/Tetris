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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>




// Function to initialize the game field.
void game_field_init(char FIELD[][COLS])
{
    // Iterate over each row in the game field.
    for (int i = 0; i < ROWS; i++)
    {
        // Iterate over each column in the game field.
        for (int j = 0; j < COLS; j++)
        {
            // Check if the current column is the leftmost column.
            if (j == 0)
              FIELD[i][j] = 1; // Set the leftmost column to 1, representing the left boundary.

            // Check if the current column is the rightmost column.
            else if (j == COLS - 1)
              FIELD[i][j] = 1; // Set the rightmost column to 1, representing the right boundary.

            // Check if the current row is not the topmost row.
            else if (i > 0)
              FIELD[i][j] = 0; // Set the inner cells to 0, indicating an empty space.

            // If the current row is the topmost row, set the cell to 1.
            else
              FIELD[i][j] = 1; // Set the topmost row to 1, representing the top boundary.
        }
    }
}


// Function to initialize a new Tetris piece
Current init_piece(char piece[][PIECE_SIZE][PIECE_SIZE], Current current_piece)
{
  current_piece.x = ROWS - 1;
  current_piece.y = COLS / 2;

  // Generate a random number between 0 and 6
  int randomNumber = rand() % 7;

  // Initialize the piece with the random piece
  for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
        {
          current_piece.piece[i][j] = piece[randomNumber][i][j];
        }
    }
  return current_piece;
}

// Function to check for completed lines and clear them
int check_line(char FIELD[][COLS])
{
  int linesCleared = 0;

  for (int x = ROWS - 1; x >= 1; x--)
    {
      int row = 0;
      for (int y = 1; y < COLS + 1; y++)
        {
          if (FIELD[x][y] == 1)
            row++;
        }

      if (row == COLS)
        {
          linesCleared++;

          // Clear the line
          for (int y = 1; y < COLS -1; y++)
            {
              FIELD[x][y] = 0;
            }

          // Move down the lines above the cleared line
          for (int i = x; ROWS > i; i++)
            {
              for (int j = 1; j < COLS - 1; j++)
                {
                  FIELD[i][j] = FIELD[i +  1][j];
                }
            }

          x++; // Consider the same row in the next iteration
        }
    }

  return linesCleared;
}


// Function to get the current time in milliseconds
long long current_time_in_milliseconds(void) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL);
}

// Main function to run the Tetris game
int main(void)
{
  struct termios saved_attributes;
  // Game field representing the play area
  char FIELD[ROWS][COLS];
  // Tetris piece patterns
  char piece[7][PIECE_SIZE][PIECE_SIZE] = {{{1, 1, 0, 0},{1, 1, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0}}
                                           ,{{1, 0, 0, 0},{1, 0, 0, 0},{1, 0, 0, 0},{1, 0, 0, 0}}
                                           ,{{1, 1, 0, 0},{1, 0, 0, 0},{1, 0, 0, 0},{0, 0, 0, 0}}
                                           ,{{0, 1, 1, 0},{0, 0, 1, 1},{0, 0, 0, 0},{0, 0, 0, 0}}
                                           ,{{0, 1, 1, 0},{1, 1, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0}}
                                           ,{{1, 1, 0, 0},{0, 1, 0, 0},{0, 1, 0, 0},{0, 0, 0, 0}}
                                           ,{{0, 1, 0, 0},{1, 1, 1, 0},{0, 0, 0, 0},{0, 0, 0, 0}}};
  // score variable
  int score = 0;
  // Seed the random number generator with the current time
  srand(time(NULL));

  // variable what I use for go out from game loop
  char option = 1;
  // Init game FIELD
  // Every element of GAME FIELD = 0
  // but borders = 1
  game_field_init(FIELD);
  // Turn off CANONICAL MODE
  // clear terminal
  set_input_mode(saved_attributes);
  // Current piece init
  Current current_piece;
  current_piece = init_piece(piece, current_piece);

  // Variable to store the last time the piece was moved
  long long last_move_time = current_time_in_milliseconds();

  // GAME loop
  while (option)
    {
      afficher(FIELD, current_piece, score);
      // ================= verification de la colision ================

      if (current_time_in_milliseconds() - last_move_time >= GAME_SPEED) {
        //usleep(GAME_SPEED*100);
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
            for (int y = 1; y<COLS-1; y++)
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

      int linesCleared = check_line(FIELD);
      if (linesCleared > 0)
        {
          // Update the score
          score += linesCleared;
        }
    }

  printf("\e[?25h");
  return 1;
}



#include "main.h"
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
  // Set the initial position of the new Tetris piece
  current_piece.x = ROWS - 1;         // Set the initial row (bottom of the game field)
  current_piece.y = COLS / 2;         // Set the initial column (center of the game field)

  // Generate a random number between 0 and 6 to select a random Tetris piece
  int randomNumber = rand() % 7;

  // Initialize the current piece with the randomly selected piece
  for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
        {
          // Copy the structure of the selected piece to the current piece
          current_piece.piece[i][j] = piece[randomNumber][i][j];
        }
    }

  // Return the initialized current piece
  return current_piece;
}


// Function to check for completed lines and clear them
int check_line(char FIELD[][COLS])
{
  int linesCleared = 0;

  // Iterate through each row from the bottom to the top
  for (int x = ROWS - 1; x >= 1; x--)
    {
      int row = 0;

      // Check each cell in the current row
      for (int y = 1; y < COLS + 1; y++)
        {
          // Count the filled cells in the current row
          if (FIELD[x][y] == 1)
            row++;
        }

      // If the entire row is filled, clear it
      if (row == COLS)
        {
          linesCleared++;

          // Clear the cells in the completed line
          for (int y = 1; y < COLS - 1; y++)
            {
              FIELD[x][y] = 0;
            }

          // Move down the lines above the cleared line
          for (int i = x; ROWS > i; i++)
            {
              for (int j = 1; j < COLS - 1; j++)
                {
                  // Shift the cells from the line above to the cleared line
                  FIELD[i][j] = FIELD[i +  1][j];
                }
            }

          x++; // Consider the same row in the next iteration
        }
    }

  return linesCleared; // Return the number of cleared lines
}


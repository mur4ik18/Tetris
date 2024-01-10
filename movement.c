#include "movement.h"
#include "main.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

// Function to check for collisions with the game field
int check_collision(char FIELD[][COLS], Current current_piece)
{
  // ======== bas ==========
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++)
      {
        //printf("%d %d = %d %d == %d %d\n", current_piece.x, current_piece.y, x, y, current_piece.x + x, current_piece.y + y);
        if ((current_piece.piece[x][y] == 1) && (FIELD[current_piece.x + x -1][current_piece.y +y] == 1))
          return 1;
      }
  }
  // ======== droit ========
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++)
      {
        //printf("%d %d = %d %d == %d %d\n", current_piece.x, current_piece.y, x, y, current_piece.x + x, current_piece.y + y);
        if ((current_piece.piece[x][y] == 1) && (FIELD[current_piece.x + x][current_piece.y +y + 1] == 1))
          return 3;
      }
  }
  // ======== gauche =======
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++)
      {
        //printf("%d %d = %d %d == %d %d\n", current_piece.x, current_piece.y, x, y, current_piece.x + x, current_piece.y + y);
        if ((current_piece.piece[x][y] == 1) && (FIELD[current_piece.x + x][current_piece.y + y - 1] == 1))
          return 2;
      }
  }
  return 0;
}


// Function to rotate the current Tetris piece
void rotate(Current *current_piece) {
    char temp[4][4];

    // Transpose the piece
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[i][j] = current_piece->piece[j][i];
        }
    }

    // Reverse each row to implement the rotation
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            char swap = temp[i][j];
            temp[i][j] = temp[i][3 - j];
            temp[i][3 - j] = swap;
        }
    }

    // Find the leftmost position of '1'
    int leftmostColumn = 3;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (temp[i][j] == 1) {
                leftmostColumn = (j < leftmostColumn) ? j : leftmostColumn;
            }
        }
    }

    // Shift the piece to the left
    for (int i = 0; i < 4; i++) {
        for (int j = leftmostColumn; j < 4; j++) {
            current_piece->piece[i][j - leftmostColumn] = temp[i][j];
        }
        for (int j = 4 - leftmostColumn; j < 4; j++) {
            current_piece->piece[i][j] = 0;  // Clear the remaining columns
        }
    }
}


// Function to check if a rotation is possible
int can_rotate(char FIELD[][COLS], Current current_piece)
{
  Current temp_piece = current_piece;

  // Perform a temporary rotation
  rotate(&temp_piece);

  // Check for collisions after the rotation
  if (check_collision(FIELD, temp_piece) != 1)
    {
      return 1;  // Rotation is possible
    }

  return 0;  // Rotation is not possible
}


// Function to move the piece down as fast as possible
void fastDown(char FIELD[][COLS], Current *current_piece) {
  while (check_collision(FIELD, *current_piece) != 1) {
    current_piece->x--;
  }
  //current_piece->x++;  // Adjust for the last valid position
}

// Function to handle user input and update the game state
int handle_input(char FIELD[][COLS], Current *current_piece, int *score) {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    // Use select to wait for input with a timeout of 0
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);

    // Check if there is input available
    if (FD_ISSET(STDIN_FILENO, &fds)) {
        char ch = getchar();
        // Check for user inputs and update game state accordingly
        if (ch == EXIT) {
            // Exit the game loop if the user presses the exit key
          return 1;
        } else if ((ch == 27) && (getchar() == 91)) {  // Check for escape sequences
            switch (getchar()) {
                case LEFT:  // Left arrow key
                  // Move the piece to the left if no collision is detected
                  if ((check_collision(FIELD, *current_piece) != 2) && (check_collision(FIELD, *current_piece) != 1))
                    current_piece->y--;
                  break;
                case RIGHT:  // Right arrow key
                  // Move the piece to the right if no collision is detected
                  if ((check_collision(FIELD, *current_piece) != 3) && (check_collision(FIELD, *current_piece) != 1))
                    current_piece->y++;
                  break;
                case DOWN:
                  // Move the piece down rapidly if allowed
                  fastDown(FIELD, current_piece);
                  break;
            }
        } else if (ch == ROTATE) {
            // Rotate the piece if rotation is allowed
            if (can_rotate(FIELD, *current_piece) ) {
                rotate(current_piece);
            }
        }
    }
    return 0;
}


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
  if (!check_collision(FIELD, temp_piece))
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
                  if ((check_collision(FIELD, *current_piece) != 2) && (check_collision(FIELD, *current_piece) != 1))
                    current_piece->y--;
                  break;
                case RIGHT:  // Right arrow key
                  if ((check_collision(FIELD, *current_piece) != 3) && (check_collision(FIELD, *current_piece) != 1))
                    current_piece->y++;
                  break;
            case DOWN:
              fastDown(FIELD, current_piece);
              break;
            }
        } else if (ch == ROTATE) {
            // Rotate the piece if rotation is allowed
          if (can_rotate(FIELD, *current_piece)) {
                rotate(current_piece);
            }
        }
    }
    return 0;
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
                  fin();
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


// Function to display the game field and current piece
void afficher(char FIELD[][COLS], Current current_piece, int score)
{
  // Print score
  printf("SCORE - %d\n", score);

  // =================== afficher le champ de jeux =========
  // a ameilleurer
  printf("\e[?25l");
 

  // print field
  for (size_t j=ROWS-2; j>0; j--){
    printf(":");
    for (size_t i=0; i<COLS; i++) {
      if (FIELD[j][i] == 1) {
        printf("#");
      }
      else if ((((current_piece.x + 3) >= j) && (j >= current_piece.x)) && (((current_piece.y + 3) >= i) && (i >= current_piece.y)))
        {
          if (current_piece.piece[j - current_piece.x][i - current_piece.y])
            printf("0");
          else
            printf(".");
        }
      else {
        printf(".");
      }
    }
    printf(":\n");
  }
  printf(":");
  for (size_t i =0; i<COLS; i++) {
    printf("-");
  }
  printf(":\n");
  // Move cursor back to top
  printf("\e[%iA", ROWS); 

}

// Function to display the game over screen
void fin(void)
{
  printf("\e[?25l");  // Hide the cursor

  // Clear the screen
  for (int x = 0; x < ROWS + 2; x++)
    {
      for (int y = 0; y < COLS + 2; y++)
        {
          printf(" ");
        }
      printf("\n");
    }

  // Move cursor to the center of the screen 
  printf("\e[%iA", ROWS + 2);
  printf("\e[%iC", COLS / 2 - 5);

  printf("GAME OVER!\n");

  // Wait for user input to exit
  printf("Press 'q' to quit...\n");
  char ch;
  while (1)
    {
      if (read(STDIN_FILENO, &ch, 1) > 0)
        {
          if (ch == 'q')
            {
              printf("\e[?25h");  // Show the cursor
              exit(EXIT_SUCCESS);
            }
          else
            printf("%d\n", ch);
        }
    }
}

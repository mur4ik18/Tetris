#include "display.h"
#include "main.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function to display the game field and current piece
void afficher(char FIELD[][COLS], Current current_piece, int score)
{
  // Print score
  printf("SCORE - %d\n", score);

  // =================== Display the game field ====================
  // Hide the cursor
  printf("\e[?25l");
 
  // Print field
  for (int j = ROWS - 2; j > 0; j--) {
    printf(":");
    for (int i = 0; i < COLS; i++) {
      if (FIELD[j][i] == 1) {
        printf("#");  // Display filled cell for game field boundary
      } else if ((((current_piece.x + 3) >= j) && (j >= current_piece.x)) &&
                 (((current_piece.y + 3) >= i) && (i >= current_piece.y))) {
        // Display the current piece
        if (current_piece.piece[j - current_piece.x][i - current_piece.y]) {
          printf("0");  // Display filled cell for the current piece
        } else {
          printf(".");  // Display empty cell for the current piece
        }
      } else {
        printf(".");  // Display empty cell for the game field
      }
    }
    printf(":\n");
  }
  printf(":");
  for (int i = 0; i < COLS; i++) {
    printf("-");  // Display boundary for the bottom of the game field
  }
  printf(":\n");
  // Move cursor back to the top
  printf("\e[%iA", ROWS); 

  // Show the cursor
  printf("\e[?25h");
}


// Function to display the game over screen
void game_over(void)
{
  printf("\e[?25l");  // Hide the cursor

  // Clear the screen
  for (int x = 0; x < ROWS + 2; x++)
    {
      for (int y = 0; y < COLS + 2; y++)
        {
          printf(" ");  // Print empty spaces to clear the screen
        }
      printf("\n");  // Move to the next line
    }

  // Move cursor to the center of the screen 
  printf("\e[%iA", ROWS + 2);  // Move the cursor up
  printf("\e[%iC", COLS / 2 - 5);  // Move the cursor to the center

  printf("GAME OVER!\n");  // Display the "GAME OVER!" message

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
              exit(EXIT_SUCCESS);  // Exit the program with success status
            }
          else
            printf("%d\n", ch);  // Print the ASCII value of the pressed key
        }
    }
}



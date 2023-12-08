#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#define ROWS 20
#define COLS 10

// Ici on declare notre champe de jeux, on va l'utiliser pour sauvegarder les positions de chaque piece.
char FIELD[ROWS][COLS] = {0};

struct termios saved_attributes;
typedef struct
{
  char x;
  char y;
  char *piece;
}current_piece;


// # ==== Terminal Input mode ==== #
void
reset_input_mode (void)
{
  tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

void
set_input_mode (void)
{
  struct termios tattr;
  char *name;

  /* Make sure stdin is a terminal. */
  if (!isatty (STDIN_FILENO))
    {
      fprintf (stderr, "Not a terminal.\n");
      exit (EXIT_FAILURE);
    }

  /* Save the terminal attributes so we can restore them later. */
  tcgetattr (STDIN_FILENO, &saved_attributes);
  atexit (reset_input_mode);

  /* Set the funny terminal modes. */
  tcgetattr (STDIN_FILENO, &tattr);
  tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
  tattr.c_cc[VMIN] = 1;
  tattr.c_cc[VTIME] = 0;
  tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}






// # ==== MAIN ==== #

int main(void)
{
  char option = 1;
  char ch;
  set_input_mode();
  while (option)
    {
      // Obtenir la touche
      read (STDIN_FILENO, &ch, 1);
      if (ch == 113)          /* q */
        break;
      else
	printf("%d\n", ch);
    }
 
  
  return 1;
}

#include "terminal.h"
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

// Function to set terminal input mode
void reset_input_mode (struct termios saved_attributes)
{
  tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

// Function to set terminal input mode
void set_input_mode (struct termios saved_attributes)
{
  struct termios tattr;

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


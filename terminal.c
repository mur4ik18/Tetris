#include "terminal.h"
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


// Function to reset terminal input mode to the saved attributes
void reset_input_mode (struct termios saved_attributes)
{
  /* Documentation:

     This function is responsible for resetting the terminal input mode to the saved attributes.
     It takes a struct termios parameter saved_attributes representing the saved terminal attributes.
     After calling this function, the terminal input mode will be restored to the state it was in before modifications.
  */

  // Set the terminal attributes to the saved ones
  tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

// Function to set terminal input mode to non-canonical and non-echo mode
void set_input_mode (struct termios saved_attributes)
{
  /*
    Documentation:

    This function is responsible for setting the terminal input mode to non-canonical and non-echo mode,
    allowing the program to read input characters individually without waiting for the Enter key.
    It takes a struct termios parameter saved_attributes to save the original terminal attributes,
    and atexit is used to register the reset_input_mode function to be called at program exit to ensure the terminal mode is properly reset.
   */
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

  /* Set the non-canonical and non-echo terminal modes. */
  tcgetattr (STDIN_FILENO, &tattr);
  tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
  tattr.c_cc[VMIN] = 1;
  tattr.c_cc[VTIME] = 0;
  tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}

